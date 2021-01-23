#include "exception.h"

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
