#include "sensors/mpu.h"

#include "hal/spi.h"

static MpuData mpu_data_struct = {0};
static uint8_t mpu_data[TOTAL_REGISTERS] = {0};

const MpuData* get_mpu_data(void) { return &mpu_data_struct; }

void update_mpu_data(void) {
    read_registers(ACCEL_REG_X, mpu_data, TOTAL_REGISTERS);

    mpu_data_struct.accel_x = (mpu_data[0] << 8) | mpu_data[1];
    mpu_data_struct.accel_y = (mpu_data[2] << 8) | mpu_data[3];
    mpu_data_struct.accel_z = (mpu_data[4] << 8) | mpu_data[5];
    mpu_data_struct.temp = (mpu_data[6] << 8) | mpu_data[7];
    mpu_data_struct.gyro_x = (mpu_data[8] << 8) | mpu_data[9];
    mpu_data_struct.gyro_y = (mpu_data[10] << 8) | mpu_data[11];
    mpu_data_struct.gyro_z = (mpu_data[12] << 8) | mpu_data[13];
}

void clear_mpu_data(void) {
    mpu_data_struct.accel_x = 0;
    mpu_data_struct.accel_y = 0;
    mpu_data_struct.accel_z = 0;
    mpu_data_struct.temp = 0;
    mpu_data_struct.gyro_x = 0;
    mpu_data_struct.gyro_y = 0;
    mpu_data_struct.gyro_z = 0;
}
