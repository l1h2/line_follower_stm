#ifndef PURE_PURSUIT_BASE_H
#define PURE_PURSUIT_BASE_H

#include <stdint.h>

#include "pid/pid_base.h"
#include "track/track_base.h"

/**
 * @struct PurePursuit
 * @brief Structure to hold Pure Pursuit controller parameters and state.
 */
typedef struct {
    uint8_t lookahead;           // Lookahead distance in cm
    uint32_t frame_interval;     // Frame interval in ms
    uint32_t last_pp_time;       // Last update time in ms
    const TrackCounters* track;  // Pointer to the track counters
    const PidStruct* pid;        // Pointer to the PID controller
} PurePursuit;

#endif  // PURE_PURSUIT_BASE_H
