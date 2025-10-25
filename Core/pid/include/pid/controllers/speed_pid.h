#ifndef SPEED_PID_H
#define SPEED_PID_H

#include "../pid_base.h"

/**
 * @brief Initialize the speed PID controller with the given error structure.
 * @param error_struct Pointer to the ErrorStruct containing speed error values.
 * @return Pointer to the initialized BaseSpeedPid structure.
 */
const BaseSpeedPid* init_base_speed_pid(const ErrorStruct* const error_struct);

/**
 * @brief Get a pointer to the BaseSpeedPid structure.
 * @return Pointer to the BaseSpeedPid structure.
 */
const BaseSpeedPid* get_base_speed_pid_ptr(void);

/**
 * @brief Update the speed PID controller and set motor PWMs.
 */
void update_base_speed_pid(void);

/**
 * @brief Check if an update is pending for the speed PID controller.
 * @return true if an update is pending, false otherwise.
 */
bool update_pending_base_speed_pid(void);

/**
 * @brief Update the last PID update time to the current time.
 */
void update_base_speed_pid_time(void);

/**
 * @brief Set the proportional gain (Kp) for the speed PID controller.
 * @param kp The new proportional gain value.
 */
void set_base_speed_kp(const uint16_t kp);

/**
 * @brief Set the integral gain (Ki) for the speed PID controller.
 * @param ki The new integral gain value.
 */
void set_base_speed_ki(const float ki);

/**
 * @brief Set the derivative gain (Kd) for the speed PID controller.
 * @param kd The new derivative gain value.
 */
void set_base_speed_kd(const uint16_t kd);

/**
 * @brief Set the feedforward gain (Kff) for the speed PID controller.
 * @param kff The new feedforward gain value.
 */
void set_base_speed_kff(const uint16_t kff);

/**
 * @brief Set the base speed for the speed PID controller.
 * @param speed The new base speed in cm/s.
 */
void set_base_speed(const float speed);

#endif  // SPEED_PID_H
