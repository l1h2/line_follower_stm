#ifndef ERRORS_H
#define ERRORS_H

#include <stdbool.h>
#include <stdint.h>

#include "../pid_base.h"

/**
 * @brief Initializes the error struct.
 * @param sensors Pointer to the SensorState struct containing sensor data.
 * @return Pointer to the initialized ErrorStruct.
 */
const ErrorStruct* init_errors(const SensorState* const sensors);

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
 * @param read_encoder Boolean flag to indicate if encoder data should be read.
 * @note Reading encoder data recalculates speeds based on the interval between
 * reads.
 */
void update_errors(const uint16_t timeout, const bool read_encoder);

/**
 * @brief Updates the error struct with the latest sensor data and error values.
 * @param read_encoder Boolean flag to indicate if encoder data should be read.
 * @return true if the errors were updated, false otherwise.
 * @note This function is non-blocking and will return false if the sensors are
 * still being read.
 * @note Reading encoder data recalculates speeds based on the interval between
 * reads.
 */
bool update_errors_async(const bool read_encoder);

/**
 * @brief Clears the error struct by resetting its values.
 */
void clear_errors(void);

#endif  // ERRORS_H
