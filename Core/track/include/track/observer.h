#ifndef OBSERVER_H
#define OBSERVER_H

#include <stdbool.h>

#include "pid/pid_base.h"
#include "track/track_base.h"

/**
 * @brief Initializes the observer module.
 * @param error_struct Pointer to the ErrorStruct containing sensor and
 * error information.
 */
void init_observer(const ErrorStruct* const error_struct);

/**
 * @brief Checks if the line is detected by any of the central sensors.
 * @return LostType indicating the type of line loss (NONE, LEFT, RIGHT, PITCH).
 */
LostType check_line(void);

/**
 * @brief Checks if the active sensors are non-contiguous.
 * @return true if non-contiguous sensors are detected, false otherwise.
 */
bool check_non_contiguous_sensors(void);

/**
 * @brief Checks if a crossing is detected based on the number of active
 * central sensors.
 * @return true if a crossing is detected, false otherwise.
 */
bool check_crossing(void);

/**
 * @brief Checks if a curve marker is detected based on side sensor states and
 * error threshold.
 * @return true if a curve marker is detected, false otherwise.
 */
bool check_curve_marker(void);

/**
 * @brief Checks if a track marker is detected based on side sensor states and
 * error threshold.
 * @return true if a track marker is detected, false otherwise.
 */
bool check_track_marker(void);

#endif  // OBSERVER_H
