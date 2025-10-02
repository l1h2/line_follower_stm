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
    int16_t accel_x;  // Acceleration in X-axis
    int16_t accel_y;  // Acceleration in Y-axis
    int16_t accel_z;  // Acceleration in Z-axis
    int16_t temp;     // Temperature
    int16_t gyro_x;   // Gyroscope in X-axis
    int16_t gyro_y;   // Gyroscope in Y-axis
    int16_t gyro_z;   // Gyroscope in Z-axis
} MpuData;

/**
 * @struct EncoderData
 * @brief Structure to hold the encoder values.
 */
typedef struct {
    int16_t left_encoder;   // Left encoder value
    int16_t right_encoder;  // Right encoder value
    float distance;         // Distance calculated from encoder values
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
