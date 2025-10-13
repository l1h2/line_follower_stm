#include "pure_pursuit/pure_pursuit.h"

#include <math.h>
#include <stdlib.h>

#include "motors/motors.h"
#include "pid/controllers/speed_pid.h"
#include "pid/errors/speed_errors.h"
#include "sensors/encoder.h"
#include "sensors/sensors.h"
#include "timer/time.h"
#include "track/track.h"

#define LOOKAHEAD_CM 5.0f
#define FRAME_INTERVAL_MS 10         // ms
#define SENSORS_UPDATE_INTERVAL 1UL  // ms

static PurePursuit pp = {
    .lookahead = LOOKAHEAD_CM,
    .frame_interval = FRAME_INTERVAL_MS,
    .last_pp_time = 0,
    .track = NULL,
    .pid = NULL,
};

static float target_x = 0.0f;
static float target_y = 0.0f;
static float speed_left = 0.0f;
static float speed_right = 0.0f;

static uint32_t last_track_update = 0;
static bool is_updating_sensors = false;

static inline void update_targets(void) {
    // TODO: Implement target point calculation based on lookahead
}

static inline float get_alpha(void) {
    const float dx = target_x - pp.track->x;
    const float dy = target_y - pp.track->y;
    return atan2f(dy, dx);
}

static inline float get_curvature(void) {
    const float alpha = get_alpha();
    if (alpha == 0.0f) return 0.0f;

    return (WHEEL_BASE_CM * sinf(alpha)) / pp.lookahead;
}

static inline void update_target_speeds(void) {
    const float curvature = get_curvature();
    const float base_speed = pp.pid->speed_pid->base_speed;

    speed_left = base_speed * (1 - curvature);
    speed_right = base_speed * (1 + curvature);
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

void set_lookahead(const uint8_t distance) { pp.lookahead = distance; }
