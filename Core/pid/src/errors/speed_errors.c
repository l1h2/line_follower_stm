#include "pid/errors/speed_errors.h"

#include <stdlib.h>

#include "sensors/encoder.h"

#define BASE_SPEED 20.0f  // cm/s

static SpeedErrors errors = {
    .left_target_speed = BASE_SPEED,
    .right_target_speed = BASE_SPEED,
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
    errors.left_target_speed = BASE_SPEED;
    errors.right_target_speed = BASE_SPEED;
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
