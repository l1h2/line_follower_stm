#include "hal/led_ll.h"

#include "main.h"

void board_led_on(void) { LL_GPIO_SetOutputPin(LED_GPIO_Port, LED_Pin); }

void board_led_off(void) { LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin); }

void board_led_toggle(void) { LL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin); }
