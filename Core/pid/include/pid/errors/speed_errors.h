#ifndef SPEED_ERRORS_H
#define SPEED_ERRORS_H

#include "../pid_base.h"

/**
 * @brief Initialize the speed errors structure with the given error structure.
 * @param error_struct Pointer to an initialized ErrorStruct containing sensor
 * data.
 * @return Pointer to the initialized SpeedErrors structure.
 */
const SpeedErrors* init_speed_errors(const ErrorStruct* const error_struct);

/**
 * @brief Get a pointer to the current SpeedErrors structure.
 * @return Pointer to the SpeedErrors structure.
 */
const SpeedErrors* get_speed_errors(void);

/**
 * @brief Update the speed errors based on the latest encoder data.
 * @note This function needs encoder values to be updated before calling.
 */
void update_speed_errors(void);

/**
 * @brief Clear the speed errors by resetting their values.
 */
void clear_speed_errors(void);

/**
 * @brief Set the target speeds for the left and right motors.
 * @param left_speed Target speed for the left motor in cm/s.
 * @param right_speed Target speed for the right motor in cm/s.
 */
void set_speed_targets(const float left_speed, const float right_speed);

#endif  // SPEED_ERRORS_H
