#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initializes the system timer.
 */
void init_system_timer(void);

/**
 * @brief Returns the current system time.
 *
 * @return The current system time in milliseconds.
 * @warning This function may overflow after approximately 49.7 days.
 */
uint32_t get_system_time(void);

/**
 * @brief Returns the current system time in microseconds.
 *
 * @return The current system time in microseconds.
 * @warning This function may overflow after approximately 71.5 minutes.
 */
uint32_t get_system_time_us(void);

/**
 * @brief Checks if a specified duration has elapsed since a given start time.
 *
 * @param start The start time in milliseconds.
 * @param duration The duration in milliseconds to check against.
 * @return true if the interval has elapsed, false otherwise.
 */
bool time_elapsed_ms(const uint32_t start, const uint32_t duration);

/**
 * @brief Delays execution for a specified number of milliseconds.
 *
 * @param ms The number of milliseconds to delay.
 */
void delay_ms(const uint32_t ms);

#endif  // TIMER_H
