#include "exception.h"
#include "ebasehandler.h"

#include <cpu.h>

#include <string.h>

static void inf_loop(unsigned int cause, unsigned int epc, unsigned int bad_vaddr, struct Registers *ctx)
{
    while (1);
}

static EXCEPTION_HANDLER *g_irq_exception_handler = inf_loop;
static EXCEPTION_HANDLER *g_syscall_exception_handler = inf_loop;
static EXCEPTION_HANDLER *g_default_exception_handler = inf_loop;

static const char *g_exceptionCause[32] = 
{
    "Interrupt", "TLB modification", "TLB load/inst fetch", "TLB store",
    "Address load/inst fetch", "Address store", "Bus error (instr)", 
    "Bus error (data)", "Syscall", "Breakpoint", "Reserved instruction", 
    "Coprocessor unusable", "Arithmetic overflow", "Unknown 13", "Unknown 14", 
	"FPU Exception", "Unknown 16", "Unknown 17", "Unknown 18",
	"Unknown 20", "Unknown 21", "Unknown 22", "Unknown 23", 
	"Unknown 24", "Unknown 25", "Unknown 26", "Unknown 27", 
	"Unknown 28", "Unknown 29", "Unknown 30", "Unknown 31"
};

const char *exception_get_cause_string(int cause)
{
    return g_exceptionCause[(cause >> 2) & 0xF];
}

enum ExceptionType exception_get_cause(int cause)
{
    return (cause >> 2) & 0xF;
}

void exception_handler(unsigned int cause, unsigned int epc, unsigned int bad_vaddr, struct Registers *ctx)
{
    switch (exception_get_cause(cause)) {
        case EXCEPTION_IRQ:
            g_irq_exception_handler(cause, epc, bad_vaddr, ctx);
            break;

        
        case EXCEPTION_SYSCALL:
            g_syscall_exception_handler(cause, epc, bad_vaddr, ctx);
            break;

        default:
            g_default_exception_handler(cause, epc, bad_vaddr, ctx);
            break;
    }
}

void exception_register_handler(enum ExceptionType cause, EXCEPTION_HANDLER *handler)
{
    switch (cause) {
        case EXCEPTION_IRQ:
            g_irq_exception_handler = handler;
            break;

        
        case EXCEPTION_SYSCALL:
            g_syscall_exception_handler = handler;
            break;
    }
}

void exception_register_default_handler(EXCEPTION_HANDLER *handler)
{
    g_default_exception_handler = handler;
}

void exception_init(void)
{
	memcpy((void *)0xBFC00000, ebase_handler, ebase_handler_size);
	cpu_dcache_wb_inv_all();
	cpu_icache_inv_all();

	exception_register_ebase_handler(ebase_handler);
}
