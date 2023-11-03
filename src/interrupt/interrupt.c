#include "interrupt.h"

#include <cpu.h>
#include <exception.h>

#include <stddef.h>
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

#define NUM_IRQ_HANDLERS                    (16)

static IrqHandlerFunction g_irq_handlers[IRQ_MAXIMUM_COUNT][NUM_IRQ_HANDLERS] = { 0 };

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
    *INTERRUPT_CTRL2_MASK_REG = (interrupts[1] & (INTR_MASK(IRQ_UNK38) | INTR_MASK(IRQ_UNK39))) >> 6;
    cpu_sync();
}

static void on_irq(unsigned int cause, unsigned int epc, unsigned int bad_vaddr, struct Registers *ctx)
{
    uint32_t *irqs = enabled_interrupts();
    uint32_t ctrl0_irqs = *INTERRUPT_CTRL0_UNMASKED_FLAGS_REG & irqs[0];
    
    // TODO: represent this better. what a fucking mess
    uint32_t ctrl1_irqs = ((*INTERRUPT_CTRL1_UNMASKED_FLAGS_REG & ~(INTR_MASK(IRQ_UNK38) | INTR_MASK(IRQ_UNK39))) | ((*INTERRUPT_CTRL2_UNMASKED_FLAGS_REG << 6) & (INTR_MASK(IRQ_UNK38) | INTR_MASK(IRQ_UNK39)))) * irqs[1];

    unsigned int irq_num = 0;

    if (ctrl0_irqs) {
        irq_num = cpu_clz(cpu_bitrev(ctrl0_irqs));
    }
    else if (ctrl1_irqs) {
        irq_num = cpu_clz(cpu_bitrev(ctrl0_irqs)) + 32;
    }
    else {
        // not our interrupt?
        return;
    }

    if (!g_irq_handlers[irq_num][0]) {
        return;
    }

    for (size_t handler = 0; handler < NUM_IRQ_HANDLERS && g_irq_handlers[irq_num][handler]; ++handler) {
        if (g_irq_handlers[irq_num][handler]() != IRQ_HANDLE_OK) {
            return;
        }
    }

    interrupt_clear(irq_num);
}

void interrupt_init(void)
{
    uint32_t mask = interrupt_suspend();
	cpu_set_status((cpu_get_status() & 0xFFFF00FF) | 0x400);
    uint32_t *interrupts = enabled_interrupts();
    interrupts[0] = interrupts[1] = 0;
    set_interrupt_masks(interrupts);
    interrupt_resume(mask);

    exception_register_handler(EXCEPTION_IRQ, on_irq);
    cpu_enable_interrupts();
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

void interrupt_set_handler(enum InterruptType irq, IrqHandlerFunction handler)
{
    if ((unsigned int)irq < IRQ_MAXIMUM_COUNT) {
        for (size_t i = 0; i < NUM_IRQ_HANDLERS; ++i) {
            if (!g_irq_handlers[irq][i]) {
                g_irq_handlers[irq][i] = handler;
                break;
            }
        }
    }
}
