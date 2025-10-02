#include "sensors/encoder.h"

#include <stdint.h>

#include "hal/encoders.h"

#define WHEEL_CIRCUMFERENCE_MM (WHEEL_DIAMETER_MM * 3.14159f)
#define MM_PER_PULSE (WHEEL_CIRCUMFERENCE_MM / ENCODER_PULSES_PER_REV)

static EncoderData encoder_data = {0};

static int16_t left_encoder = 0;
static int16_t right_encoder = 0;

static inline void update_distance(void) {
    encoder_data.distance +=
        ((float)(left_encoder + right_encoder) / 2.0f) * MM_PER_PULSE;
}

void init_encoder(void) { init_encoder_counters(); }

const EncoderData* get_encoder_data(void) { return &encoder_data; }

void restart_encoders(void) { set_encoders(0); }

void update_encoder_data(void) {
    left_encoder = get_encoder_left();
    right_encoder = get_encoder_right();
    restart_encoders();

    encoder_data.left_encoder += left_encoder;
    encoder_data.right_encoder += right_encoder;
    update_distance();
}

void clear_encoder_data(void) {
    left_encoder = 0;
    right_encoder = 0;
    encoder_data.left_encoder = 0;
    encoder_data.right_encoder = 0;
    encoder_data.distance = 0.0f;
}

void start_encoders(void) {
    restart_encoders();
    clear_encoder_data();
    update_encoder_data();
}
