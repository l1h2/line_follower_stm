#include "pid/pid.h"

#include <stdlib.h>

#include "motors/motors.h"
#include "pid/controllers/base_pwm_pid.h"
#include "pid/controllers/delta_pid.h"
#include "pid/controllers/speed_pid.h"
#include "pid/errors/errors.h"
#include "pid/errors/speed_errors.h"
#include "pid/pid_base.h"
#include "sensors/encoder.h"
#include "timer/time.h"

#define BASE_PWM 300
#define ACCELERATION_STEP 1  // PWM units per update

static PidStruct pid = {
    .base_pwm = BASE_PWM,
    .current_pwm = BASE_PWM,
    .max_pwm = 0,
    .min_pwm = 0,
    .acceleration = ACCELERATION_STEP,
    .errors = NULL,
    .delta_pid = NULL,
    .base_pwm_pid = NULL,
    .speed_pid = NULL,
};

static inline bool updates_pending(void) {
    return update_pending_delta_pwm_pid() || update_pending_base_pwm_pid();
}

static inline void update_pid_times(void) {
    if (update_pending_delta_pwm_pid()) update_delta_pwm_pid();
    if (update_pending_base_pwm_pid()) update_base_pwm_pid();
}

static inline void update_current_pwm(void) {
    const int16_t ref_pwm = pid.base_pwm + get_base_pwm_pid();
    const int16_t accel_pwm = pid.current_pwm + pid.acceleration;

    if (accel_pwm >= ref_pwm) {
        pid.current_pwm = ref_pwm;
        return;
    }

    pid.current_pwm = accel_pwm;
}

static int16_t get_new_pwm(const int16_t delta_term) {
    update_current_pwm();

    int16_t pwm = pid.current_pwm + delta_term;

    if (pwm > pid.max_pwm) return pid.max_pwm;
    if (pwm < pid.min_pwm) return pid.min_pwm;

    return pwm;
}

static void update_motors(void) {
    const int16_t delta_pwm = get_delta_pwm_pid();

    const int16_t left_pwm = get_new_pwm(delta_pwm);
    const int16_t right_pwm = get_new_pwm(-delta_pwm);

    set_motors(left_pwm, right_pwm);
}

const PidStruct* init_pid(const SensorState* const sensors) {
    pid.max_pwm = get_max_pwm();
    pid.min_pwm = -pid.max_pwm;

    pid.errors = init_errors(sensors);
    pid.delta_pid = init_delta_pwm_pid(pid.errors);
    pid.base_pwm_pid = init_base_pwm_pid(pid.errors);
    pid.speed_pid = init_base_speed_pid(pid.errors);

    return &pid;
}

const PidStruct* get_pid(void) { return &pid; }

bool update_pid(void) {
    if (!updates_pending()) return false;
    if (!update_errors_async(false)) return false;

    update_pid_times();
    update_motors();

    return true;
}

bool update_speed_pid(void) {
    if (!update_pending_base_speed_pid()) return false;

    update_encoder_data();
    set_speed_targets(pid.speed_pid->base_speed, pid.speed_pid->base_speed);
    update_speed_errors();
    update_base_speed_pid();

    int16_t left_pwm;
    int16_t right_pwm;
    get_base_speed_pid(&left_pwm, &right_pwm);
    set_motors(left_pwm, right_pwm);

    return true;
}

void restart_pid(void) {
    pid.current_pwm = pid.base_pwm;
    clear_errors();
    clear_speed_errors();
}

void set_base_pwm(const uint16_t pwm) {
    const uint16_t min_pwm = get_min_pwm();

    if (pwm >= pid.max_pwm) {
        pid.base_pwm = pid.max_pwm;
    } else if (pwm <= min_pwm) {
        pid.base_pwm = min_pwm;
    } else {
        pid.base_pwm = pwm;
    }

    pid.current_pwm = pid.base_pwm;
}

void set_current_pwm(const int16_t pwm) {
    if (pwm >= pid.max_pwm) {
        pid.current_pwm = pid.max_pwm;
    } else if (pwm <= pid.min_pwm) {
        pid.current_pwm = pid.min_pwm;
    } else {
        pid.current_pwm = pwm;
    }
}

void set_max_pwm(const uint16_t pwm) {
    const uint16_t max_pwm = get_max_pwm();
    const uint16_t min_pwm = get_min_pwm();

    if (pwm > max_pwm) {
        pid.max_pwm = max_pwm;
    } else if (pwm < min_pwm) {
        pid.max_pwm = min_pwm;
    } else {
        pid.max_pwm = pwm;
    }

    pid.min_pwm = -pid.max_pwm;
}

void set_pwm_accel(const uint16_t accel) { pid.acceleration = accel; }

void set_pwm_kp(const uint8_t kp) { set_delta_pwm_kp(kp); }

void set_pwm_ki(const uint8_t ki) { set_delta_pwm_ki(ki); }

void set_pwm_kd(const uint16_t kd) { set_delta_pwm_kd(kd); }

void set_pwm_alpha(const float alpha) { set_delta_pwm_alpha(alpha); }

void set_pwm_clamp(const uint16_t clamp) { set_delta_pwm_clamp(clamp); }

void set_pwm_kb(const uint8_t kb) { set_base_pwm_kp(kb); }

void set_pwm_kff(const uint8_t kff) { set_base_pwm_kff(kff); }

void set_speed_kp(const uint16_t kp) { set_base_speed_kp(kp); }

void set_speed_ki(const float ki) { set_base_speed_ki(ki); }

void set_speed_kd(const uint16_t kd) { set_base_speed_kd(kd); }

void set_speed_kff(const uint16_t kff) { set_base_speed_kff(kff); }

void set_speed(const float speed) { set_base_speed(speed); }
