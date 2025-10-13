#include "track/track.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include "timer/time.h"
#include "track/observer.h"

#define MIN_ARC_ANGLE_RAD 1e-6f
#define DETECTION_DEBOUNCE_TIME_MS 30
#define MARKER_COUNTER_THRESHOLD 6
#define CROSSING_COUNTER_THRESHOLD 1

static TrackCounters track = {0};

static const ErrorStruct* errors = NULL;
static uint32_t last_true_check_time = 0;

typedef enum { LINE, CROSSING, CURVE, MARKER } MemoryCounters;

static struct {
    uint8_t counter;
    MemoryCounters last;
} memory = {0, LINE};

static inline void reset_memory(void) {
    memory.counter = 0;
    memory.last = LINE;
}

static inline void complete_lap(void) {
    track.laps++;

    track.line_counter = 0;
    track.crossings = 0;
    track.curve_markers = 0;
    track.track_markers = 0;
    track.section = 0;

    // TODO: Get exact distance for track markers
    track.x = -60;
    track.y = 0;

    reset_memory();
}

static inline void start_lap(void) {
    track.track_markers = 1;
    track.section = 1;

    track.line_counter = 0;
    track.crossings = 0;
    track.curve_markers = 0;

    track.x = 0;
    track.y = 0;

    reset_memory();
}

// TODO: Add distance resets based on markers
static inline void update_distance(void) {
    track.distance += errors->sensors->encoders->current_distance;
    if (track.distance < 0) track.distance = 0;
}

// TODO: Add XY resets based on markers
static inline void update_position(void) {
    const float dist = errors->sensors->encoders->current_distance;
    const float angle = errors->sensors->encoders->current_angle;
    const float heading = errors->sensors->encoders->heading;

    if (fabsf(angle) <= MIN_ARC_ANGLE_RAD) {
        const float mid_heading = heading - (angle / 2.0f);
        track.x += dist * cosf(mid_heading);
        track.y += dist * sinf(mid_heading);
        return;
    }

    const float radius = dist / angle;
    const float dx_body = radius * sinf(angle);
    const float dy_body = radius * (1.0f - cosf(angle));

    const float prev_heading = heading - angle;
    const float cos_heading = cosf(prev_heading);
    const float sin_heading = sinf(prev_heading);
    track.x += dx_body * cos_heading - dy_body * sin_heading;
    track.y += dx_body * sin_heading + dy_body * cos_heading;
}

static inline void update_line(void) {
    switch (check_line()) {
        case NONE:
            track.line_counter++;
            break;
        case LEFT:
            track.lost_left_counter++;
            break;
        case RIGHT:
            track.lost_right_counter++;
            break;
        case PITCH:
            track.lost_pitch_counter++;
            break;
    }
}

static bool check_memory(const MemoryCounters counter_type) {
    if (counter_type != memory.last) {
        memory.last = counter_type;
        memory.counter = 1;
        return false;
    }

    memory.counter++;

    if (counter_type == CROSSING &&
        memory.counter < CROSSING_COUNTER_THRESHOLD) {
        return false;
    }
    if (memory.counter < MARKER_COUNTER_THRESHOLD) return false;

    reset_memory();
    return true;
}

static inline void update_section_for_crossings(void) {
    if (track.section == 0) return;

    track.section++;
}

static inline void update_section_for_markers(void) {
    if (track.track_markers == 1) {
        start_lap();
    } else {
        complete_lap();
    }
}

static inline bool process_event(const MemoryCounters event) {
    if (!check_memory(event)) return false;

    last_true_check_time = time();

    switch (event) {
        case CROSSING:
            track.crossings++;
            update_section_for_crossings();
            break;
        case CURVE:
            track.curve_markers++;
            break;
        case MARKER:
            track.track_markers++;
            update_section_for_markers();
            break;
        default:
            return false;
    }

    return true;
}

static inline bool update_track_counters(void) {
    if (!time_elapsed(last_true_check_time, DETECTION_DEBOUNCE_TIME_MS)) {
        return false;
    }

    if (check_non_contiguous_sensors()) return false;

    if (check_crossing() && process_event(CROSSING)) return true;
    if (check_curve_marker() && process_event(CURVE)) return true;
    if (check_track_marker() && process_event(MARKER)) return true;

    return false;
}

const TrackCounters* init_track(const ErrorStruct* const error_struct) {
    init_observer(error_struct);
    errors = error_struct;

    return &track;
}

const TrackCounters* get_track(void) { return &track; }

void reset_track(void) {
    track = (TrackCounters){0};
    reset_memory();
}

bool update_track(const bool encoder_updated) {
    if (encoder_updated) {
        update_distance();
        update_position();
    }

    update_line();
    return update_track_counters();
}

void update_positions(void) {
    update_distance();
    update_position();
}
