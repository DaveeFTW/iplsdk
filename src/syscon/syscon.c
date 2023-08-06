#include "gpio.h"
#include "sysreg.h"

#define REG32(addr) ((volatile unsigned int *)(addr))

#define SYSCON_MAX_PACKET_SIZE  (0x10)

struct SysconPacketHeader {
    unsigned char cmd;
    unsigned char length;
}  __attribute__ ((__packed__));

enum SysconCommand {
    SYSCON_GET_BARYON_VERSION = 1,
    SYSCON_CTRL_HR_POWER = 0x34,
    SYSCON_CTRL_LED = 0x47,
};

static unsigned char calculate_checksum(unsigned char *data, unsigned int length)
{
    unsigned char sum = 0;

    for (unsigned i = 0; i < length; ++i) {
        sum += data[i];
    }

    return ~sum;
}

static void syscon_transmit_packet(unsigned char *packet, unsigned char length)
{
    // calculate a checksum for the packet
    packet[length] = calculate_checksum(packet, length);
    packet[length + 1] = 0xFF;

    // signal syscon that we want to transmit a packet to it
    gpio_read();
    gpio_clear(GPIO_PORT_SYSCON_REQUEST);

    // read out any data from the SPI shift register
    while (*REG32(0xBE58000C) & 0b100)
    {
        *REG32(0xBE580008);
    }

    // initiate a SPI transfer
    *REG32(0xBE58000C);
    *REG32(0xBE580020) = 3;

    /* this exist in the original syscon code, but we have no use for it here
    if (data->unk_4 & 0x40000) {
        *REG32(0xBE580008) = (data->unk_c << 8) | 2;
        *REG32(0xBE580008) = ~((data->unk_c + 2) << 8);
    }
    else
    */
    {
        for (unsigned int i = 0; i < (length + 1); i += 2) {
            *REG32(0xBE58000C);
            *REG32(0xBE580008) = (packet[i] << 8) | packet[i + 1];
        }
    }

    // finalise SPI transfer and tell syscon we're done on our side
    *REG32(0xBE580004) = 6;
    gpio_set(GPIO_PORT_SYSCON_REQUEST);
}

int syscon_receive_packet(unsigned char *packet)
{
    int result = 0;

    // we should expect some sort of reply from syscon
    // if there is no data to read then something has gone wrong
    // if ((*REG32(0xBE58000C) & 0b100) == 0) {
    //     packet[0] = 0xFF;
    //     packet[1] = 0;
    //     result = -1;
    //     for (unsigned int i = 0xF; i > 0; --i);
    // }

    // // this appears to be some protocol error condition (?)
    // if ((*REG32(0xBE58000C) & 0b1) == 0) {
    //     result = -1;
    // }

    // // not sure what SPI condition this is either
    // if (*REG32(0xBE580018) & 0b1) {
    //     *REG32(0xBE580020) = 1;
    // }

    // read the response from syscon into our packet buffer
    for (unsigned int i = 0; i < SYSCON_MAX_PACKET_SIZE; i += 2) {
        // if there is no more data to be read then exit
        if ((*REG32(0xBE58000C) & 0b100) == 0) {
            break;
        }

        // read the data from the shift register
        const unsigned int read_data = *REG32(0xBE580008) & 0xFFFF;

        // the first byte (id) is our result
        if (i == 0) {
            result = read_data >> 8;
        }

        packet[i] = read_data >> 8;
        packet[i + 1] = read_data & 0xFF;
    }

    *REG32(0xBE580004) = 4;
    gpio_clear(GPIO_PORT_SYSCON_REQUEST);

    // if there is an error we can now safely exit
    if (result < 0) {
        return result;
    }
    
    // get the length from the packet
    unsigned char length = ((struct SysconPacketHeader *)packet)->length;

    // it should be at least 3 bytes to fit the header and the minimum data size of 1
    // and not more (or equal since there is checksum byte) than the max packet size
    if (length < (sizeof(struct SysconPacketHeader) + 1) ||
        length >= SYSCON_MAX_PACKET_SIZE) {
        return -2;
    }

    // finally if the checksum doesn't match we return an error
    if (calculate_checksum(packet, length) != packet[length]) {
        return -2;
    }

    return result;
}

static int syscon_issue_command(enum SysconCommand cmd, const unsigned char *data, unsigned int length, unsigned char *out_data, unsigned int* out_size)
{
    unsigned char packet[SYSCON_MAX_PACKET_SIZE];
    struct SysconPacketHeader *header = (struct SysconPacketHeader *)packet;

    // fill the packet with the command data
    header->cmd = cmd;
    header->length = length + sizeof(struct SysconPacketHeader);
    for (unsigned int i = 0; i < length; ++i) {
        packet[sizeof(struct SysconPacketHeader) + i] = data[i];
    }

retry:
    // send the packet to syscon
    syscon_transmit_packet(packet, header->length);

    // now we poll the syscon GPIO to tell us when it has responded
    while (gpio_query_interrupt(GPIO_PORT_TACHYON_SPI_CS) == 0);
    gpio_acquire_interrupt(GPIO_PORT_TACHYON_SPI_CS);

    // read the result from syscon, and return it to the caller
    const int result = syscon_receive_packet(out_data);
    struct SysconPacketHeader *out_header = (struct SysconPacketHeader *)out_data;
    *out_size = out_header->length - sizeof(struct SysconPacketHeader);
    
    switch (out_data[2]) {
        case 0x80:
        case 0x81:
            goto retry;
        default:
            break;
    }
    return 0;
}

static int syscon_issue_command_write(enum SysconCommand cmd, const unsigned char *data, unsigned int length)
{
    unsigned char dummy_packet[SYSCON_MAX_PACKET_SIZE];
    unsigned int dummy_length;
    return syscon_issue_command(cmd, data, length, dummy_packet, &dummy_length);
}

static int syscon_issue_command_read(enum SysconCommand cmd, unsigned char *data)
{
    unsigned char recv_packet[SYSCON_MAX_PACKET_SIZE];
    unsigned int recv_length;
    int res = syscon_issue_command(cmd, (void *)0, 0, recv_packet, &recv_length);

    // exit early if we have some error
    if (res < 0) {
        return res;
    }

    // copy data from syscon to the provided parameter
    for (int i = 0; i < recv_length; ++i) {
        data[i] = recv_packet[sizeof(struct SysconPacketHeader) + i];
    }

    return res;
}

void syscon_init(void)
{
    // enable GPIO to signal syscon
    sysreg_io_enable_gpio_port(GPIO_PORT_SYSCON_REQUEST);
    gpio_set_port_mode(GPIO_PORT_SYSCON_REQUEST, GPIO_MODE_OUTPUT);
	sysreg_io_enable_gpio();

    // syscon communicates over an SPI channel to the allegrex processor.
    // syscon holds the "master" device status, and the allegrex uses a
    // GPIO to signal to syscon when to initiate a message transfer to it.
    sysreg_clock_select_spi(0, 1);
    sysreg_clock_enable_spi(0);
    sysreg_io_enable_spi(0);

    // its not totally clear what device 0xBE580000 is, but its used solely
    // for syscon SPI communication.
    *REG32(0xBE580000) = 0xCF;
    *REG32(0xBE580004) = 4;
    *REG32(0xBE580014) = 0;
    *REG32(0xBE580024) = 0;

    // initialise the GPIOs that syscon uses
    gpio_clear(GPIO_PORT_SYSCON_REQUEST);
    gpio_set_port_mode(GPIO_PORT_SYSCON_REQUEST, GPIO_MODE_OUTPUT);
    gpio_set_port_mode(GPIO_PORT_TACHYON_SPI_CS, GPIO_MODE_INPUT);

	*REG32(0xbe240010) &= ~0x10;
	*REG32(0xbe240014) &= ~0x10;
	*REG32(0xbe240018) |=  0x10;
	*REG32(0xbe240024) = 0x10;
}

int syscon_get_baryon_version(unsigned int *baryon)
{
    return syscon_issue_command_read(SYSCON_GET_BARYON_VERSION, (unsigned char *)baryon);
}

int syscon_ctrl_led(unsigned int led, unsigned int on)
{
    unsigned char led_cmd = 0;

    // get the bit that represents the LED we're interested in
    switch (led) {
        // memory stick
        case 0:
            led_cmd = 0x40;
            break;
        // wlan
        case 1:
            led_cmd = 0x80;
            break;
        // power
        case 2:
            led_cmd = 0x20;
            break;
        // bluetooth
        case 3:
            // TODO: check pommel type. error if < 0x300
            led_cmd = 0x10;
            break;
        default:
            return -1;
    }

    if (on) 
    {
        // the "on" bit changed between PSP-1000, and beyond select the
        // correct bit as appropriate
        unsigned int baryon = 0;
        if (syscon_get_baryon_version(&baryon) < 0) {
            return -2;
        }

        const unsigned char psp_model = (baryon >> 16) & 0xF0;
        const unsigned char is_psp1000 = psp_model == 0x00 || psp_model == 0x10;
        led_cmd |= (is_psp1000) ? (0x10) : (0x1);
    }

    // write message to syscon to modify LED state
    return syscon_issue_command_write(SYSCON_CTRL_LED, &led_cmd, 1);
}

int syscon_ctrl_hr_power(unsigned int on)
{
    unsigned char on_val = on;

    // write message directly to syscon
    syscon_issue_command_write(SYSCON_CTRL_HR_POWER, &on_val, 1);
}