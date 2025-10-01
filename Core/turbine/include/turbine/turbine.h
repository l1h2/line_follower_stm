#ifndef TURBINE_H
#define TURBINE_H

#include <stdbool.h>
#include <stdint.h>

#define TURBINE_DEFAULT_ON_TIME 3000  // Time for turbine to be on in ms

/**
 * @brief Set the desired turbine pwm.
 * @param pwm The desired pwm value.
 */
void set_turbine_pwm(const uint16_t pwm);

/**
 * @brief Get the current turbine pwm.
 * @return The current turbine pwm value.
 */
uint16_t get_turbine_pwm(void);

/**
 * @brief Update the turbine pwm and set it to the motor.
 * @param pwm The desired pwm value.
 */
void update_turbine_pwm(const uint16_t pwm);

/**
 * @brief Check if the turbine is set to run.
 * @return true if the turbine can run, false otherwise.
 */
bool turbine_can_run(void);

/**
 * @brief Turn on the turbine and wait for the specified time.
 * @param wait The time to wait in milliseconds.
 * @note The wait time is meant to allow the turbine to generate vacuum before
 * starting operation.
 */
void turn_on_turbine(const uint32_t wait);

/**
 * @brief Turn on the turbine and wait asynchronously for the specified time.
 * @param wait The time to wait in milliseconds.
 * @return true if the wait time has elapsed, false otherwise.
 * @note The wait time is meant to allow the turbine to generate vacuum before
 * starting operation.
 */
bool turn_on_turbine_async(const uint32_t wait);

/**
 * @brief Turn off the turbine.
 */
void turn_off_turbine(void);

#endif  // TURBINE_H
