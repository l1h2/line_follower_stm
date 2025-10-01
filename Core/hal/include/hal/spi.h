#ifndef HAL_SPI_H
#define HAL_SPI_H

#include <stdint.h>

#define ACCEL_REG_X 0x3B
#define ACCEL_REG_Y 0x3D
#define ACCEL_REG_Z 0x3F
#define TEMP_REG 0x41
#define GYRO_REG_X 0x43
#define GYRO_REG_Y 0x45
#define GYRO_REG_Z 0x47

#define TOTAL_REGISTERS 14

/**
 * @brief Initialize the SPI peripheral.
 */
void init_spi(void);

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
