#include "serial/serial_out.h"

#include <stddef.h>

#include "logger/logger.h"
#include "pid/pid.h"
#include "state_machine/handlers/config_handler.h"
#include "timer/time.h"
#include "turbine/turbine.h"

static const StateMachine* sm = NULL;
static const PidStruct* pid = NULL;

static uint32_t last_log_time = 0;

void send_message(const SerialMessages msg) {
    if (msg >= SERIAL_MESSAGE_COUNT) return;

    if (!sm) sm = get_state_machine();
    if (!pid) pid = get_pid();

    switch (msg) {
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
    // Skip command signals
    for (uint8_t i = STOP + 1; i < SERIAL_MESSAGE_COUNT; i++) {
        send_message((SerialMessages)i);
    }
}

void send_all_messages_async(const uint32_t interval) {
    if (!time_elapsed(last_log_time, interval)) return;

    last_log_time = time();
    send_all_messages();
}
