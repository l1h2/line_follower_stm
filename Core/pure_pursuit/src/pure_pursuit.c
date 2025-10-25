#include "pure_pursuit/pure_pursuit.h"

#include <stdlib.h>

#include "math/math.h"
#include "motors/motors.h"
#include "pid/controllers/speed_pid.h"
#include "pid/errors/speed_errors.h"
#include "sensors/encoder.h"
#include "sensors/sensors.h"
#include "timer/time.h"
#include "track/track.h"
#include "track/track_selector.h"

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

static uint32_t last_track_update = 0;
static bool is_updating_sensors = false;
static float inv_lookahead_sq =
    1.0f / ((float)LOOKAHEAD_CM * (float)LOOKAHEAD_CM);

static struct {
    float next_x;
    float next_y;
    float target_x;
    float target_y;
    float speed_left;
    float speed_right;
    uint16_t waypoint_index;
} pp_state = {0};

static inline bool out_of_range(void) {
    const float dx = waypoints_x[pp_state.waypoint_index] - pp.track->x;
    const float dy = waypoints_y[pp_state.waypoint_index] - pp.track->y;
    return (dx * dx + dy * dy) >= (pp.lookahead * pp.lookahead);
}

static inline void update_next_waypoint(void) {
    for (uint16_t i = 0; i < waypoint_count; i++) {
        if (out_of_range()) break;
        pp_state.waypoint_index =
            (pp_state.waypoint_index + 1) % waypoint_count;
    }

    pp_state.next_x = waypoints_x[pp_state.waypoint_index];
    pp_state.next_y = waypoints_y[pp_state.waypoint_index];
}

static inline void update_targets(void) {
    update_next_waypoint();

    const float dx = pp_state.next_x - pp.track->x;
    const float dy = pp_state.next_y - pp.track->y;

    const float ratio = pp.lookahead * fast_inv_sqrtf(dx * dx + dy * dy);
    pp_state.target_x = pp.track->x + dx * ratio;
    pp_state.target_y = pp.track->y + dy * ratio;
}

static inline void update_target_speeds(void) {
    update_targets();

    const float dx = pp_state.target_x - pp.track->x;
    const float dy = pp_state.target_y - pp.track->y;
    const float y_r = pp.track->cos_heading * dy - pp.track->sin_heading * dx;

    const float curvature =
        pp.pid->errors->sensors->encoders->effective_wheel_base * y_r *
        inv_lookahead_sq;

    pp_state.speed_left = pp.pid->speed_pid->base_speed * (1 - curvature);
    pp_state.speed_right = pp.pid->speed_pid->base_speed * (1 + curvature);
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

    update_base_speed_pid_time();
    update_encoder_data();
    update_positions();
    update_target_speeds();

    set_speed_targets(pp_state.speed_left, pp_state.speed_right);
    update_speed_errors();
    update_base_speed_pid();

    return true;
}

void restart_pure_pursuit(void) {
    pp_state = (typeof(pp_state)){0};
    is_updating_sensors = false;
}

void set_lookahead(const uint8_t distance) {
    pp.lookahead = distance;
    inv_lookahead_sq = 1.0f / ((float)distance * (float)distance);
}
