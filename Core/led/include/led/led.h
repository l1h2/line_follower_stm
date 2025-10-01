#ifndef LED_H
#define LED_H

#include <stdint.h>

/**
 * @brief Turns the LED on.
 */
void led_on(void);

/**
 * @brief Turns the LED off.
 */
void led_off(void);

/**
 * @brief Toggles the LED state.
 */
void led_toggle(void);

/**
 * @brief Blinks the LED a specified number of times with a given interval.
 *
 * @param times Number of times to blink the LED.
 * @param interval_ms Interval in milliseconds between toggles.
 * @warning This function is blocking and will halt execution for the duration
 * of the blinking.
 */
void led_blink(const uint32_t times, const uint32_t interval_ms);

/**
 * @brief Continuously blinks the LED with a specified interval.
 *
 * @param interval_ms Interval in milliseconds between toggles.
 * @warning This function is blocking and will halt execution indefinitely.
 */
void led_blinker(const uint32_t interval_ms);

#endif  // LED_H
