#include "sysreg.h"

#define REG32(addr) ((volatile unsigned int *)(addr))

void sysreg_clock_enable_uart_bus(void)
{
    *REG32(0xbc100050) |= 0x4000;
}

void sysreg_clock_enable_uart(int port)
{
    *REG32(0xbc100058) |= (0x40 << port);
}

void sysreg_io_enable_uart(int port)
{
    *REG32(0xbc100078) |= (0x00010000 << port);
}
