#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#pragma once

enum Led {
    LED_MEMORY_STICK,
    LED_WLAN,
    LED_POWER,
    LED_BLUETOOTH
};

void led_init(void);

#ifdef __cplusplus
}
#endif //__cplusplus
