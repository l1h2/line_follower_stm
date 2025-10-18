#ifndef PID_H
#define PID_H

#include <stdbool.h>
#include <stdint.h>

#include "pid_base.h"

/**
 * @brief Initializes the PID controller and all related registers.
 * @param sensors Pointer to the SensorState structure containing sensor data.
 * @return Pointer to the initialized PidStruct structure.
 */
const PidStruct* init_pid(const SensorState* const sensors);

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
 * @brief Restarts the current PID used to the initial PID values.
 */
void restart_pid(void);

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
 * @brief Set the filter coefficient (alpha) for the Delta PWM PID controller.
 * @param alpha The new filter coefficient value to be set.
 */
void set_pwm_alpha(const float alpha);

/**
 * @brief Set the integral windup clamp value for the Delta PWM PID
 * controller.
 * @param clamp The new integral windup clamp value to be set.
 */
void set_pwm_clamp(const uint16_t clamp);

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

/**
 * @brief Set the proportional gain (Kp) value for the Speed PID controller.
 * @param kp The new proportional gain value to be set.
 */
void set_speed_kp(const uint16_t kp);

/**
 * @brief Set the integral gain (Ki) value for the Speed PID controller.
 * @param ki The new integral gain value to be set.
 */
void set_speed_ki(const float ki);

/**
 * @brief Set the derivative gain (Kd) value for the Speed PID controller.
 * @param kd The new derivative gain value to be set.
 */
void set_speed_kd(const uint16_t kd);

/**
 * @brief Set the feedforward gain (Kff) for the speed PID controller.
 * @param kff The new feedforward gain value.
 */
void set_speed_kff(const uint16_t kff);

/**
 * @brief Set the target speed for the Speed PID controller.
 * @param speed The new target speed value to be set (in cm/s).
 */
void set_speed(const float speed);

#endif  // PID_H
