#include "kirk.h"
#include "utils.h"

typedef struct
{
    volatile unsigned int signature;
    volatile unsigned int version;
    volatile unsigned int error;
    volatile unsigned int proc_phase;
    volatile unsigned int command;
    volatile unsigned int result;
    volatile unsigned int unk_18;
    volatile unsigned int status;
    volatile unsigned int status_async;
    volatile unsigned int status_async_end;
    volatile unsigned int status_end;
    volatile unsigned int src_addr;
    volatile unsigned int dst_addr;
} PspKirkRegs;

#define MAKE_PHYS_ADDR(_addr)    (((unsigned int)_addr) & 0x1FFFFFFF)
#define SYNC()                   asm("sync")
#define KIRK_HW_REGISTER_ADDR    ((PspKirkRegs *)0xBDE00000)
 
int kirk1(void *dst, const void *src)
{
    clear_dcache();
    PspKirkRegs *const kirk = KIRK_HW_REGISTER_ADDR;
 
    kirk->command = 1; // decrypt operation
    kirk->src_addr = MAKE_PHYS_ADDR(src);
    kirk->dst_addr = MAKE_PHYS_ADDR(dst);
 
    kirk->proc_phase = 1; // start processing
 
    while((kirk->status & 0x11) == 0); // wait until processing complete
 
    if (kirk->status & 0x10) // error occured
    {
        kirk->proc_phase = 2;
 
        while((kirk->status & 2) == 0);
 
        kirk->status_end = kirk->status;
        SYNC();
        return -1;
    }
 
    kirk->status_end = kirk->status;
    SYNC();
    return kirk->result;
}

int kirk4(void *dst, const void *src)
{
    clear_dcache();
    PspKirkRegs *const kirk = KIRK_HW_REGISTER_ADDR;
 
    kirk->command = 4; // encrypt operation
    kirk->src_addr = MAKE_PHYS_ADDR(src);
    kirk->dst_addr = MAKE_PHYS_ADDR(dst);
 
    kirk->proc_phase = 1; // start processing
 
    while((kirk->status & 0x11) == 0); // wait until processing complete
 
    if (kirk->status & 0x10) // error occured
    {
        kirk->proc_phase = 2;
 
        while((kirk->status & 2) == 0);
 
        kirk->status_end = kirk->status;
        SYNC();
        return -1;
    }
 
    kirk->status_end = kirk->status;
    SYNC();
    return kirk->result;
}

int kirk7(void *dst, const void *src)
{
    clear_dcache();
    PspKirkRegs *const kirk = KIRK_HW_REGISTER_ADDR;
 
    kirk->command = 7; // decrypt operation
    kirk->src_addr = MAKE_PHYS_ADDR(src);
    kirk->dst_addr = MAKE_PHYS_ADDR(dst);
 
    kirk->proc_phase = 1; // start processing
 
    while((kirk->status & 0x11) == 0); // wait until processing complete
 
    if (kirk->status & 0x10) // error occured
    {
        kirk->proc_phase = 2;
 
        while((kirk->status & 2) == 0);
 
        kirk->status_end = kirk->status;
        SYNC();
        return -1;
    }
 
    kirk->status_end = kirk->status;
    SYNC();
    return kirk->result;
}
