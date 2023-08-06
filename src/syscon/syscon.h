#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void syscon_init(void);
int syscon_get_baryon_version(unsigned int *baryon);
int syscon_ctrl_led(unsigned int led, unsigned int on);
int syscon_ctrl_hr_power(unsigned int on);

#ifdef __cplusplus
}
#endif //__cplusplus
