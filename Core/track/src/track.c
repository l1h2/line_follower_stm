#include "track/track.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include "logger/logger.h"
#include "math/math.h"
#include "timer/time.h"
#include "track/observer.h"

#define MIN_ARC_ANGLE_RAD 0.3f
#define IMU_FUSION_ALPHA 1.0f
#define DETECTION_DEBOUNCE_TIME_MS 30
#define MARKER_COUNTER_THRESHOLD 12
#define CROSSING_COUNTER_THRESHOLD 1

static TrackCounters track = {0};

static const ErrorStruct* errors = NULL;
static uint32_t last_true_check_time = 0;
static float prev_mpu_yaw = 0.0f;
static bool heading_vec_initialized = false;

typedef enum { LINE, CROSSING, CURVE, MARKER } MemoryCounters;

static struct {
    uint8_t counter;
    MemoryCounters last;
} memory = {0, LINE};

static inline void reset_memory(void) {
    memory.counter = 0;
    memory.last = LINE;
}

static inline void reset_headings(void) {
    track.sin_heading = 0.0f;
    track.cos_heading = 1.0f;
    prev_mpu_yaw = 0.0f;
    heading_vec_initialized = false;
}

static inline void complete_lap(void) {
    track.laps++;

    track.line_counter = 0;
    track.crossings = 0;
    track.curve_markers = 0;
    track.track_markers = 0;
    track.section = 0;

    // TODO: Get exact distance for track markers
    // track.x = -60;
    // track.y = 0;

    // reset_headings();
    reset_memory();
}

static inline void start_lap(void) {
    track.track_markers = 1;
    track.section = 1;

    track.line_counter = 0;
    track.crossings = 0;
    track.curve_markers = 0;

    // track.x = 0;
    // track.y = 0;

    // reset_headings();
    reset_memory();
}

static inline void anchor_heading_vector(const float heading) {
    track.cos_heading = cosf(heading);
    track.sin_heading = sinf(heading);
    prev_mpu_yaw = heading;
    track.heading = heading;
    heading_vec_initialized = true;
}

// TODO: Add distance resets based on markers
static inline void update_distance(void) {
    track.distance += errors->sensors->encoders->current_distance;
    if (track.distance < 0) track.distance = 0;
}

static inline float get_delta_angle(void) {
    const float enc_angle = errors->sensors->encoders->current_angle;
    const float current_yaw = errors->sensors->mpu_data->yaw;

    float delta_yaw = current_yaw - prev_mpu_yaw;
    normalize_angle(&delta_yaw);
    prev_mpu_yaw = current_yaw;

    return track.imu_alpha * delta_yaw + (1.0f - track.imu_alpha) * enc_angle;
}

// TODO: Add XY resets based on markers
static inline void update_position(void) {
    const float dist = errors->sensors->encoders->current_distance;
    const float angle = get_delta_angle();
    const float half_step = 0.5f * angle;

    if (!heading_vec_initialized) {
        anchor_heading_vector(errors->sensors->mpu_data->yaw - angle);
    }

    float s_half, c_half;
    if (fabsf(half_step) <= MIN_ARC_ANGLE_RAD) {
        sincos_poly_truncation(half_step, &s_half, &c_half);
    } else {
        s_half = sinf(half_step);
        c_half = cosf(half_step);
    }

    const float sin_h = track.sin_heading;
    float cos_h = track.cos_heading;

    const float cos_mid = cos_h * c_half - sin_h * s_half;
    const float sin_mid = sin_h * c_half + cos_h * s_half;

    track.x += dist * cos_mid;
    track.y += dist * sin_mid;

    const float s_full = 2.0f * s_half * c_half;
    const float c_full = c_half * c_half - s_half * s_half;

    track.cos_heading = cos_h * c_full - sin_h * s_full;
    track.sin_heading = sin_h * c_full + cos_h * s_full;
    normalize_unit_vector(&track.cos_heading, &track.sin_heading);

    track.heading += angle;
    normalize_angle(&track.heading);
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
    debug_print("Crossing");
}

static inline void update_section_for_markers(void) {
    if (track.track_markers == 1) {
        start_lap();
    } else {
        complete_lap();
    }
    debug_print("Marker");
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
    track.imu_alpha = IMU_FUSION_ALPHA;

    return &track;
}

const TrackCounters* get_track(void) { return &track; }

void reset_track(void) {
    const float imu_alpha = track.imu_alpha;
    track = (TrackCounters){0};
    track.imu_alpha = imu_alpha;

    reset_headings();
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

void set_imu_alpha(const float alpha) { track.imu_alpha = alpha; }
