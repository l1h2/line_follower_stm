#include "sensors/vision.h"

#include "hal/ir_sensors.h"

static IrSensorData sensors = {0, false, false, TOTAL_CENTRAL_SENSORS};

const IrSensorData* get_ir_sensors(void) { return &sensors; }

void update_ir_sensors(const uint16_t timeout) {
    sensors.central_sensors_state = read_central_sensors(timeout);

    const bool* side_sensors = get_side_sensor_values();
    sensors.left_sensor = side_sensors[0];
    sensors.right_sensor = side_sensors[1];
}

void clear_ir_sensors(void) {
    sensors.central_sensors_state = 0;
    sensors.left_sensor = false;
    sensors.right_sensor = false;
}

void start_ir_sensors_read(void) { start_read(); }

void stop_ir_sensors_read(void) { stop_read(); }

bool ir_sensors_are_reading(void) {
    return central_sensor_is_reading(SENSOR_READ_TIMEOUT_US);
}

bool update_ir_sensors_async(void) {
    if (central_sensor_is_reading(SENSOR_READ_TIMEOUT_US)) return false;

    sensors.central_sensors_state = read_central_sensors_async();

    const bool* side_sensors = get_side_sensor_values();
    sensors.left_sensor = side_sensors[0];
    sensors.right_sensor = side_sensors[1];

    return true;
}

uint16_t* get_central_ir_sensor_times(void) {
    return get_central_sensor_values();
}
