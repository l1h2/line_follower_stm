#ifndef PID_H
#define PID_H

#include <stdbool.h>
#include <stdint.h>

#include "pid_base.h"

/**
 * @brief Initializes the PID controller and all related registers.
 */
void init_pid(void);

/**
 * @brief Returns a pointer to the PID struct.
 * @return Pointer to the PidStruct containing PID parameters and state.
 */
const PidStruct* get_pid(void);

/**
 * @brief Updates the PID controller with the current error values.
 * @return true if the PID controller was updated, false otherwise.
 */
bool update_pid(void);

/**
 * @brief Set the proportional gain (Kp) value for the PID controller.
 * @param kp The new proportional gain value to be set.
 */
void set_kp(const uint8_t kp);

/**
 * @brief Set the integral gain (Ki) value for the PID controller.
 * @param ki The new integral gain value to be set.
 */
void set_ki(const uint8_t ki);

/**
 * @brief Set the derivative gain (Kd) value for the PID controller.
 * @param kd The new derivative gain value to be set.
 */
void set_kd(const uint16_t kd);

/**
 * @brief Resets the current PWM used to the base PWM value.
 */
void reset_pwm(void);

/**
 * @brief Restarts the current PWM used to the initial PWM value.
 */
void restart_pwm(void);

/**
 * @brief Sets the base PWM value for the motors.
 * @param pwm The new base PWM value to be set.
 */
void set_base_pwm(const uint16_t pwm);

/**
 * @brief Sets the PWM value for the motors.
 * @param pwm The new PWM value to be set.
 */
void set_current_pwm(const int16_t pwm);

/**
 * @brief Sets the max PWM value for the motors.
 * @param pwm The new base PWM value to be set.
 * @note This function is used for gradually stopping the motors.
 */
void set_max_pwm(const uint16_t pwm);

#endif  // PID_H
