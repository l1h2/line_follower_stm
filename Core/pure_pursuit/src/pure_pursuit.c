#include "pure_pursuit/pure_pursuit.h"

#include <stdlib.h>

#include "logger/logger.h"
#include "math/math.h"
#include "motors/motors.h"
#include "pid/controllers/speed_pid.h"
#include "pid/errors/speed_errors.h"
#include "sensors/encoder.h"
#include "sensors/sensors.h"
#include "timer/time.h"
#include "track/track.h"

#define LOOKAHEAD_CM 5
#define FRAME_INTERVAL_MS 10UL       // ms
#define SENSORS_UPDATE_INTERVAL 1UL  // ms

static PurePursuit pp = {
    .lookahead = LOOKAHEAD_CM,
    .frame_interval = FRAME_INTERVAL_MS,
    .last_pp_time = 0,
    .track = NULL,
    .pid = NULL,
};

static float next_x = 0.0f;
static float next_y = 0.0f;
static float target_x = 0.0f;
static float target_y = 0.0f;
static float speed_left = 0.0f;
static float speed_right = 0.0f;
static float inv_lookahead_sq =
    1.0f / ((float)LOOKAHEAD_CM * (float)LOOKAHEAD_CM);

static uint32_t last_track_update = 0;
static bool is_updating_sensors = false;

#define WAYPOINT_COUNT 4
#define TOTAL_DISTANCE 200.0f
static const float waypoints_x[WAYPOINT_COUNT] = {0, 50, 50, 0};
static const float waypoints_y[WAYPOINT_COUNT] = {0, 0, 50, 50};
static const float distances[WAYPOINT_COUNT] = {0, 50, 100, 150};
static uint8_t current_waypoint_index = 0;

static inline void update_next_waypoint(void) {
    if (pp.track->distance + pp.lookahead >= TOTAL_DISTANCE) {
        current_waypoint_index = 0;
        reset_track();
    }

    while (current_waypoint_index < WAYPOINT_COUNT - 1 &&
           pp.track->distance + pp.lookahead >=
               distances[current_waypoint_index]) {
        current_waypoint_index++;
    }

    next_x = waypoints_x[current_waypoint_index];
    next_y = waypoints_y[current_waypoint_index];
}

static inline void update_targets(void) {
    update_next_waypoint();

    const float dx = next_x - pp.track->x;
    const float dy = next_y - pp.track->y;

    const float ratio = pp.lookahead * fast_inv_sqrtf(dx * dx + dy * dy);
    target_x = pp.track->x + dx * ratio;
    target_y = pp.track->y + dy * ratio;
}

static inline void update_target_speeds(void) {
    const float dx = target_x - pp.track->x;
    const float dy = target_y - pp.track->y;
    const float y_r = pp.track->cos_heading * dy - pp.track->sin_heading * dx;

    const float curvature = WHEEL_BASE_CM * y_r * inv_lookahead_sq;

    speed_left = pp.pid->speed_pid->base_speed * (1 - curvature);
    speed_right = pp.pid->speed_pid->base_speed * (1 + curvature);
}

static inline bool check_sensor_update(void) {
    if (!is_updating_sensors) {
        start_async_sensors_read();
        is_updating_sensors = true;
        return false;
    }

    if (!update_sensors_async(false)) return false;

    is_updating_sensors = false;
    stop_async_sensors_read();
    return true;
}

const PurePursuit* init_pure_pursuit(const TrackCounters* const track,
                                     const PidStruct* const pid) {
    pp.track = track;
    pp.pid = pid;
    return &pp;
}

const PurePursuit* get_pure_pursuit(void) { return &pp; }

bool update_peripheral_sensors(void) {
    if (!time_elapsed(last_track_update, SENSORS_UPDATE_INTERVAL)) return false;
    if (!check_sensor_update()) return false;

    last_track_update = time();
    return true;
}

bool update_pure_pursuit(void) {
    if (!update_pending_base_speed_pid()) return false;

    update_base_speed_pid();
    update_encoder_data();
    update_positions();
    update_targets();
    update_target_speeds();

    set_speed_targets(speed_left, speed_right);
    update_speed_errors();

    int16_t left_pwm;
    int16_t right_pwm;
    get_base_speed_pid(&left_pwm, &right_pwm);
    set_motors(left_pwm, right_pwm);

    return true;
}

void restart_pure_pursuit(void) {
    pp.last_pp_time = 0;
    last_track_update = 0;
    is_updating_sensors = false;
    current_waypoint_index = 0;
}

void set_lookahead(const uint8_t distance) {
    pp.lookahead = distance;
    inv_lookahead_sq = 1.0f / ((float)distance * (float)distance);
}
