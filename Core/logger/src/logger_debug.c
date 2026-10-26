#include "logger/logger_debug.h"

#include "build_info.h"
#include "logger/logger_base.h"
#include "math/math.h"
#include "pid/errors/errors.h"
#include "pid/pid.h"
#include "pure_pursuit/pure_pursuit.h"
#include "sensors/encoder.h"
#include "sensors/mpu.h"
#include "sensors/sensors.h"
#include "sensors/vision.h"
#include "state_machine/handlers/config_handler.h"
#include "timer/time.h"
#include "track/track.h"
#include "track/track_selector.h"
#include "turbine/turbine.h"

#define SENSOR_READ_DEBUG_TIMEOUT_US 3000

static uint32_t last_log_time = 0;
static uint32_t last_sensor_update = 0;

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
void debug_print_float(float value, uint8_t decimal_places) {
    print_float(value, decimal_places);
}
void debug_print_binary(const uint8_t byte) { print_binary(byte); }
void debug_print_bool(const bool value) { print_bool(value); }
void debug_print_string(const char* str) { print_string(str); }
void debug_print_new_line(void) { print_new_line(); }
void debug_print(const char* str) { print(str); }

void debug_print_central_ir_sensors(void) {
    const SensorState* const sensors = get_sensors();
    const uint8_t total = sensors->ir_sensors->total_central_sensors;

    for (uint8_t i = 0; i < total; i++) {
        print_bit(i, sensors->ir_sensors->central_sensors_state);
        if (i < total - 1) print_string(" - ");
    }
    print_new_line();
}

static inline void debug_print_ir_sensor_times(
    const SensorState* const sensors) {
    const uint16_t* const times = get_central_ir_sensor_times();
    const uint8_t total = sensors->ir_sensors->total_central_sensors;

    for (uint8_t i = 0; i < total; i++) {
        print_word(times[i]);
        if (i < total - 1) print_string(" - ");
    }
}

void debug_print_ir_sensors(void) {
    const SensorState* const sensors = get_sensors();

    print_bool(sensors->ir_sensors->left_sensor);
    print_string(" | ");

    debug_print_ir_sensor_times(sensors);

    print_string(" | ");
    print_bool(sensors->ir_sensors->right_sensor);

    print_new_line();
}

void debug_print_errors(void) {
    const ErrorStruct* const errors = get_errors();

    print_string("Error byte: ");
    print_reverse_binary(errors->sensors->ir_sensors->central_sensors_state);
    print_string(" - Error: ");
    print_signed_byte(errors->error);
    print_string(" - Last error: ");
    print_signed_byte(errors->delta_error);
    print_string(" - Error sum: ");
    print_signed_word(errors->error_sum);
    print_new_line();
}

void debug_print_mpu_accelerations(void) {
    const MpuData* const mpu = get_mpu_data();

    print_string("Accel [X Y Z]:  ");
    print_signed_word(mpu->accel_x);
    print_string("  /  ");
    print_signed_word(mpu->accel_y);
    print_string("  /  ");
    print_signed_word(mpu->accel_z);
    print_new_line();
}

void debug_print_mpu_gyroscopes(void) {
    const MpuData* const mpu = get_mpu_data();

    print_string("Gyro  [X Y Z]:  ");
    print_signed_word(mpu->gyro_x);
    print_string("  /  ");
    print_signed_word(mpu->gyro_y);
    print_string("  /  ");
    print_signed_word(mpu->gyro_z);
    print_new_line();
}

void debug_print_mpu_gyroscope_biases(void) {
    const MpuData* const mpu = get_mpu_data();

    print_string("Gyro Biases [X Y Z]:  ");
    print_float(mpu->bias_gyro_x, 2);
    print_string("  /  ");
    print_float(mpu->bias_gyro_y, 2);
    print_string("  /  ");
    print_float(mpu->bias_gyro_z, 2);
    print_new_line();
}

void debug_print_mpu_angles(void) {
    const MpuData* const mpu = get_mpu_data();

    print_string("Roll/Pitch/Yaw:  ");
    print_float(rad_to_deg(mpu->roll), 2);
    print_string("  /  ");
    print_float(rad_to_deg(mpu->pitch), 2);
    print_string("  /  ");
    print_float(rad_to_deg(mpu->yaw), 2);
    print_new_line();
}

void debug_print_mpu_temperature(void) {
    const MpuData* const mpu = get_mpu_data();

    print_string("Temp:  ");
    print_signed_word(mpu->temp);
    print_new_line();
}

void debug_print_mpu_data(void) {
    debug_print_mpu_accelerations();
    debug_print_mpu_gyroscopes();
    debug_print_mpu_gyroscope_biases();
    debug_print_mpu_angles();
    debug_print_mpu_temperature();
}

void debug_print_encoder_pulses(void) {
    const EncoderData* const encoders = get_encoder_data();

    print_string("Pulses [L R]:  ");
    print_signed_long(encoders->left_encoder);
    print_string("  /  ");
    print_signed_long(encoders->right_encoder);
    print_new_line();
}

void debug_print_encoder_distances(void) {
    const EncoderData* const encoders = get_encoder_data();

    print_string("Distance (cm) [L R T]:  ");
    print_float(encoders->left_distance, 2);
    print_string("  /  ");
    print_float(encoders->right_distance, 2);
    print_string("  /  ");
    print_float(encoders->distance, 2);
    print_new_line();
}

void debug_print_encoder_speeds(void) {
    const EncoderData* const encoders = get_encoder_data();

    print_string("Speed (cm/s) [L R T]:  ");
    print_float(encoders->left_speed, 2);
    print_string("  /  ");
    print_float(encoders->right_speed, 2);
    print_string("  /  ");
    print_float(encoders->speed, 2);
    print_new_line();
}

void debug_print_encoder_data(void) {
    debug_print_encoder_pulses();
    debug_print_encoder_distances();
    debug_print_encoder_speeds();
}

static inline void update_sensor_data_for_debug(void) {
    if (!time_elapsed(last_sensor_update, SENSOR_UPDATE_INTERVAL_MS)) return;

    last_sensor_update = time();
    update_sensors(SENSOR_READ_DEBUG_TIMEOUT_US, false);
}

bool debug_print_diagnostics(void) {
    if (!time_elapsed(last_log_time, LOG_INTERVAL)) {
        update_sensor_data_for_debug();
        return false;
    }

    last_log_time = time();
    update_errors(SENSOR_READ_DEBUG_TIMEOUT_US, true);

    debug_print_new_line();

    debug_print_ir_sensors();
    debug_print_new_line();

    debug_print_errors();
    debug_print_new_line();

    debug_print_mpu_data();
    debug_print_new_line();

    debug_print_encoder_data();
    debug_print_new_line();

    debug_print(
        "**********************************************************************"
        "*");

    return true;
}

void debug_print_run_config(void) {
    const StateMachine* const sm = get_state_machine();
    const PidStruct* const pid = get_pid();
    const EncoderData* const encoders = get_encoder_data();

    print_string("Build:  ");
    print(BUILD_TIMESTAMP);

    print_string("Track [Name Waypoints]:  ");
    print_string(TRACK_NAME);
    print_string("  /  ");
    print_word(WAYPOINT_COUNT);
    print_new_line();

    print_string("Modes [Running Stop Log]:  ");
    print_word(sm->running_mode);
    print_string("  /  ");
    print_word(sm->stop_mode);
    print_string("  /  ");
    print_bool(sm->log_data);
    print_new_line();

    print_string("Stop [Laps Time(s) Distance(cm) Markers]:  ");
    print_word(sm->laps);
    print_string("  /  ");
    print_long(sm->stop_time / 1000);
    print_string("  /  ");
    print_word(sm->stop_distance);
    print_string("  /  ");
    print_word(sm->stop_markers);
    print_new_line();

    print_string("Line PID [Kp Ki Kd Alpha Clamp]:  ");
    print_word(pid->delta_pid->kp);
    print_string("  /  ");
    print_word(pid->delta_pid->ki);
    print_string("  /  ");
    print_word(pid->delta_pid->kd);
    print_string("  /  ");
    print_float(pid->delta_pid->alpha, 4);
    print_string("  /  ");
    print_word(pid->delta_pid->clamp);
    print_new_line();

    print_string("Base PWM PID [Kb Kff]:  ");
    print_word(pid->base_pwm_pid->kp);
    print_string("  /  ");
    print_word(pid->base_pwm_pid->kff);
    print_new_line();

    print_string("PWM [Base Max Accel Turbine]:  ");
    print_word(pid->base_pwm);
    print_string("  /  ");
    print_word(pid->max_pwm);
    print_string("  /  ");
    print_word(pid->acceleration);
    print_string("  /  ");
    print_word(get_turbine_pwm());
    print_new_line();

    print_string("Speed PID [Kp Ki Kd Kff]:  ");
    print_word(pid->speed_pid->kp);
    print_string("  /  ");
    print_float(pid->speed_pid->ki, 4);
    print_string("  /  ");
    print_word(pid->speed_pid->kd);
    print_string("  /  ");
    print_word(pid->speed_pid->kff);
    print_new_line();

    print_string("Pure Pursuit [Base speed(cm/s) Lookahead(cm)]:  ");
    print_float(pid->speed_pid->base_speed, 2);
    print_string("  /  ");
    print_word(get_pure_pursuit()->lookahead);
    print_new_line();

    print_string(
        "Odometry [Effective wheel base(cm) Curvature gain IMU alpha]:  ");
    print_float(encoders->effective_wheel_base, 2);
    print_string("  /  ");
    print_float(encoders->wheel_base_correction, 2);
    print_string("  /  ");
    print_float(get_track()->imu_alpha, 4);
    print_new_line();
}
