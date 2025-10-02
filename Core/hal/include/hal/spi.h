#ifndef HAL_SPI_H
#define HAL_SPI_H

#include <stdbool.h>
#include <stdint.h>

#define ACCEL_REG_X 0x3B  // Starting register for accelerometer data
#define ACCEL_REG_Y 0x3D  // Register for accelerometer Y data
#define ACCEL_REG_Z 0x3F  // Register for accelerometer Z data
#define TEMP_REG 0x41     // Register for temperature data
#define GYRO_REG_X 0x43   // Starting register for gyroscope data
#define GYRO_REG_Y 0x45   // Register for gyroscope Y data
#define GYRO_REG_Z 0x47   // Register for gyroscope Z data

#define TOTAL_REGISTERS 14  // Total registers to read for accel, temp, gyro

/**
 * @brief Initialize the SPI peripheral.
 * @return true if initialization was successful, false otherwise.
 */
bool init_spi(void);

/**
 * @brief Read multiple registers from the MPU-6050 sensor via SPI.
 *
 * @param reg The starting register address to read from.
 * @param buffer Pointer to the buffer where the read data will be stored.
 * @param bytes The number of bytes to read.
 * @warning Ensure the buffer is large enough to hold the requested number of
 * bytes.
 */
void read_registers(const uint8_t reg, uint8_t* buffer, const uint8_t bytes);

#endif  // HAL_SPI_H
