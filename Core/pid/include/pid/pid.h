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
 * @brief Updates the speed PID controller with the current speed error
 * values.
 * @return true if the speed PID controller was updated, false otherwise.
 */
bool update_speed_pid(void);

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

/**
 * @brief Sets the acceleration step for PWM changes.
 * @param accel The new acceleration step value to be set.
 */
void set_pwm_accel(const uint16_t accel);

/**
 * @brief Set the proportional gain (Kp) value for the Delta PWM PID controller.
 * @param kp The new proportional gain value to be set.
 */
void set_pwm_kp(const uint8_t kp);

/**
 * @brief Set the integral gain (Ki) value for the Delta PWM PID controller.
 * @param ki The new integral gain value to be set.
 */
void set_pwm_ki(const uint8_t ki);

/**
 * @brief Set the derivative gain (Kd) value for the Delta PWM PID controller.
 * @param kd The new derivative gain value to be set.
 */
void set_pwm_kd(const uint16_t kd);

/**
 * @brief Set the base proportional gain (Kb) value for the Base PWM PID
 * controller.
 * @param kb The new base proportional gain value to be set.
 */
void set_pwm_kb(const uint8_t kb);

/**
 * @brief Set the feedforward gain (Kff) value for the Base PWM PID controller.
 * @param kff The new feedforward gain value to be set.
 */
void set_pwm_kff(const uint8_t kff);

#endif  // PID_H
