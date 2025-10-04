#include "pid/errors/speed_errors.h"

#include <stdlib.h>

#include "sensors/encoder.h"

static SpeedErrors errors = {
    .left_target_speed = 0.0f,
    .right_target_speed = 0.0f,
    .left_error = 0.0f,
    .right_error = 0.0f,
    .left_last_error = 0.0f,
    .right_last_error = 0.0f,
    .left_delta_error = 0.0f,
    .right_delta_error = 0.0f,
    .left_error_sum = 0.0f,
    .right_error_sum = 0.0f,
};

static const EncoderData* encoders = NULL;

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

void init_speed_errors(const ErrorStruct* error_struct) {
    encoders = error_struct->sensors->encoders;
}

const SpeedErrors* get_speed_errors(void) { return &errors; }

void update_speed_errors(void) {
    update_encoder_data();

    update_error();
    update_error_sum();
    update_delta_error();
    update_last_error();
}

void clear_speed_errors(void) {
    errors.left_target_speed = 0.0f;
    errors.right_target_speed = 0.0f;
    errors.left_error = 0.0f;
    errors.right_error = 0.0f;
    errors.left_last_error = 0.0f;
    errors.right_last_error = 0.0f;
    errors.left_delta_error = 0.0f;
    errors.right_delta_error = 0.0f;
    errors.left_error_sum = 0.0f;
    errors.right_error_sum = 0.0f;

    clear_encoder_data();
}

void set_speed_targets(const float left_speed, const float right_speed) {
    errors.left_target_speed = left_speed;
    errors.right_target_speed = right_speed;
}
