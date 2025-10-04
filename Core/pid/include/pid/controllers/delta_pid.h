#ifndef DELTA_PID_H
#define DELTA_PID_H

#include <stdbool.h>
#include <stdint.h>

#include "../pid_base.h"

/**
 * @brief Initialize the Delta PWM PID controller with the given error
 * structure.
 * @param error_struct Pointer to the ErrorStruct containing error information.
 */
void init_delta_pwm_pid(const ErrorStruct* error_struct);

/**
 * @brief Get a pointer to the Delta PID structure.
 * @return Pointer to the DeltaPid structure.
 */
const DeltaPid* get_delta_pwm_pid_ptr(void);

/**
 * @brief Calculate and return the Delta PWM PID value.
 * @return The calculated Delta PWM PID value.
 */
int16_t get_delta_pwm_pid(void);

/**
 * @brief Check if an update is pending for the Delta PWM PID controller.
 * @return true if an update is pending, false otherwise.
 */
bool update_pending_delta_pwm_pid(void);

/**
 * @brief Update the last PID time to the current time.
 */
void update_delta_pwm_pid(void);

/**
 * @brief Set the proportional gain (Kp) for the Delta PWM PID controller.
 * @param kp The new proportional gain value.
 */
void set_delta_pwm_kp(const uint8_t kp);

/**
 * @brief Set the integral gain (Ki) for the Delta PWM PID controller.
 * @param ki The new integral gain value.
 */
void set_delta_pwm_ki(const uint8_t ki);

/**
 * @brief Set the derivative gain (Kd) for the Delta PWM PID controller.
 * @param kd The new derivative gain value.
 */
void set_delta_pwm_kd(const uint16_t kd);

/**
 * @brief Set the filter coefficient (alpha) for the Delta PWM PID controller.
 * @param alpha The new filter coefficient value.
 */
void set_delta_pwm_alpha(const float alpha);

/**
 * @brief Set the integral windup clamp for the Delta PWM PID controller.
 * @param clamp The new integral windup clamp value.
 */
void set_delta_pwm_clamp(const uint16_t clamp);

#endif  // DELTA_PID_H
