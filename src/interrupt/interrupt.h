#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

enum InterruptType
{
    IRQ_KIRK = 24
};

void interrupt_init(void);
void interrupt_enable(enum InterruptType interrupt);

unsigned int interrupt_suspend(void);
void interrupt_resume(unsigned int mask);
void interrupt_resume_with_sync(unsigned int mask);

int interrupt_occured(enum InterruptType interrupt);
void interrupt_clear(enum InterruptType interrupt);

#ifdef __cplusplus
}
#endif //__cplusplus
