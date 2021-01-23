#include "interrupt.h"

int interrupt_occured(enum InterruptType interrupt)
{
    unsigned int occured = *(volatile unsigned int *)0xBC300000;
    return occured & (1 << interrupt);
}

void interrupt_clear(enum InterruptType interrupt)
{
    *(volatile unsigned int *)0xBC300000 = (1 << interrupt);
}