#include "track/observer.h"

#include <stdint.h>
#include <stdlib.h>

// Minimum number of sensors for crossing detection
#define CROSSING_SENSORS_THRESHOLD 4

// Threshold for max error possible in a side marker detection
#define SIDE_MARKERS_ERROR_THRESHOLD 4

static const ErrorStruct* errors = NULL;
static uint8_t total_sensors = 0;

void init_observer(const ErrorStruct* const error_struct) {
    errors = error_struct;
    total_sensors = errors->sensors->ir_sensors->total_central_sensors;
}

LostType check_line(void) {
    const uint8_t central_sensors =
        errors->sensors->ir_sensors->central_sensors_state;

    if (central_sensors != 0) return NONE;
    if (errors->error <= errors->min_error) return LEFT;
    if (errors->error >= errors->max_error) return RIGHT;
    return PITCH;
}

bool check_non_contiguous_sensors(void) {
    bool found_active = false;
    bool found_gap = false;

    const uint8_t central_sensors =
        errors->sensors->ir_sensors->central_sensors_state;

    for (uint8_t i = 0; i < total_sensors; i++) {
        if (central_sensors & (1 << i)) {
            if (found_gap) return true;
            found_active = true;
        } else if (found_active) {
            found_gap = true;  // Gap found after active sensors
        }
    }

    return false;
}

bool check_crossing(void) {
    const uint8_t central_sensors =
        errors->sensors->ir_sensors->central_sensors_state;

    uint8_t active_count = 0;
    for (uint8_t i = 0; i < total_sensors; i++) {
        if (central_sensors & (1 << i)) {
            active_count++;
        }
    }

    return active_count >= CROSSING_SENSORS_THRESHOLD;
}

bool check_curve_marker(void) {
    if (!errors->sensors->ir_sensors->left_sensor ||
        errors->sensors->ir_sensors->right_sensor ||
        abs(errors->error) >= SIDE_MARKERS_ERROR_THRESHOLD) {
        return false;
    }

    return true;
}

bool check_track_marker(void) {
    if (!errors->sensors->ir_sensors->right_sensor ||
        errors->sensors->ir_sensors->left_sensor ||
        abs(errors->error) >= SIDE_MARKERS_ERROR_THRESHOLD) {
        return false;
    }

    return true;
}
