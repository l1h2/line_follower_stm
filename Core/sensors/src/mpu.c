#include "sensors/mpu.h"

#include <stdint.h>

#include "hal/spi.h"
#include "logger/logger.h"

static MpuData mpu_data = {0};
static uint8_t mpu_data_values[TOTAL_REGISTERS] = {0};

void init_mpu(void) {
    debug_print("Attempting to initialize MPU peripheral...");

    if (init_spi()) {
        debug_print("MPU initialized successfully");
    } else {
        debug_print("Failed to initialize MPU");
    }
}

const MpuData* get_mpu_data(void) { return &mpu_data; }

void update_mpu_data(void) {
    read_registers(ACCEL_REG_X, mpu_data_values, TOTAL_REGISTERS);

    mpu_data.accel_x = (mpu_data_values[0] << 8) | mpu_data_values[1];
    mpu_data.accel_y = (mpu_data_values[2] << 8) | mpu_data_values[3];
    mpu_data.accel_z = (mpu_data_values[4] << 8) | mpu_data_values[5];
    mpu_data.temp = (mpu_data_values[6] << 8) | mpu_data_values[7];
    mpu_data.gyro_x = (mpu_data_values[8] << 8) | mpu_data_values[9];
    mpu_data.gyro_y = (mpu_data_values[10] << 8) | mpu_data_values[11];
    mpu_data.gyro_z = (mpu_data_values[12] << 8) | mpu_data_values[13];
}

void clear_mpu_data(void) {
    mpu_data.accel_x = 0;
    mpu_data.accel_y = 0;
    mpu_data.accel_z = 0;
    mpu_data.temp = 0;
    mpu_data.gyro_x = 0;
    mpu_data.gyro_y = 0;
    mpu_data.gyro_z = 0;
}
