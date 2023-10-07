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

int syscon_ctrl_power(unsigned int dev, unsigned int arg);

typedef enum {
    SYSCON_LED_MS,
    SYSCON_LED_WLAN,
    SYSCON_LED_POWER,
    SYSCON_LED_BT
} SysconLed;

int syscon_ctrl_led(SysconLed led, unsigned int on);
int syscon_ctrl_hr_power(unsigned int on);
int syscon_handshake_unlock(void);
int syscon_get_pommel_version(unsigned int *version);
int syscon_get_power_status(unsigned int *status);
int syscon_ctrl_voltage(unsigned int a0, unsigned int a1);

#ifdef __cplusplus
}
#endif //__cplusplus
