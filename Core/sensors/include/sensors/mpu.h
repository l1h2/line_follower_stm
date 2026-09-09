#ifndef MPU_H
#define MPU_H

#include <stdbool.h>

#include "sensors/sensors_base.h"

/**
 * @brief Initializes the MPU-9250 sensor.
 * @return true if initialization is successful, false otherwise.
 */
bool init_mpu(void);

/**
 * @brief Returns a pointer to the MPU data structure.
 * @return Pointer to the MPU data structure.
 */
const MpuData* get_mpu_data(void);

/**
 * @brief Reads data from the MPU-9250 sensor and updates the internal data
 * structure.
 */
void update_mpu_data(void);

/**
 * @brief Clears the MPU data structure, setting all values to zero.
 */
void clear_mpu_data(void);

/**
 * @brief Restarts the MPU readings.
 */
void restart_mpu(void);

/**
 * @brief Starts a gyroscope calibration run, clearing the accumulators.
 * @note Must be called before mpu_calibrate_gyro_async().
 */
void start_mpu_gyro_calibration(void);

/**
 * @brief Advances the gyroscope calibration by at most one sample.
 * @return true once the calibration has finished, false while still sampling.
 * @note If the calibration is interrupted, start_mpu_gyro_calibration() must be
 * called again to reset the accumulators.
 */
bool mpu_calibrate_gyro_async(void);

/**
 * @brief Calibrates the gyroscope of the MPU-9250 sensor, blocking until done.
 */
void mpu_calibrate_gyro(void);

#endif  // MPU_H
