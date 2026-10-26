#include "serial/serial_out.h"

#include <stddef.h>

#include "logger/logger.h"
#include "pid/controllers/speed_pid.h"
#include "timer/time.h"
#include "track/track.h"
#include "turbine/turbine.h"

static const StateMachine* sm = NULL;
static const SensorState* sensors = NULL;
static const PidStruct* pid = NULL;
static const TrackCounters* track = NULL;
static const PurePursuit* pure_pursuit = NULL;

static uint32_t last_log_time = 0;

static uint8_t operation_data[OPERATION_DATA_SIZE] = {0};
static uint8_t extended_data[EXTENDED_DATA_SIZE] = {0};
static TrackTelemetry track_telemetry = {0};

static inline uint16_t parse_float(float value, uint8_t precision) {
    for (; precision > 0; precision--) value *= 10.0f;
    return (uint16_t)(value + 0.5f);
}

static inline int16_t parse_signed_float(float value, uint8_t precision) {
    for (; precision > 0; precision--) value *= 10.0f;
    return (int16_t)(value + (value >= 0.0f ? 0.5f : -0.5f));
}

static inline void pack_int16(uint8_t* const data, const int16_t value) {
    data[0] = value & 0xFF;
    data[1] = (value >> 8);
}

static inline void update_operation_data(void) {
    track_telemetry = consume_track_telemetry();

    operation_data[0] = sensors->ir_sensors->central_sensors_state;
    operation_data[1] = sensors->ir_sensors->left_sensor;
    operation_data[1] |= sensors->ir_sensors->right_sensor << 1;
    operation_data[1] |= (track_telemetry.event & 0x03) << 2;

    // Convert from cm to mm for higher precision
    const int16_t x = parse_signed_float(track->x, 1);
    const int16_t y = parse_signed_float(track->y, 1);
    operation_data[2] = x & 0xFF;
    operation_data[3] = (x >> 8);
    operation_data[4] = y & 0xFF;
    operation_data[5] = (y >> 8);

    const int16_t heading = parse_signed_float(track->heading, 4);
    operation_data[6] = heading & 0xFF;
    operation_data[7] = (heading >> 8);
}

static inline void update_extended_data(void) {
    update_operation_data();
    for (uint8_t i = 0; i < OPERATION_DATA_SIZE; i++) {
        extended_data[i] = operation_data[i];
    }

    pack_int16(&extended_data[8],
               parse_signed_float(sensors->encoders->left_speed, 1));
    pack_int16(&extended_data[10],
               parse_signed_float(sensors->encoders->right_speed, 1));
    pack_int16(
        &extended_data[12],
        parse_signed_float(pid->errors->speed_errors->left_target_speed, 1));
    pack_int16(
        &extended_data[14],
        parse_signed_float(pid->errors->speed_errors->right_target_speed, 1));

    int16_t left_pwm, right_pwm;
    get_base_speed_pwm(&left_pwm, &right_pwm);
    pack_int16(&extended_data[16], left_pwm);
    pack_int16(&extended_data[18], right_pwm);

    extended_data[20] = track_telemetry.right_sensor;
}

void init_serial_out(const StateMachine* const state_machine,
                     const SensorState* const sensor_state,
                     const PidStruct* const pid_struct,
                     const TrackCounters* const track_counters,
                     const PurePursuit* const pure_pursuit_struct) {
    sm = state_machine;
    sensors = sensor_state;
    pid = pid_struct;
    track = track_counters;
    pure_pursuit = pure_pursuit_struct;
}

void send_message(const SerialMessages msg) {
    if (msg >= SERIAL_MESSAGE_COUNT) return;

    switch (msg) {
        case PING:
            send_data(msg, NULL);
            break;
        case START:
            send_data(msg, NULL);
            break;
        case STOP:
            send_data(msg, NULL);
            break;
        case STATE:
            send_data(msg, (const uint8_t*)&sm->current_state);
            break;
        case RUNNING_MODE:
            send_data(msg, (const uint8_t*)&sm->running_mode);
            break;
        case STOP_MODE:
            send_data(msg, (const uint8_t*)&sm->stop_mode);
            break;
        case LAPS:
            send_data(msg, (const uint8_t*)&sm->laps);
            break;
        case STOP_TIME: {
            const uint8_t stop_time = (uint8_t)(sm->stop_time / 1000);
            send_data(msg, (const uint8_t*)&stop_time);
            break;
        }
        case LOG_DATA:
            send_data(msg, (const uint8_t*)&sm->log_data);
            break;
        case PID_KP:
            send_data(msg, (const uint8_t*)&pid->delta_pid->kp);
            break;
        case PID_KI:
            send_data(msg, (const uint8_t*)&pid->delta_pid->ki);
            break;
        case PID_KD:
            send_data(msg, (const uint8_t*)&pid->delta_pid->kd);
            break;
        case PID_KB:
            send_data(msg, (const uint8_t*)&pid->base_pwm_pid->kp);
            break;
        case PID_KFF:
            send_data(msg, (const uint8_t*)&pid->base_pwm_pid->kff);
            break;
        case PID_ACCEL:
            send_data(msg, (const uint8_t*)&pid->acceleration);
            break;
        case PID_BASE_PWM:
            send_data(msg, (const uint8_t*)&pid->base_pwm);
            break;
        case PID_MAX_PWM:
            send_data(msg, (const uint8_t*)&pid->max_pwm);
            break;
        case TURBINE_PWM:
            const uint16_t turbine_pwm = get_turbine_pwm();
            send_data(msg, (const uint8_t*)&turbine_pwm);
            break;
        case SPEED_KP:
            send_data(msg, (const uint8_t*)&pid->speed_pid->kp);
            break;
        case SPEED_KI:
            const uint16_t speed_ki = parse_float(pid->speed_pid->ki, 4);
            send_data(msg, (const uint8_t*)&speed_ki);
            break;
        case SPEED_KD:
            send_data(msg, (const uint8_t*)&pid->speed_pid->kd);
            break;
        case BASE_SPEED:
            const uint16_t base_speed =
                parse_float(pid->speed_pid->base_speed, 2);
            send_data(msg, (const uint8_t*)&base_speed);
            break;
        case PID_ALPHA:
            // Convert from [0.0, 1.0] range to percentage
            const uint16_t alpha = parse_float(pid->delta_pid->alpha, 4);
            send_data(msg, (const uint8_t*)&alpha);
            break;
        case PID_CLAMP:
            send_data(msg, (const uint8_t*)&pid->delta_pid->clamp);
            break;
        case STOP_DISTANCE:
            send_data(msg, (const uint8_t*)&sm->stop_distance);
            break;
        case STOP_MARKERS:
            send_data(msg, (const uint8_t*)&sm->stop_markers);
            break;
        case OPERATION_DATA:
            update_operation_data();
            send_data(msg, operation_data);
            break;
        case EXTENDED_DATA:
            update_extended_data();
            send_data(msg, extended_data);
            break;
        case LOOKAHEAD:
            send_data(msg, (const uint8_t*)&pure_pursuit->lookahead);
            break;
        case SPEED_KFF:
            send_data(msg, (const uint8_t*)&pid->speed_pid->kff);
            break;
        case CURVATURE_GAIN:
            const uint16_t gain =
                parse_float(sensors->encoders->wheel_base_correction, 2);
            send_data(msg, (const uint8_t*)&gain);
            break;
        case IMU_ALPHA:
            // Convert from [0.0, 1.0] range to percentage
            const uint16_t imu_alpha = parse_float(track->imu_alpha, 4);
            send_data(msg, (const uint8_t*)&imu_alpha);
            break;
        default:
            debug_print("Attempted to send unknown message");
            break;
    }
}

void send_messages(const SerialMessages* msgs, const uint8_t count) {
    if (!msgs || count == 0) return;
    for (uint8_t i = 0; i < count; i++) send_message(msgs[i]);
}

void send_all_messages(void) {
    // Skips command signals and telemetry data
    for (uint8_t i = STOP + 1; i < SERIAL_MESSAGE_COUNT; i++) {
        if (i == OPERATION_DATA || i == EXTENDED_DATA) continue;
        send_message((SerialMessages)i);
    }
}

void send_all_messages_async(const uint32_t interval) {
    if (!time_elapsed(last_log_time, interval)) return;

    last_log_time = time();
    send_all_messages();
}
