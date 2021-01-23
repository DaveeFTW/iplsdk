#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

enum ExceptionType
{
    EXCEPTION_IRQ = 0,
    EXCEPTION_SYSCALL = 8
};

typedef void (EBASE_HANDLER)(void);
void register_ebase_handler(EBASE_HANDLER *handler);
EBASE_HANDLER *get_ebase_handler(void);

const char *exception_get_cause_string(int cause);
enum ExceptionType exception_get_cause(int cause);

#ifdef __cplusplus
}
#endif //__cplusplus
