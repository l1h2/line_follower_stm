#include "track/position_correction.h"

#include <float.h>
#include <stddef.h>
#include <stdint.h>

#include "sensors/sensors.h"
#include "track/observer.h"
#include "track/track_selector.h"

#define SENSOR_BAR_OFFSET_CM 9.7f  // Rotation center to sensor bar, forward
#define SENSOR_PITCH_CM 0.8f       // Spacing between adjacent central sensors

#define SEARCH_PATIENCE 4       // Segments to check for minimum before stopping
#define SEARCH_BACKOFF_CM 4.0f  // How far back a later sensor may still match
#define CORRECTION_RATE 0.01f   // Weight of the correction applied per call

// The square of the backoff distance, to avoid repeated square calls.
#define SEARCH_BACKOFF_CM_SQUARED (SEARCH_BACKOFF_CM * SEARCH_BACKOFF_CM)

static const TrackCounters* track = NULL;
static const SensorState* sensors = NULL;

static uint16_t waypoint_index;

static inline uint16_t prev_index(const uint16_t index) {
    return index ? index - 1 : WAYPOINT_COUNT - 1;
}

static inline uint16_t next_index(const uint16_t index) {
    return index + 1 < WAYPOINT_COUNT ? index + 1 : 0;
}

static inline float get_distance_squared(const uint16_t index) {
    const float dx = waypoints_x[index] - track->x;
    const float dy = waypoints_y[index] - track->y;
    return dx * dx + dy * dy;
}

static inline void update_waypoint_index(const uint16_t lookahead_index) {
    const float search_radius_squared = get_distance_squared(lookahead_index);
    uint16_t index = prev_index(lookahead_index);

    while (index != waypoint_index) {
        if (get_distance_squared(index) >= search_radius_squared) break;
        index = prev_index(index);
    }

    waypoint_index = index;
}

static inline void get_sensor_position(const uint8_t sensor_index, float* x,
                                       float* y) {
    const float cos_heading = track->cos_heading;
    const float sin_heading = track->sin_heading;

    const float y_offset =
        ((sensors->ir_sensors->total_central_sensors - 1) * 0.5f -
         sensor_index) *
        SENSOR_PITCH_CM;

    *x = track->x + SENSOR_BAR_OFFSET_CM * cos_heading - y_offset * sin_heading;
    *y = track->y + SENSOR_BAR_OFFSET_CM * sin_heading + y_offset * cos_heading;
}

static inline void get_point_candidate(float* x, float* y, const uint16_t index,
                                       const uint16_t next,
                                       const float sensor_x,
                                       const float sensor_y) {
    const float x1 = waypoints_x[index];
    const float y1 = waypoints_y[index];
    const float x2 = waypoints_x[next];
    const float y2 = waypoints_y[next];

    const float vx = x2 - x1;
    const float vy = y2 - y1;

    const float projection = (sensor_x - x1) * vx + (sensor_y - y1) * vy;
    const float length_squared = vx * vx + vy * vy;

    if (projection <= 0.0f) {
        *x = x1;
        *y = y1;
    } else if (projection >= length_squared) {
        *x = x2;
        *y = y2;
    } else {
        const float ratio = projection / length_squared;
        *x = x1 + ratio * vx;
        *y = y1 + ratio * vy;
    }
}

static inline uint16_t backoff_index(const uint16_t index) {
    if (index == waypoint_index) return index;

    const float x = waypoints_x[index];
    const float y = waypoints_y[index];
    uint16_t result = index;

    while (result != waypoint_index) {
        const uint16_t previous = prev_index(result);
        const float dx = waypoints_x[previous] - x;
        const float dy = waypoints_y[previous] - y;

        if (dx * dx + dy * dy >= SEARCH_BACKOFF_CM_SQUARED) break;

        result = previous;
    }

    return result;
}

static inline void get_nearest_line_point(float* nearest_x, float* nearest_y,
                                          uint16_t* sensor_waypoint_index,
                                          const float sensor_x,
                                          const float sensor_y) {
    uint16_t index = backoff_index(*sensor_waypoint_index);
    float best = FLT_MAX;
    uint8_t misses = 0;

    *nearest_x = sensor_x;
    *nearest_y = sensor_y;

    for (uint16_t i = 0; i < WAYPOINT_COUNT; i++) {
        const uint16_t next = next_index(index);

        float point_x, point_y;
        get_point_candidate(&point_x, &point_y, index, next, sensor_x,
                            sensor_y);

        const float dx = sensor_x - point_x;
        const float dy = sensor_y - point_y;
        const float distance_squared = dx * dx + dy * dy;

        if (distance_squared < best) {
            best = distance_squared;
            *nearest_x = point_x;
            *nearest_y = point_y;
            *sensor_waypoint_index = index;
            misses = 0;
        } else if (++misses >= SEARCH_PATIENCE) {
            break;
        }

        index = next;
    }
}

static inline void get_lateral_correction(float* lateral_x, float* lateral_y,
                                          const uint16_t index, const float dx,
                                          const float dy) {
    const uint16_t next = next_index(index);

    const float vx = waypoints_x[next] - waypoints_x[index];
    const float vy = waypoints_y[next] - waypoints_y[index];
    const float length_squared = vx * vx + vy * vy;

    if (length_squared <= 0.0f) {
        *lateral_x = 0.0f;
        *lateral_y = 0.0f;
        return;
    }

    const float scale = (dy * vx - dx * vy) / length_squared;

    *lateral_x = -scale * vy;
    *lateral_y = scale * vx;
}

static inline void compute_correction(float* x_corr, float* y_corr) {
    const uint8_t central_sensors = sensors->ir_sensors->central_sensors_state;
    const uint8_t total_sensors = sensors->ir_sensors->total_central_sensors;
    uint16_t sensor_waypoint_index = waypoint_index;

    for (uint8_t i = 0; i < total_sensors; i++) {
        if (!(central_sensors & (1 << i))) continue;

        float sensor_x, sensor_y;
        get_sensor_position(i, &sensor_x, &sensor_y);

        float nearest_x, nearest_y;
        get_nearest_line_point(&nearest_x, &nearest_y, &sensor_waypoint_index,
                               sensor_x, sensor_y);

        float lateral_x, lateral_y;
        get_lateral_correction(&lateral_x, &lateral_y, sensor_waypoint_index,
                               nearest_x - sensor_x, nearest_y - sensor_y);

        *x_corr += lateral_x;
        *y_corr += lateral_y;
    }

    *x_corr *= CORRECTION_RATE;
    *y_corr *= CORRECTION_RATE;
}

void init_position_correction(const TrackCounters* t) {
    track = t;
    sensors = get_sensors();
}

void get_correction(float* x_corr, float* y_corr,
                    const uint16_t lookahead_index) {
    if (!check_correctable_line()) return;

    update_waypoint_index(lookahead_index);
    compute_correction(x_corr, y_corr);
}

void reset_position_correction(void) { waypoint_index = 0; }
