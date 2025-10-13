#include "sensors/encoder.h"

#include "hal/encoders.h"
#include "timer/time.h"

#define WHEEL_CIRCUMFERENCE_MM (WHEEL_DIAMETER_MM * 3.14159f)
#define MM_PER_PULSE (WHEEL_CIRCUMFERENCE_MM / ENCODER_PULSES_PER_REV)
#define CM_PER_PULSE (MM_PER_PULSE / 10.0f)

static EncoderData encoder_data = {0};

static int16_t left_encoder = 0;
static int16_t right_encoder = 0;
static uint32_t current_time = 0;

static inline void update_counters(void) {
    encoder_data.left_encoder += left_encoder;
    encoder_data.right_encoder += right_encoder;
}

static inline void update_distances(void) {
    encoder_data.current_left_distance = ((float)left_encoder * CM_PER_PULSE);
    encoder_data.current_right_distance = ((float)right_encoder * CM_PER_PULSE);
    encoder_data.current_distance = (encoder_data.current_left_distance +
                                     encoder_data.current_right_distance) /
                                    2.0f;

    encoder_data.left_distance += encoder_data.current_left_distance;
    encoder_data.right_distance += encoder_data.current_right_distance;
    encoder_data.distance =
        (encoder_data.left_distance + encoder_data.right_distance) / 2.0f;
}

static inline void update_angle(void) {
    const float delta_distance = encoder_data.current_right_distance -
                                 encoder_data.current_left_distance;

    encoder_data.current_angle = delta_distance / WHEEL_BASE_CM;
    encoder_data.heading += encoder_data.current_angle;
}

static inline void update_speeds(void) {
    encoder_data.current_interval =
        (current_time - encoder_data.last_update_time) / 1000.0f;
    if (encoder_data.current_interval <= 0.0f) return;

    encoder_data.left_speed =
        encoder_data.current_left_distance / encoder_data.current_interval;
    encoder_data.right_speed =
        encoder_data.current_right_distance / encoder_data.current_interval;
    encoder_data.speed =
        (encoder_data.left_speed + encoder_data.right_speed) / 2.0f;
}

void init_encoder(void) { init_encoder_counters(); }

const EncoderData* get_encoder_data(void) { return &encoder_data; }

void restart_encoders(void) { set_encoders(0); }

void update_encoder_data(void) {
    current_time = time();

    left_encoder = get_encoder_left();
    right_encoder = get_encoder_right();
    restart_encoders();

    update_counters();
    update_distances();
    update_angle();
    update_speeds();

    encoder_data.last_update_time = current_time;
}

bool update_encoder_data_async(const uint32_t interval) {
    if (!time_elapsed(encoder_data.last_update_time, interval)) return false;

    update_encoder_data();
    return true;
}

void clear_encoder_data(void) {
    left_encoder = 0;
    right_encoder = 0;
    encoder_data.current_left_distance = 0.0f;
    encoder_data.current_right_distance = 0.0f;

    encoder_data.left_encoder = 0;
    encoder_data.right_encoder = 0;
    encoder_data.left_distance = 0.0f;
    encoder_data.right_distance = 0.0f;
    encoder_data.distance = 0.0f;
    encoder_data.left_speed = 0.0f;
    encoder_data.right_speed = 0.0f;
    encoder_data.speed = 0.0f;
    encoder_data.last_update_time = time();
}

void start_encoders(void) {
    clear_encoder_data();
    restart_encoders();
    update_encoder_data();
}
