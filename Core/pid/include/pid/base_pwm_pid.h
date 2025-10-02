#ifndef BASE_PWM_PID_H
#define BASE_PWM_PID_H

#include <stdbool.h>
#include <stdint.h>

#include "pid_base.h"

/**
 * @brief Initialize the Base PWM PID controller with the given error structure.
 * @param error_struct Pointer to the ErrorStruct containing error information.
 */
void init_base_pwm_pid(const ErrorStruct* error_struct);

/**
 * @brief Calculate and return the Base PWM PID value.
 * @return The calculated Base PWM PID value.
 */
int16_t get_base_pwm_pid(void);

/**
 * @brief Get a pointer to the Base PID structure.
 * @return Pointer to the BasePwmPid structure.
 */
const BasePwmPid* get_base_pwm_pid_ptr(void);

/**
 * @brief Check if an update is pending for the Base PWM PID controller.
 * @return true if an update is pending, false otherwise.
 */
bool update_pending_base_pwm_pid(void);

/**
 * @brief Update the last PID time to the current time.
 */
void update_base_pwm_pid(void);

/**
 * @brief Set the proportional gain (Kp) for the Base PWM PID controller.
 * @param kp The new proportional gain value.
 */
void set_base_pwm_kp(const uint8_t kp);

/**
 * @brief Set the integral gain (Ki) for the Base PWM PID controller.
 * @param ki The new integral gain value.
 */
void set_base_pwm_ki(const uint8_t ki);

/**
 * @brief Set the derivative gain (Kd) for the Base PWM PID controller.
 * @param kd The new derivative gain value.
 */
void set_base_pwm_kd(const uint16_t kd);

/**
 * @brief Set the feedforward gain (Kff) for the Base PWM PID controller.
 * @param kff The new feedforward gain value.
 */
void set_base_pwm_kff(const uint8_t kff);

#endif  // BASE_PWM_PID_H
