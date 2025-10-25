#include "sensors/mpu.h"

#include <stdint.h>

#include "hal/spi.h"
#include "logger/logger.h"
#include "math/math.h"
#include "serial/serial_in.h"
#include "timer/time.h"

#define MPU_SAMPLE_RATE 1000  // Hz
#define LBS_PER_DEG 16.4f
#define RAD_PER_SEC (MATH_PI / 180.0f / LBS_PER_DEG)
#define STD_RAD_PER_LBS (RAD_PER_SEC / MPU_SAMPLE_RATE)

#define CALIBRATION_INTERVAL 1.0f  // ms
#define CALIBRATION_SAMPLES 3000

static MpuData mpu_data = {0};
static uint8_t mpu_data_values[TOTAL_REGISTERS] = {0};

static float pv_gyro_x = 0;
static float pv_gyro_y = 0;
static float pv_gyro_z = 0;
static bool integrators_initialized = false;
static uint32_t last_update_time = 0;
static uint32_t current_time = 0;

static inline void update_readings(void) {
    read_registers(ACCEL_REG_X, mpu_data_values, TOTAL_REGISTERS);

    mpu_data.accel_x = (int16_t)(((uint16_t)mpu_data_values[0] << 8) |
                                 (uint16_t)mpu_data_values[1]);
    mpu_data.accel_y = (int16_t)(((uint16_t)mpu_data_values[2] << 8) |
                                 (uint16_t)mpu_data_values[3]);
    mpu_data.accel_z = (int16_t)(((uint16_t)mpu_data_values[4] << 8) |
                                 (uint16_t)mpu_data_values[5]);

    mpu_data.temp = (int16_t)(((uint16_t)mpu_data_values[6] << 8) |
                              (uint16_t)mpu_data_values[7]);

    mpu_data.gyro_x = (int16_t)(((uint16_t)mpu_data_values[8] << 8) |
                                (uint16_t)mpu_data_values[9]);
    mpu_data.gyro_y = (int16_t)(((uint16_t)mpu_data_values[10] << 8) |
                                (uint16_t)mpu_data_values[11]);
    mpu_data.gyro_z = (int16_t)(((uint16_t)mpu_data_values[12] << 8) |
                                (uint16_t)mpu_data_values[13]);

    current_time = time_us();
}

static inline void update_angles(void) {
    const float gyro_x = (float)mpu_data.gyro_x - mpu_data.bias_gyro_x;
    const float gyro_y = (float)mpu_data.gyro_y - mpu_data.bias_gyro_y;
    const float gyro_z = (float)mpu_data.gyro_z - mpu_data.bias_gyro_z;

    if (!integrators_initialized) {
        pv_gyro_x = gyro_x;
        pv_gyro_y = gyro_y;
        pv_gyro_z = gyro_z;
        last_update_time = current_time;
        integrators_initialized = true;
        return;
    }

    const float dt =
        (float)(current_time - last_update_time) * 1e-6f * RAD_PER_SEC;
    if (dt <= 0.0f) return;

    mpu_data.roll += 0.5f * (gyro_x + pv_gyro_x) * dt;
    mpu_data.pitch += 0.5f * (gyro_y + pv_gyro_y) * dt;
    mpu_data.yaw += 0.5f * (gyro_z + pv_gyro_z) * dt;

    normalize_angle(&mpu_data.roll);
    normalize_angle(&mpu_data.pitch);
    normalize_angle(&mpu_data.yaw);

    pv_gyro_x = gyro_x;
    pv_gyro_y = gyro_y;
    pv_gyro_z = gyro_z;
    last_update_time = current_time;
}

bool init_mpu(void) {
    debug_print("Attempting to initialize MPU peripheral...");

    if (!init_spi()) {
        debug_print("Failed to initialize MPU");
        return false;
    }

    mpu_calibrate_gyro();
    debug_print("MPU initialized successfully");
    return true;
}

const MpuData* get_mpu_data(void) { return &mpu_data; }

void update_mpu_data(void) {
    update_readings();
    update_angles();
}

void clear_mpu_data(void) {
    mpu_data = (MpuData){0};

    pv_gyro_x = 0;
    pv_gyro_y = 0;
    pv_gyro_z = 0;
    last_update_time = 0;
    current_time = 0;
    integrators_initialized = false;
}

void restart_mpu(void) {
    mpu_data.accel_x = 0;
    mpu_data.accel_y = 0;
    mpu_data.accel_z = 0;
    mpu_data.temp = 0;
    mpu_data.gyro_x = 0;
    mpu_data.gyro_y = 0;
    mpu_data.gyro_z = 0;
    mpu_data.yaw = 0;
    mpu_data.pitch = 0;
    mpu_data.roll = 0;

    pv_gyro_x = 0;
    pv_gyro_y = 0;
    pv_gyro_z = 0;
    last_update_time = 0;
    current_time = 0;
    integrators_initialized = false;
}

void mpu_calibrate_gyro(void) {
    debug_print("Calibrating MPU gyroscope");

    float sum_gyro_x = 0;
    float sum_gyro_y = 0;
    float sum_gyro_z = 0;

    uint16_t sampled = 0;
    uint32_t sample_time = 0;
    while (sampled < CALIBRATION_SAMPLES) {
        if (!time_elapsed(sample_time, CALIBRATION_INTERVAL)) continue;

        sample_time = time();
        update_readings();

        sum_gyro_x += (float)mpu_data.gyro_x;
        sum_gyro_y += (float)mpu_data.gyro_y;
        sum_gyro_z += (float)mpu_data.gyro_z;
        sampled++;

        process_serial_messages();
    }

    mpu_data.bias_gyro_x = sum_gyro_x / (float)CALIBRATION_SAMPLES;
    mpu_data.bias_gyro_y = sum_gyro_y / (float)CALIBRATION_SAMPLES;
    mpu_data.bias_gyro_z = sum_gyro_z / (float)CALIBRATION_SAMPLES;

    debug_print("Finished MPU gyroscope calibration.");
    debug_print_mpu_gyroscope_biases();

    restart_mpu();
}
