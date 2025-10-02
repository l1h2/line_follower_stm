#include "pid/pid.h"

#include <stdlib.h>

#include "motors/motors.h"
#include "pid/base_pwm_pid.h"
#include "pid/delta_pid.h"
#include "pid/errors.h"
#include "pid/pid_base.h"
#include "timer/time.h"

#define INITIAL_PWM 200
#define BASE_PWM 300
#define ACCELERATION_STEP 1  // PWM units per update

static PidStruct pid = {
    .base_pwm = BASE_PWM,
    .current_pwm = INITIAL_PWM < BASE_PWM ? INITIAL_PWM : BASE_PWM,
    .max_pwm = 0,
    .min_pwm = 0,
    .acceleration = ACCELERATION_STEP,
    .errors = NULL,
    .delta_pid = NULL,
    .base_pwm_pid = NULL,
};

static bool is_updating_errors = false;

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

void init_pid(void) {
    pid.max_pwm = get_max_pwm();
    pid.min_pwm = -pid.max_pwm;

    init_errors();
    pid.errors = get_errors();

    init_delta_pwm_pid(pid.errors);
    pid.delta_pid = get_delta_pwm_pid_ptr();

    init_base_pwm_pid(pid.errors);
    pid.base_pwm_pid = get_base_pwm_pid_ptr();
}

const PidStruct* get_pid(void) { return &pid; }

bool update_pid(void) {
    if (!updates_pending()) return false;

    if (!is_updating_errors) {
        update_errors_async();
        is_updating_errors = true;
        return false;
    }

    if (!update_errors_async()) return false;

    is_updating_errors = false;
    update_pid_times();
    update_motors();
    return true;
}

void reset_pwm(void) { pid.current_pwm = pid.base_pwm; }

void restart_pwm(void) {
    pid.current_pwm = INITIAL_PWM < pid.base_pwm ? INITIAL_PWM : pid.base_pwm;
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

    reset_pwm();
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

void set_pwm_kb(const uint8_t kb) { set_base_pwm_kp(kb); }

void set_pwm_kff(const uint8_t kff) { set_base_pwm_kff(kff); }
