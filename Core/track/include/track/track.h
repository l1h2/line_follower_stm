#ifndef TRACK_H
#define TRACK_H

#include <stdbool.h>

#include "pid/pid_base.h"
#include "track/track_base.h"

/**
 * @brief Initializes the track module.
 * @param error_struct Pointer to the ErrorStruct containing sensor and
 * error information.
 * @return Pointer to the initialized TrackCounters structure.
 */
const TrackCounters* init_track(const ErrorStruct* const error_struct);

/**
 * @brief Returns a pointer to the current TrackCounters structure.
 * @return Pointer to the TrackCounters containing current track information.
 */
const TrackCounters* get_track(void);

/**
 * @brief Resets the track information and counters.
 */
void reset_track(void);

/**
 * @brief Updates the track information based on sensor readings.
 * @param encoder_updated Boolean indicating if the encoder data was
 * updated.
 * @return true if the track information was updated, false otherwise.
 */
bool update_track(const bool encoder_updated);

/**
 * @brief Updates the robot's position and distance traveled.
 */
void update_positions(void);

/**
 * @brief Sets the IMU fusion alpha value for heading correction.
 * @param alpha The alpha value to set (between 0 and 1).
 */
void set_imu_alpha(const float alpha);

#endif  // TRACK_H
