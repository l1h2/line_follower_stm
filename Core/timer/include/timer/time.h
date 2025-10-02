#ifndef TIME_H
#define TIME_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initializes the timer module and system timer (SysTick).
 */
void init_timer(void);

/**
 * @brief Returns the current system time.
 *
 * @return The current system time in milliseconds.
 * @note overflows every ~49.7 days (2^32 ms)
 */
uint32_t time(void);

/**
 * @brief Returns the current system time in microseconds.
 *
 * @return The current system time in microseconds.
 * @note overflows every ~71.5 minutes (2^32 us)
 */
uint32_t time_us(void);

/**
 * @brief Checks if a specified duration has elapsed since a given start time.
 *
 * @param start The start time in milliseconds.
 * @param duration The duration in milliseconds to check against.
 * @return true if the interval has elapsed, false otherwise.
 */
bool time_elapsed(const uint32_t start, const uint32_t duration);

/**
 * @brief Checks if a specified duration has elapsed since a given start time in
 * microseconds.
 *
 * @param start The start time in microseconds.
 * @param duration The duration in microseconds to check against.
 * @return true if the interval has elapsed, false otherwise.
 */
bool time_elapsed_us(const uint32_t start, const uint32_t duration);

/**
 * @brief Delays execution for a specified number of milliseconds.
 *
 * @param ms The number of milliseconds to delay.
 */
void delay(const uint32_t ms);

/**
 * @brief Delays execution for a specified number of microseconds.
 *
 * @param us The number of microseconds to delay.
 */
void delay_us(const uint32_t us);

#endif  // TIME_H
