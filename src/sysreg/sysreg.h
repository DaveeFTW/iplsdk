#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void sysreg_clock_enable_uart_bus(void);
void sysreg_clock_enable_uart(int port);
void sysreg_io_enable_uart(int port);

// APB
void sysreg_clock_enable_apb_bus(void);

// EMC SM
void sysreg_clock_enable_emc_sm_bus(void);
void sysreg_io_enable_emc_sm(void);

// EMC DDR
void sysreg_clock_enable_emc_ddr_bus(void);

// GPIO
void sysreg_io_enable_gpio(void);
void sysreg_io_enable_gpio_port(int port);

// I2C
void sysreg_clock_enable_i2c(void);
void sysreg_io_enable_i2c(void);

// PWM
void sysreg_clock_enable_pwm(void);
void sysreg_io_enable_pwm(void);

// LCDC
void sysreg_clock_enable_lcdc(void);
void sysreg_io_enable_lcdc(void);

// USB
void sysreg_clock_enable_usb_bus(void);
void sysreg_io_enable_usb(void);

// SPI
void sysreg_io_enable_spi(unsigned int port);
void sysreg_clock_enable_spi(unsigned int port);
void sysreg_clock_select_spi(unsigned int port, unsigned int clk);

#ifdef __cplusplus
}
#endif //__cplusplus
