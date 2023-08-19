#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

int syscon_init(void);

// we don't fully understand how the syscon/baryon version is constructed.
// what we do know is that the 3rd byte (mask 0x00FF0000) is strongly
// correlated with the model
#define BARYON_VERSION_MODEL_CODE(version)  ((version >> 16) & 0xFF)
#define BARYON_MODEL_CODE_IS_HANDSHAKE_TYPE1(code) (code == 0x2C || code == 0x2E || code == 0x40)
#define BARYON_MODEL_CODE_IS_HANDSHAKE_TYPE2(code) (code == 0x30)

unsigned int syscon_get_baryon_version(void);

int syscon_ctrl_led(unsigned int led, unsigned int on);
int syscon_ctrl_hr_power(unsigned int on);
int syscon_handshake_unlock(void);

#ifdef __cplusplus
}
#endif //__cplusplus
