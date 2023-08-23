#include "interrupt.h"

#include <stdint.h>

#define REG32(addr)                         ((volatile uintptr_t *)(addr))
#define INTERRUPT_MMIO_BASE                 (0xBC300000)

#define INTERRUPT_CTRL0_UNMASKED_FLAGS_REG  (REG32(INTERRUPT_MMIO_BASE + 0x00))
#define INTERRUPT_CTRL0_FLAGS_REG           (REG32(INTERRUPT_MMIO_BASE + 0x04))
#define INTERRUPT_CTRL0_MASK_REG            (REG32(INTERRUPT_MMIO_BASE + 0x08))

#define INTERRUPT_CTRL1_UNMASKED_FLAGS_REG  (REG32(INTERRUPT_MMIO_BASE + 0x10))
#define INTERRUPT_CTRL1_FLAGS_REG           (REG32(INTERRUPT_MMIO_BASE + 0x14))
#define INTERRUPT_CTRL1_MASK_REG            (REG32(INTERRUPT_MMIO_BASE + 0x18))

#define INTERRUPT_CTRL2_UNMASKED_FLAGS_REG  (REG32(INTERRUPT_MMIO_BASE + 0x20))
#define INTERRUPT_CTRL2_FLAGS_REG           (REG32(INTERRUPT_MMIO_BASE + 0x24))
#define INTERRUPT_CTRL2_MASK_REG            (REG32(INTERRUPT_MMIO_BASE + 0x28))

#define INTR_MASK(x)                        (1 << (x % 32))

#define ALWAYS_ENABLED_INTERRUPTS           (INTR_MASK(IRQ_ALL_UART) | INTR_MASK(IRQ_ALL_SPI) | INTR_MASK(IRQ_ALL_TIMERS) | INTR_MASK(IRQ_ALL_USB) | INTR_MASK(IRQ_UNK29))

static inline void sync(void)
{
    __asm("sync");
}

static inline uint32_t *enabled_interrupts(void)
{
    static uint32_t masks[2] = { 0 };
    return &masks[0];
}

static inline int is_ctrl2_interrupt(enum InterruptType interrupt)
{
    return interrupt == IRQ_UNK38 || interrupt == IRQ_UNK39;
}

static inline int is_ctrl1_interrupt(enum InterruptType interrupt)
{
    return interrupt >= 32 && !is_ctrl2_interrupt(interrupt);
}

static inline int is_ctrl0_interrupt(enum InterruptType interrupt)
{
    return interrupt < 32;
}

static inline void set_interrupt_masks(uint32_t *interrupts)
{
    *INTERRUPT_CTRL0_MASK_REG = interrupts[0] | ALWAYS_ENABLED_INTERRUPTS;
    *INTERRUPT_CTRL1_MASK_REG = interrupts[1] & ~(INTR_MASK(IRQ_UNK38) | INTR_MASK(IRQ_UNK39));
    *INTERRUPT_CTRL2_MASK_REG = (interrupts[2] & (INTR_MASK(IRQ_UNK38) | INTR_MASK(IRQ_UNK39))) >> 6;
    sync();
}

void interrupt_init(void)
{
    uint32_t mask = interrupt_suspend();
    uint32_t *interrupts = enabled_interrupts();
    interrupts[0] = interrupts[1] = 0;
    set_interrupt_masks(interrupts);
    interrupt_resume(mask);
}

void interrupt_enable(enum InterruptType interrupt)
{
    uint32_t mask = interrupt_suspend();

    uint32_t *interrupts = enabled_interrupts();
    interrupts[interrupt/32] = (1 << (interrupt % 32));
    set_interrupt_masks(interrupts);
    interrupt_resume(mask);
}

int interrupt_occured(enum InterruptType interrupt)
{
    if (is_ctrl0_interrupt(interrupt)) {
        return *INTERRUPT_CTRL0_UNMASKED_FLAGS_REG & INTR_MASK(interrupt);
    }

    else if (is_ctrl1_interrupt(interrupt)) {
        return *INTERRUPT_CTRL1_UNMASKED_FLAGS_REG & INTR_MASK(interrupt);
    }
    
    else { // if (is_ctrl2_interrupt(interrupt)) {
        return *INTERRUPT_CTRL2_UNMASKED_FLAGS_REG & (INTR_MASK(interrupt) >> 6);
    }
}

void interrupt_clear(enum InterruptType interrupt)
{
    if (is_ctrl0_interrupt(interrupt)) {
        *INTERRUPT_CTRL0_UNMASKED_FLAGS_REG = INTR_MASK(interrupt);
    }

    else if (is_ctrl1_interrupt(interrupt)) {
        *INTERRUPT_CTRL1_UNMASKED_FLAGS_REG = INTR_MASK(interrupt);
    }
    
    else { // if (is_ctrl2_interrupt(interrupt)) {
        *INTERRUPT_CTRL2_UNMASKED_FLAGS_REG = (INTR_MASK(interrupt) >> 6);
    }
}
