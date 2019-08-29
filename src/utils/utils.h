#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void clear_dcache(void);
void clear_icache(void);

unsigned int delay_us(unsigned int usec);

#ifdef __cplusplus
}
#endif //__cplusplus
