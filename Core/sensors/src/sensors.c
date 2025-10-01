#include "sensors/sensors.h"

#include "sensors/mpu.h"
#include "sensors/vision.h"

static SensorState sensors = {0};

void init_sensors(void) {
    if (sensors.ir_sensors == 0) sensors.ir_sensors = get_ir_sensors();
    if (sensors.mpu_data == 0) sensors.mpu_data = get_mpu_data();
}

const SensorState* get_sensors(void) { return &sensors; }

void update_sensors(const uint16_t timeout) {
    update_ir_sensors(timeout);
    update_mpu_data();
}

void clear_sensors(void) {
    clear_ir_sensors();
    clear_mpu_data();
}

void start_async_sensors_read(void) { start_ir_sensors_read(); }

void stop_async_sensors_read(void) { stop_ir_sensors_read(); }

bool sensors_are_reading(void) { return ir_sensors_are_reading(); }

bool update_sensors_async(void) {
    if (!update_ir_sensors_async()) return false;

    update_mpu_data();
    return true;
}
