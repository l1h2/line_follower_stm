#ifndef POSITION_CORRECTION_H
#define POSITION_CORRECTION_H

#include <stdint.h>

#include "track/track_base.h"

/**
 * @brief Initializes the position correction module.
 * @param track_data Pointer to the TrackCounters.
 * @note Must run after the sensors are initialized, since the sensor state is
 * looked up here.
 */
void init_position_correction(const TrackCounters* const track_data);

/**
 * @brief Clears the map cursor so a new run starts from the beginning.
 * @note The cursor only ever moves forward, so without this a new run inherits
 * the previous one's position and cannot come back to the start of the map.
 */
void reset_position_correction(void);

/**
 * @brief Computes the position correction for the current line observation.
 * @param x_corr Receives the X correction in cm.
 * @param y_corr Receives the Y correction in cm.
 * @param lookahead_index Waypoint pure pursuit is currently steering towards.
 * @note Corrects lateral displacement only. Position along the line is not
 * observable from the line sensors, and heading is left to the IMU.
 * @note The cursor is derived from the lookahead waypoint on every call, so
 * skipping calls costs nothing beyond the steps that were skipped.
 */
void get_correction(float* x_corr, float* y_corr,
                    const uint16_t lookahead_index);

#endif  // POSITION_CORRECTION_H
