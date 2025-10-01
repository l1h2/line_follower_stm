#ifndef ERRORS_H
#define ERRORS_H

#include <stdbool.h>

#include "pid_base.h"

/**
 * @brief Initializes the error struct.
 */
void init_errors(void);

/**
 * @brief Returns a pointer to the error struct.
 *
 * @return Pointer to the ErrorStruct containing error values.
 */
const ErrorStruct* get_errors(void);

/**
 * @brief Updates the error struct with the latest sensor data and error values.
 * @param timeout The maximum time to wait for sensor readings (in
 * microseconds).
 */
void update_errors(const uint16_t timeout);

/**
 * @brief Updates the error struct with the latest sensor data and error values.
 * @return true if the errors were updated, false otherwise.
 * @note This function is non-blocking and will return false if the sensors are
 * still being read.
 */
bool update_errors_async(void);

/**
 * @brief Clears the error struct by resetting its values.
 */
void clear_errors(void);

#endif  // ERRORS_H
