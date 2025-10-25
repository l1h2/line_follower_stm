#include "pid/errors/speed_errors.h"

#include <stdlib.h>

#include "sensors/encoder.h"

static SpeedErrors errors = {0};

static const EncoderData* encoders = NULL;

static float last_left_target_speed = 0.0f;
static float last_right_target_speed = 0.0f;

static inline void update_error(void) {
    errors.left_error = errors.left_target_speed - encoders->left_speed;
    errors.right_error = errors.right_target_speed - encoders->right_speed;
}

static inline void update_error_sum(void) {
    errors.left_error_sum += errors.left_error;
    errors.right_error_sum += errors.right_error;
}

static inline void update_delta_error(void) {
    errors.left_delta_error = errors.left_error - errors.left_last_error;
    errors.right_delta_error = errors.right_error - errors.right_last_error;
}

static inline void update_last_error(void) {
    errors.left_last_error = errors.left_error;
    errors.right_last_error = errors.right_error;
}

static inline void update_delta_speed(void) {
    errors.left_delta_target_speed =
        errors.left_target_speed - last_left_target_speed;
    errors.right_delta_target_speed =
        errors.right_target_speed - last_right_target_speed;
}

const SpeedErrors* init_speed_errors(const ErrorStruct* const error_struct) {
    encoders = error_struct->sensors->encoders;
    return &errors;
}

const SpeedErrors* get_speed_errors(void) { return &errors; }

void update_speed_errors(void) {
    update_error();
    update_error_sum();
    update_delta_error();
    update_last_error();
    update_delta_speed();
}

void clear_speed_errors(void) {
    errors = (SpeedErrors){0};
    last_left_target_speed = 0.0f;
    last_right_target_speed = 0.0f;
}

void set_speed_targets(const float left_speed, const float right_speed) {
    errors.left_target_speed = left_speed;
    errors.right_target_speed = right_speed;
}
