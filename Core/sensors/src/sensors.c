#include "sensors/sensors.h"

#include <stdlib.h>

#include "sensors/encoder.h"
#include "sensors/mpu.h"
#include "sensors/vision.h"

static SensorState sensors = {
    .ir_sensors = NULL,
    .mpu_data = NULL,
    .encoders = NULL,
};

const SensorState* init_sensors(void) {
    init_mpu();
    init_encoder();

    sensors.ir_sensors = get_ir_sensors();
    sensors.mpu_data = get_mpu_data();
    sensors.encoders = get_encoder_data();

    return &sensors;
}

const SensorState* get_sensors(void) { return &sensors; }

void update_sensors(const uint16_t timeout, const bool read_encoder) {
    update_ir_sensors(timeout);
    update_mpu_data();
    if (read_encoder) update_encoder_data();
}

void clear_sensors(void) {
    clear_ir_sensors();
    clear_mpu_data();
    clear_encoder_data();
}

void start_async_sensors_read(void) { start_ir_sensors_read(); }

void stop_async_sensors_read(void) { stop_ir_sensors_read(); }

bool sensors_are_reading(void) { return ir_sensors_are_reading(); }

bool update_sensors_async(const bool read_encoder) {
    if (!update_ir_sensors_async()) return false;

    update_mpu_data();
    if (read_encoder) update_encoder_data();
    return true;
}

void restart_sensors(void) {
    clear_ir_sensors();
    start_encoders();
    restart_mpu();
}
