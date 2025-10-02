#include "logger/logger_debug.h"

#include "logger/logger_base.h"
#include "pid/errors.h"
#include "sensors/sensors.h"
#include "sensors/vision.h"
#include "timer/time.h"

#define SENSOR_READ_DEBUG_TIMEOUT_US 3000

static uint32_t last_log_time = 0;

void debug_print_char(const char c) { print_char(c); }
void debug_print_bit(const uint8_t bit_position, const uint8_t byte) {
    print_bit(bit_position, byte);
}
void debug_print_byte(const uint8_t byte) { print_byte(byte); }
void debug_print_hex(const uint8_t byte) { print_hex(byte); }
void debug_print_signed_byte(int8_t byte) { print_signed_byte(byte); }
void debug_print_word(const uint16_t word) { print_word(word); }
void debug_print_signed_word(int16_t word) { print_signed_word(word); }
void debug_print_long(const uint32_t dword) { print_long(dword); }
void debug_print_signed_long(int32_t dword) { print_signed_long(dword); }
void debug_print_binary(const uint8_t byte) { print_binary(byte); }
void debug_print_bool(const bool value) { print_bool(value); }
void debug_print_string(const char *str) { print_string(str); }
void debug_print_new_line(void) { print_new_line(); }
void debug_print(const char *str) { print(str); }

void debug_print_central_ir_sensors(void) {
    const SensorState *sensors = get_sensors();

    for (int8_t i = sensors->ir_sensors->total_central_sensors - 1; i >= 0;
         i--) {
        print_bit(i, sensors->ir_sensors->central_sensors_state);
        if (i > 0) print_string(" - ");
    }
    print_new_line();
}

static inline void debug_print_ir_sensor_times(const SensorState *sensors) {
    const uint16_t *times = get_central_ir_sensor_times();
    for (int8_t i = sensors->ir_sensors->total_central_sensors - 1; i >= 0;
         i--) {
        print_word(times[i]);
        if (i > 0) print_string(" - ");
    }
}

void debug_print_ir_sensors(void) {
    const SensorState *sensors = get_sensors();

    print_bool(sensors->ir_sensors->left_sensor);
    print_string(" | ");

    debug_print_ir_sensor_times(sensors);

    print_string(" | ");
    print_bool(sensors->ir_sensors->right_sensor);

    print_new_line();
}

void debug_print_errors(void) {
    const ErrorStruct *errors = get_errors();

    print_string("Error byte: ");
    print_binary(errors->sensors->ir_sensors->central_sensors_state);
    print_string(" - Error: ");
    print_signed_byte(errors->error);
    print_string(" - Last error: ");
    print_signed_byte(errors->delta_error);
    print_string(" - Error sum: ");
    print_signed_word(errors->error_sum);
    print_new_line();
}

void debug_print_mpu_data(void) {
    const SensorState *sensors = get_sensors();

    print_string("Accel [X Y Z]: ");
    print_signed_word(sensors->mpu_data->accel_x);
    print_string(", ");
    print_signed_word(sensors->mpu_data->accel_y);
    print_string(", ");
    print_signed_word(sensors->mpu_data->accel_z);
    print_new_line();

    print_string("Gyro  [X Y Z]: ");
    print_signed_word(sensors->mpu_data->gyro_x);
    print_string(", ");
    print_signed_word(sensors->mpu_data->gyro_y);
    print_string(", ");
    print_signed_word(sensors->mpu_data->gyro_z);
    print_new_line();

    print_string("Temp: ");
    print_signed_word(sensors->mpu_data->temp);
    print_new_line();
}

bool debug_print_diagnostics(void) {
    if (!time_elapsed(last_log_time, LOG_INTERVAL)) return false;

    last_log_time = time();
    update_errors(SENSOR_READ_DEBUG_TIMEOUT_US);

    debug_print_new_line();
    debug_print_ir_sensors();
    debug_print_new_line();
    debug_print_errors();
    debug_print_new_line();
    debug_print_mpu_data();
    debug_print_new_line();
    debug_print(
        "**********************************************************************"
        "*");

    return true;
}
