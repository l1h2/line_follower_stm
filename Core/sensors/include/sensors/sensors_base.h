#ifndef SENSORS_BASE_H
#define SENSORS_BASE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @struct IrSensorData
 * @brief Structure to hold the state of the infrared sensors.
 */
typedef struct {
    uint8_t central_sensors_state;  // The state of the central sensors.
    bool left_sensor;   // Flag to indicate if the left sensor is active.
    bool right_sensor;  // Flag to indicate if the right sensor is active.
    uint8_t total_central_sensors;  // Total number of central sensors.
} IrSensorData;

/**
 * @struct MpuData
 * @brief Structure to hold the MPU-9250 sensor data.
 */
typedef struct {
    int16_t accel_x;    // Acceleration in X-axis
    int16_t accel_y;    // Acceleration in Y-axis
    int16_t accel_z;    // Acceleration in Z-axis
    int16_t temp;       // Temperature
    int16_t gyro_x;     // Gyroscope in X-axis
    int16_t gyro_y;     // Gyroscope in Y-axis
    int16_t gyro_z;     // Gyroscope in Z-axis
    float bias_gyro_x;  // Gyroscope bias in X-axis
    float bias_gyro_y;  // Gyroscope bias in Y-axis
    float bias_gyro_z;  // Gyroscope bias in Z-axis
    float yaw;          // Yaw angle in radians [-π, π]
    float pitch;        // Pitch angle in radians [-π, π]
    float roll;         // Roll angle in radians [-π, π]
} MpuData;

/**
 * @struct EncoderData
 * @brief Structure to hold the encoder values.
 */
typedef struct {
    int32_t left_encoder;          // Left encoder count
    int32_t right_encoder;         // Right encoder count
    float current_left_distance;   // Left distance since last update in cm
    float current_right_distance;  // Right distance since last update in cm
    float current_distance;        // Distance since last update in cm
    float left_distance;           // Left distance in cm
    float right_distance;          // Right distance in cm
    float distance;                // Distance in cm
    float current_angle;           // Current angle in radians
    float heading;                 // Estimated heading in radians [-π, π]
    float left_speed;              // Speed of the left wheel in cm/s
    float right_speed;             // Speed of the right wheel in cm/s
    float speed;                   // Average speed of the robot in cm/s
    float current_interval;        // Time interval since last update in seconds
    uint32_t last_update_time;     // Timestamp of the last update in ms
    float effective_wheel_base;    // Effective wheel base value in cm
    float wheel_base_correction;   // Wheel base correction factor
} EncoderData;

/**
 * @struct SensorState
 * @brief Structure to hold the state of the peripheral sensors.
 */
typedef struct {
    const IrSensorData* ir_sensors;  // Pointer to the infrared sensor data
    const MpuData* mpu_data;         // Pointer to the MPU data
    const EncoderData* encoders;     // Pointer to the encoder data
} SensorState;

#endif  // SENSORS_BASE_H
