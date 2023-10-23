#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include <stdint.h>

enum ExceptionType
{
    EXCEPTION_IRQ = 0,
    EXCEPTION_SYSCALL = 8
};

struct Registers
{
    uint32_t at;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t t7;
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t t8;
    uint32_t t9;
    uint32_t k0;
    uint32_t k1;
    uint32_t gp;
    uint32_t sp;
    uint32_t fp;
    uint32_t ra;
    uint32_t lo;
    uint32_t hi;
    uint32_t v0;
    uint32_t v1;
};

typedef void (EBASE_HANDLER)(void);
typedef void (EXCEPTION_HANDLER)(unsigned int cause, unsigned int epc, unsigned int bad_vaddr, struct Registers *ctx);

void exception_register_handler(enum ExceptionType cause, EXCEPTION_HANDLER *handler);
void exception_register_default_handler(EXCEPTION_HANDLER *handler);

void exception_register_ebase_handler(EBASE_HANDLER *handler);
EBASE_HANDLER *exception_get_ebase_handler(void);

const char *exception_get_cause_string(int cause);
enum ExceptionType exception_get_cause(int cause);

void exception_init(void);

#ifdef __cplusplus
}
#endif //__cplusplus
