#ifndef ENCODER_H
#define ENCODER_H

#include <stdbool.h>
#include <stdint.h>

#include "sensors/sensors_base.h"

/**
 * @brief Initializes the encoder peripherals.
 */
void init_encoder(void);

/**
 * @brief Gets the current encoder data.
 * @return Pointer to the encoder data structure.
 */
const EncoderData* get_encoder_data(void);

/**
 * @brief Restarts the encoder counters by setting them to zero.
 */
void restart_encoders(void);

/**
 * @brief Updates the encoder data.
 * @note Reading encoder data recalculates speeds based on the interval
 * between reads.
 */
void update_encoder_data(void);

/**
 * @brief Asynchronously updates the encoder data if the specified interval
 * has elapsed since the last update.
 * @param interval_ms The minimum interval in milliseconds between updates.
 * @return true if the encoder data was updated, false otherwise.
 */
bool update_encoder_data_async(const uint32_t interval_ms);

/**
 * @brief Clears the encoder data.
 */
void clear_encoder_data(void);

/**
 * @brief Starts the encoders by restarting and updating their data.
 * @note This function should be called before starting operations that rely on
 * encoder data.
 */
void start_encoders(void);

#endif  // ENCODER_H
