#include "uart.h"
#include "pl011.h"

#define UART_IO_BASE            (0xBE400000)
#define UART_DEVICE_TO_MMIO(x)  ((uintptr_t)UART_IO_BASE + ((uintptr_t)x*0x40000))

#define UARTCLK                 (192000000) // 192 MHz

void uart_init(enum UartDevice device, unsigned int baudrate)
{
    uintptr_t mmio_base = UART_DEVICE_TO_MMIO(device);

    // calculate the baud rate divisor integer and fractional
    // parts. the UARTCLK is 192 MHz
    unsigned int bauddiv = UARTCLK/(16*baudrate);
    unsigned int ibrd = bauddiv / 64;
    unsigned int fbrd = bauddiv % 64;

    pl011_uartibrd_write(mmio_base, ibrd);
    pl011_uartfbrd_write(mmio_base, fbrd);

    // set the word length to 8 bits
    pl011_uartlcr_h_write(mmio_base, UARTLCR_H_WLEN_8BIT);

    // enable send/receive and start the uart
    pl011_uartcr_write(mmio_base, UARTCR_RXE | UARTCR_TXE | UARTCR_UARTEN);

    // enable FIFO buffers
    unsigned int lcr = pl011_uartlcr_h_read(mmio_base);
    pl011_uartlcr_h_write(mmio_base, lcr | UARTLCR_H_FEN);

    // clear all interrupts
    pl011_uarticr_write(mmio_base, UARTICR_ALL);
}

void uart_putc(enum UartDevice device, char c)
{
    uintptr_t mmio_base = UART_DEVICE_TO_MMIO(device);
    
    // wait until the transfer FIFO has space then write
    // the character
    while (pl011_uartfr_read(mmio_base) & UARTFR_TXFF);
    pl011_uartdr_write(mmio_base, c);
}

int uart_getc(enum UartDevice device)
{
    uintptr_t mmio_base = UART_DEVICE_TO_MMIO(device);
    
    // wait until the receive FIFO has some data then
    /// read the character
    while (pl011_uartfr_read(mmio_base) & UARTFR_RXFE);
    return pl011_uartdr_read(mmio_base) & UARTDR_DATA_MASK;
}

void uart_puts(enum UartDevice device, const char *s)
{
    while (*s) {
        uart_putc(device, *s);
        s++;
    }
}

void uart_flush_tx(enum UartDevice device)
{
    uintptr_t mmio_base = UART_DEVICE_TO_MMIO(device);
    
    // wait until the transfer FIFO is empty
    while (!(pl011_uartfr_read(mmio_base) & UARTFR_TXFE));
}

