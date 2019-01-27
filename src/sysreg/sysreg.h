#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void sysreg_clock_enable_uart_bus(void);
void sysreg_clock_enable_uart(int port);
void sysreg_io_enable_uart(int port);

#ifdef __cplusplus
}
#endif //__cplusplus
