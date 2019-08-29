#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

enum Ports
{
    GPIO_PORT_LCD_RESET,
    GPIO_PORT_DEBUG_1,
    GPIO_PORT_DEBUG_2,
    GPIO_PORT_SYSCON_REQUEST,
    GPIO_PORT_TACHYON_SPI_CS,
    GPIO_PORT_HEADPHONE_EN,
    GPIO_PORT_MS_LED,
    GPIO_PORT_WLAN_LED,
    GPIO_PORT_DEBUG_8,
    GPIO_PORT_LEPTON_STATUS,
    GPIO_PORT_LEPTON_WAKE,
    GPIO_PORT_UMD_STATUS
};

enum PortMode
{
    GPIO_MODE_OUTPUT,
    GPIO_MODE_INPUT,
};

void gpio_set_port_mode(int port, enum PortMode mode);
void gpio_set(int port);
void gpio_clear(int port);
unsigned int gpio_read(int port);

#ifdef __cplusplus
}
#endif //__cplusplus
