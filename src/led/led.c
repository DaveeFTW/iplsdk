#include "led.h"

#include <syscon.h>
#include <sysreg.h>
#include <gpio.h>
#include <model.h>

typedef enum {
    ACTIVE_LOW,
    ACTIVE_HIGH,
} LogicLevel;

typedef struct {
    enum Led id;
    enum Ports gpio_port;
    LogicLevel logic_level;
    int initialised;
} LedState;

static LedState g_ms_led = {
    .gpio_port = GPIO_PORT_MS_LED,
    .initialised = 0,
};

static LedState g_wlan_led = {
    .gpio_port = GPIO_PORT_WLAN_LED,
    .initialised = 0,
};

static LedState g_bt_led = {
    .gpio_port = GPIO_PORT_BT_LED,
    .initialised = 0,
};


static LedState *g_led_states[] = {
    &g_ms_led,
    &g_wlan_led,
    &g_bt_led
};

static void turn_on_led(enum Ports port, LogicLevel level)
{
    switch (level) {
        case ACTIVE_HIGH:
            gpio_set(port);
            break;

        case ACTIVE_LOW:
            gpio_clear(port);
            break;
    }
}

static void turn_off_led(enum Ports port, LogicLevel level)
{
    switch (level) {
        case ACTIVE_HIGH:
            gpio_clear(port);
            break;

        case ACTIVE_LOW:
            gpio_set(port);
            break;
    }
}

static void init_ms_led(LogicLevel logic_level)
{
    g_ms_led.logic_level = logic_level;
    g_ms_led.initialised = 1;
    syscon_ctrl_led(SYSCON_LED_MS, 1);
	sysreg_io_enable_gpio_port(GPIO_PORT_MS_LED);
	gpio_set_port_mode(GPIO_PORT_MS_LED, GPIO_MODE_OUTPUT);
    turn_off_led(g_ms_led.gpio_port, g_ms_led.logic_level);
}

static void init_wlan_led(LogicLevel logic_level)
{
    g_wlan_led.logic_level = logic_level;
    g_wlan_led.initialised = 1;
    syscon_ctrl_led(SYSCON_LED_WLAN, 1);
	sysreg_io_enable_gpio_port(GPIO_PORT_WLAN_LED);
	gpio_set_port_mode(GPIO_PORT_WLAN_LED, GPIO_MODE_OUTPUT);
    turn_off_led(g_wlan_led.gpio_port, g_wlan_led.logic_level);
}

static void init_power_led(void)
{
    g_wlan_led.initialised = 1;
    syscon_ctrl_led(SYSCON_LED_POWER, 1);
}

static void init_bt_led(LogicLevel logic_level)
{
    g_bt_led.logic_level = logic_level;
    g_bt_led.initialised = 1;
    syscon_ctrl_led(SYSCON_LED_BT, 1);
	sysreg_io_enable_gpio_port(GPIO_PORT_BT_LED);
	gpio_set_port_mode(GPIO_PORT_BT_LED, GPIO_MODE_OUTPUT);
    turn_off_led(g_bt_led.gpio_port, g_bt_led.logic_level);
}

void led_init(void)
{
    const PspModelIdentity *identity = model_get_identity();

    switch (identity->model) {
        case PSP_MODEL_01G:
        case PSP_MODEL_04G:
        case PSP_MODEL_07G:
        case PSP_MODEL_09G:
            init_ms_led(ACTIVE_HIGH);
            init_wlan_led(ACTIVE_HIGH);
            init_power_led();
            break;
        
        case PSP_MODEL_02G:
        case PSP_MODEL_03G:
            init_ms_led(ACTIVE_HIGH);
            init_wlan_led(ACTIVE_LOW);
            init_power_led();
            break;
        
        case PSP_MODEL_05G:
            init_ms_led(ACTIVE_HIGH);
            init_wlan_led(ACTIVE_LOW);
            init_power_led();
            init_bt_led(ACTIVE_HIGH);
            break;
        
        case PSP_MODEL_11G:
            init_ms_led(ACTIVE_HIGH);
            init_power_led();
            break;
    }
}
