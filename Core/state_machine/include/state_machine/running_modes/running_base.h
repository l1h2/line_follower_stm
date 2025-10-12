#ifndef RUNNING_BASE_H
#define RUNNING_BASE_H

#include <stdbool.h>

#include "track/track_base.h"

/**
 * @brief Initializes the running modes.
 * @param track_counters Pointer to the TrackCounters structure.
 */
void init_running_modes(const TrackCounters* const track_counters);

/**
 * @brief Checks if the robot should stop based on the configured stop mode.
 * @param track_updated Indicates if the track counters were updated.
 */
void check_stop(const bool track_updated);

/**
 * @brief Starts the turbine if it is set to run.
 */
void start_turbine_if_needed(void);

/**
 * @brief Stops the turbine if it is set to run.
 */
void stop_turbine_if_needed(void);

#endif  // RUNNING_BASE_H
