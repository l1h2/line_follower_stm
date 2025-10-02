#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>

/**
 * @brief Initialize the motors by setting up the PWM.
 */
void init_motors(void);

/**
 * @brief Get the maximum PWM value for the motors.
 * @return The maximum PWM value.
 */
uint16_t get_max_pwm(void);

/**
 * @brief Get the minimum PWM value for the motors.
 * @return The minimum PWM value.
 */
uint16_t get_min_pwm(void);

/**
 * @brief Set the PWM for the left motor.
 * @param pwm The PWM value to set.
 */
void set_motor_left(const int16_t pwm);

/**
 * @brief Set the PWM for the right motor.
 * @param pwm The PWM value to set.
 */
void set_motor_right(const int16_t pwm);

/**
 * @brief Set the PWM for both motors.
 * @param left_pwm The PWM value for the left motor.
 * @param right_pwm The PWM value for the right motor.
 */
void set_motors(const int16_t left_pwm, const int16_t right_pwm);

/**
 * @brief Get the current PWM of the left motor.
 * @return The current PWM value of the left motor.
 */
uint16_t get_motor_left_pwm(void);

/**
 * @brief Get the current PWM of the right motor.
 * @return The current PWM value of the right motor.
 */
uint16_t get_motor_right_pwm(void);

/**
 * @brief Get the current PWM values of both motors.
 * @param left_pwm Pointer to store the left motor PWM value.
 * @param right_pwm Pointer to store the right motor PWM value.
 */
void get_motors_pwm(uint16_t* left_pwm, uint16_t* right_pwm);

/**
 * @brief Stop both motors by setting their PWM to zero.
 */
void stop_motors(void);

#endif  // MOTORS_H
