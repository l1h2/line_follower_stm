#ifndef PURE_PURSUIT_H
#define PURE_PURSUIT_H

#include <stdbool.h>
#include <stdint.h>

#include "pid/pid_base.h"
#include "pure_pursuit/pure_pursuit_base.h"
#include "track/track_base.h"

/**
 * @brief Initializes the Pure Pursuit controller with the given track counters.
 *
 * @param track Pointer to the TrackCounters structure containing track
 * information.
 * @param pid Pointer to the PidStruct structure for PID control.
 * @return Pointer to the initialized PurePursuit structure.
 */
const PurePursuit* init_pure_pursuit(const TrackCounters* const track,
                                     const PidStruct* const pid);

/**
 * @brief Retrieves the current Pure Pursuit controller instance.
 * @return Pointer to the current PurePursuit structure.
 */
const PurePursuit* get_pure_pursuit(void);

/**
 * @brief Updates the peripheral sensors not used in the Pure Pursuit
 * controller.
 * @return true if the sensors were updated, false otherwise.
 */
bool update_peripheral_sensors(void);

/**
 * @brief Updates the Pure Pursuit controller.
 * @return true if the update was performed, false otherwise.
 */
bool update_pure_pursuit(void);

/**
 * @brief Restarts the Pure Pursuit controller, resetting its internal state.
 */
void restart_pure_pursuit(void);

/**
 * @brief Sets the lookahead distance for the Pure Pursuit controller.
 *
 * @param distance Lookahead distance in centimeters. Must be greater than
 * 0.
 */
void set_lookahead(const uint8_t distance);

#endif  // PURE_PURSUIT_H
