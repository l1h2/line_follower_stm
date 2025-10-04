#include "serial/serial_in.h"

#include "hal/usart.h"
#include "logger/logger.h"
#include "pid/pid.h"
#include "serial/serial_base.h"
#include "serial/serial_out.h"
#include "state_machine/handlers/config_handler.h"
#include "turbine/turbine.h"

static SerialMessage current_msg = {INVALID_MESSAGE, 0, {0}};

static void read_message(void) {
    if (!usart_data_received()) return;

    const uint8_t next_char = usart_peek_char();
    if (next_char >= SERIAL_MESSAGE_COUNT) {
        // Invalid message, flush the byte
        (void)usart_read_char();
        return;
    }

    const uint8_t expected_size = SERIAL_MESSAGE_SIZES[next_char];

    // Data not fully received yet
    if (usart_data_available() < (1 + expected_size)) return;

    current_msg.message = (SerialMessages)usart_read_char();
    usart_read_buffer(current_msg.payload, expected_size);
    current_msg.size = expected_size;
}

static inline uint16_t parse_uint16(const uint8_t* const payload) {
    return (uint16_t)((payload[1] << 8) | payload[0]);
}

static inline uint32_t parse_uint32(const uint8_t* const payload) {
    return (uint32_t)((payload[3] << 24) | (payload[2] << 16) |
                      (payload[1] << 8) | payload[0]);
}

static inline float parse_float(const uint8_t* const payload) {
    const uint16_t raw = parse_uint16(payload);
    return (float)raw / 100.0f;
}

static void handle_message(void) {
    if (current_msg.message == INVALID_MESSAGE) return;

    switch (current_msg.message) {
        case START:
            set_can_run(true);
            break;
        case STOP:
            set_can_run(false);
            break;
        case RUNNING_MODE:
            set_running_mode((RunningModes)current_msg.payload[0]);
            break;
        case STOP_MODE:
            set_stop_mode((StopModes)current_msg.payload[0]);
            break;
        case LAPS:
            set_laps((uint8_t)current_msg.payload[0]);
            break;
        case STOP_TIME:
            set_stop_time((uint8_t)current_msg.payload[0]);
            break;
        case LOG_DATA:
            set_log_data((bool)current_msg.payload[0]);
            break;
        case PID_KP:
            set_pwm_kp((uint8_t)current_msg.payload[0]);
            break;
        case PID_KI:
            set_pwm_ki((uint8_t)current_msg.payload[0]);
            break;
        case PID_KD:
            set_pwm_kd(parse_uint16(current_msg.payload));
            break;
        case PID_KB:
            set_pwm_kb((uint8_t)current_msg.payload[0]);
            break;
        case PID_KFF:
            set_pwm_kff((uint8_t)current_msg.payload[0]);
            break;
        case PID_ACCEL:
            set_pwm_accel(parse_uint16(current_msg.payload));
            break;
        case PID_BASE_PWM:
            set_base_pwm(parse_uint16(current_msg.payload));
            break;
        case PID_MAX_PWM:
            set_max_pwm(parse_uint16(current_msg.payload));
            break;
        case TURBINE_PWM:
            set_turbine_pwm(parse_uint16(current_msg.payload));
            break;
        case SPEED_KP:
            set_speed_kp((uint8_t)current_msg.payload[0]);
            break;
        case SPEED_KI:
            set_speed_ki((uint8_t)current_msg.payload[0]);
            break;
        case SPEED_KD:
            set_speed_kd(parse_uint16(current_msg.payload));
            break;
        case BASE_SPEED:
            set_speed(parse_float(current_msg.payload));
            break;
        case PID_ALPHA:
            // Convert from percentage to [0.0, 1.0] range
            set_pwm_alpha(parse_float(current_msg.payload) / 100.0f);
            break;
        case PID_CLAMP:
            set_pwm_clamp(parse_uint16(current_msg.payload));
            break;
        default:
            debug_print("Received unknown message");
            return;
    }

    // Acknowledge by sending back the same message
    send_message(current_msg.message);
}

bool process_serial_messages(void) {
    bool message_received = false;

    while (true) {
        read_message();

        if (current_msg.message == INVALID_MESSAGE) break;

        handle_message();
        message_received = true;
        current_msg.message = INVALID_MESSAGE;
    }

    return message_received;
}
