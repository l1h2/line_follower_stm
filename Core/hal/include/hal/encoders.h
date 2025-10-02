#ifndef HAL_ENCODER_H
#define HAL_ENCODER_H

#include <stdint.h>

#define WHEEL_DIAMETER_MM 22.0f     // Diameter of the wheel in mm
#define ENCODER_PULSES_PER_REV 168  // Number of pulses per wheel revolution

/**
 * @brief Initialize the encoder counters.
 */
void init_encoder_counters(void);

/**
 * @brief Set the left encoder counter to a specific value.
 * @param value The value to set the left encoder counter to.
 */
void set_encoder_left(const int16_t value);

/**
 * @brief Set the right encoder counter to a specific value.
 * @param value The value to set the right encoder counter to.
 */
void set_encoder_right(const int16_t value);

/**
 * @brief Set both encoder counters to specific values.
 * @param left The value to set the left encoder counter to.
 * @param right The value to set the right encoder counter to.
 */
void set_encoder_values(const int16_t left, const int16_t right);

/**
 * @brief Set both encoder counters to the same specific value.
 * @param value The value to set both encoder counters to.
 */
void set_encoders(const int16_t value);

/**
 * @brief Get the current value of the left encoder counter.
 * @return The current value of the left encoder counter.
 */
int16_t get_encoder_left(void);

/**
 * @brief Get the current value of the right encoder counter.
 * @return The current value of the right encoder counter.
 */
int16_t get_encoder_right(void);

/**
 * @brief Get the current values of both encoder counters.
 * @param left Pointer to store the left encoder value.
 * @param right Pointer to store the right encoder value.
 */
void get_encoder_values(int16_t *left, int16_t *right);

#endif  // HAL_ENCODER_H
