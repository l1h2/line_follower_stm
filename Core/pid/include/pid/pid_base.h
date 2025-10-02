#ifndef PID_BASE_H
#define PID_BASE_H

#include <stdint.h>

#include "sensors/sensors_base.h"

/**
 * @struct ErrorStruct
 * @brief Structure to hold error values for PID control.
 */
typedef struct {
    int8_t error;                // The current error value.
    int8_t last_error;           // The last error value.
    int8_t delta_error;          // The delta error value.
    int16_t error_sum;           // The sum of errors.
    int8_t feedforward;          // The feedforward value.
    uint8_t error_weight;        // The weight of the error for PID control.
    int8_t max_error;            // Maximum error value.
    int8_t min_error;            // Minimum error value.
    const SensorState* sensors;  // Pointer to sensor state information.
} ErrorStruct;

/**
 * @struct DeltaPid
 * @brief Structure to hold delta PID control parameters and state.
 */
typedef struct {
    uint8_t kp;               // Proportional gain
    uint8_t ki;               // Integral gain
    uint16_t kd;              // Derivative gain
    uint32_t frame_interval;  // PID frame interval
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
    uint32_t frame_interval;  // PID frame interval
    uint32_t last_pid_time;   // Last time the PID was updated
} BasePwmPid;

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
    const ErrorStruct* errors;       // Pointer to the error structure
    const DeltaPid* delta_pid;       // Pointer to the Delta PID structure
    const BasePwmPid* base_pwm_pid;  // Pointer to the Base PWM PID structure
} PidStruct;

#endif  // PID_BASE_H
