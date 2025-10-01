#include "led/led.h"

#include "hal/led_ll.h"
#include "timer/time.h"

void led_on(void) { board_led_on(); }

void led_off(void) { board_led_off(); }

void led_toggle(void) { board_led_toggle(); }

void led_blink(const uint32_t times, const uint32_t interval_ms) {
    for (uint32_t i = 0; i < times; i++) {
        led_toggle();
        delay(interval_ms);
        led_toggle();
        delay(interval_ms);
    }
}

void led_blinker(const uint32_t interval_ms) {
    while (1) {
        led_toggle();
        delay(interval_ms);
    }
}
