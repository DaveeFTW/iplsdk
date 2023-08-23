#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

static inline uint32_t cpu_bitrev(uint32_t x)
{
    uint32_t res;
    __asm("bitrev %0, %1" : "=r" (res) : "r"(x));
    return res;
}

static inline uint32_t cpu_clz(uint32_t x)
{
    uint32_t res;
    __asm("clz %0, %1" : "=r" (res) : "r"(x));
    return res;
}

static inline void cpu_sync(void)
{
    __asm("sync");
}

void cpu_enable_interrupts(void);
unsigned int cpu_get_status(void);
void cpu_set_status(unsigned int status);

void cpu_dcache_wb_inv_all(void);
void cpu_icache_inv_all(void);

#ifdef __cplusplus
}
#endif //__cplusplus
