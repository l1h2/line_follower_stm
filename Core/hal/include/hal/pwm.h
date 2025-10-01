#ifndef PWM_H
#define PWM_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_PWM 1000  // Maximum PWM value for motor speed
#define MIN_PWM 0     // Minimum PWM value for motor speed

typedef enum { MOTOR_LEFT, MOTOR_RIGHT, MOTOR_TURBINE } Motors;

/**
 * @brief Initialize the PWMs for motor control.
 */
void init_pwm(void);

/**
 * @brief Set the PWM value for a specific motor.
 *
 * @param motor The motor to set the PWM for (MOTOR_LEFT, MOTOR_RIGHT,
 * MOTOR_TURBINE).
 * @param value The PWM value to set (0 to 1000).
 */
void set_pwm(const Motors motor, uint32_t value);

/**
 * @brief Get the current PWM value for a specific motor.
 *
 * @param motor The motor to get the PWM value for (MOTOR_LEFT, MOTOR_RIGHT,
 * MOTOR_TURBINE).
 * @return The current PWM value (0 to 1000).
 */
uint32_t get_pwm(const Motors motor);

/**
 * @brief Set the direction of the right motor.
 *
 * @param forward true for forward direction, false for backward.
 */
void set_motor_left_dir(const bool forward);

/**
 * @brief Set the direction of the right motor.
 *
 * @param forward true for forward direction, false for backward.
 */
void set_motor_right_dir(const bool forward);

#endif  // PWM_H
