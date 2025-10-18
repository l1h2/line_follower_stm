#ifndef PID_BASE_H
#define PID_BASE_H

#include <stdint.h>

#include "sensors/sensors_base.h"

/**
 * @struct SpeedErrors
 * @brief Structure to hold speed error values for PID control.
 */
typedef struct {
    float left_target_speed;   // Target speed for the left motor in cm/s
    float right_target_speed;  // Target speed for the right motor in cm/s
    float left_error;          // Error for the left motor in cm/s
    float right_error;         // Error for the right motor in cm/s
    float left_last_error;     // Last error for the left motor in cm/s
    float right_last_error;    // Last error for the right motor in cm/s
    float left_delta_error;    // Delta error for the left motor in cm/s
    float right_delta_error;   // Delta error for the right motor in cm/s
    float left_error_sum;      // Sum of errors for the left motor in cm/s
    float right_error_sum;     // Sum of errors for the right motor in cm/s
} SpeedErrors;

/**
 * @struct ErrorStruct
 * @brief Structure to hold error values for PID control.
 */
typedef struct {
    int8_t error;                     // Current error value.
    int8_t last_error;                // Last error value.
    int8_t delta_error;               // Delta error value.
    int16_t error_sum;                // Sum of errors.
    int8_t feedforward;               // Feedforward value.
    int8_t max_error;                 // Maximum error value.
    int8_t min_error;                 // Minimum error value.
    const SpeedErrors* speed_errors;  // Speed error values.
    const SensorState* sensors;       // Sensor state information.
} ErrorStruct;

/**
 * @struct DeltaPid
 * @brief Structure to hold delta PID control parameters and state.
 */
typedef struct {
    uint8_t kp;               // Proportional gain
    uint8_t ki;               // Integral gain
    uint16_t kd;              // Derivative gain
    float alpha;              // Filter coefficient
    uint16_t clamp;           // Integral windup clamp
    uint32_t frame_interval;  // PID frame interval in ms
    uint32_t last_pid_time;   // Last time the PID was updated
} DeltaPid;

/**
 * @struct BasePwmPid
 * @brief Structure to hold base PWM PID control parameters and state.
 */
typedef struct {
    uint8_t kp;               // Proportional gain
    uint8_t ki;               // Integral gain
    uint16_t kd;              // Derivative gain
    uint8_t kff;              // Feedforward gain
    uint32_t frame_interval;  // PID frame interval in ms
    uint32_t last_pid_time;   // Last time the PID was updated
} BasePwmPid;

/**
 * @struct BaseSpeedPid
 * @brief Structure to hold base speed PID control parameters and state.
 */
typedef struct {
    uint16_t kp;              // Proportional gain
    float ki;                 // Integral gain
    uint16_t kd;              // Derivative gain
    uint16_t kff;             // Feedforward gain
    float base_speed;         // Base speed in cm/s
    uint32_t frame_interval;  // PID frame interval in ms
    uint32_t last_pid_time;   // Last time the PID was updated
} BaseSpeedPid;

/**
 * @struct PidStruct
 * @brief Structure to hold PID control parameters and state.
 */
typedef struct {
    uint16_t base_pwm;               // Base PWM value for the motors
    int16_t current_pwm;             // Current PWM value for the motors
    uint16_t max_pwm;                // Maximum PWM value for the motors
    int16_t min_pwm;                 // Minimum PWM value for the motors
    uint16_t acceleration;           // Acceleration step for PWM changes
    const ErrorStruct* errors;       // Pointer to errors
    const DeltaPid* delta_pid;       // Pointer to Delta PID
    const BasePwmPid* base_pwm_pid;  // Pointer to Base PWM PID
    const BaseSpeedPid* speed_pid;   // Pointer to Base Speed PID
} PidStruct;

#endif  // PID_BASE_H
