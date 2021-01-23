#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void cpu_enable_interrupts(void);
unsigned int cpu_get_status(void);
void cpu_set_status(unsigned int status);

#ifdef __cplusplus
}
#endif //__cplusplus
