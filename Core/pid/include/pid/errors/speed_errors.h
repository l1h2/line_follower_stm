#ifndef SPEED_ERRORS_H
#define SPEED_ERRORS_H

#include "../pid_base.h"

/**
 * @brief Initialize the speed errors structure with the given error structure.
 * @param error_struct Pointer to an initialized ErrorStruct containing sensor
 * data.
 */
void init_speed_errors(const ErrorStruct* error_struct);

/**
 * @brief Get a pointer to the current SpeedErrors structure.
 * @return Pointer to the SpeedErrors structure.
 */
const SpeedErrors* get_speed_errors(void);

/**
 * @brief Update the speed errors based on the latest encoder data.
 * @note This function always reads encoder data, updating the speed intervals.
 */
void update_speed_errors(void);

/**
 * @brief Clear the speed errors by resetting their values.
 */
void clear_speed_errors(void);

#endif  // SPEED_ERRORS_H
