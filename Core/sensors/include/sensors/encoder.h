#ifndef ENCODER_H
#define ENCODER_H

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
 */
void update_encoder_data(void);

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
