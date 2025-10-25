#include "hal/spi.h"

#include "main.h"
#include "timer/time.h"

/**
 * @brief Time for synchronous transmission
 *
 * fc = 24MHz
 * bit_time = 1/fc = 41.67ns
 * byte_time = bit_time * 8 = 333.33ns
 *
 * sent_bytes = 1 (first register address)
 * received_bytes = 14 (TOTAL_REGISTERS)
 *
 * total_time = byte_time * (sent_bytes + received_bytes) = 5us
 *
 * Because of low wait time, no async methods or interrupts are used.
 * The SPI is fast enough to handle the communication in a blocking way.
 */

#define MPU_REG_PWR_MGMT_1 0x6B
#define MPU_REG_WHO_AM_I 0x75
#define MPU_REG_CONFIG 0x1A
#define MPU_ACCEL_CONFIG 0x1C
#define MPU_ACCEL_CONFIG2 0x1D
#define MPU_GYRO_CONFIG 0x1B
#define MPU_REG_SMPLRT_DIV 0x19

#define START_COMMAND 0x80
#define CLOCK_SRC 0x01
#define DLPF_CONFIG 0x01        // ~184Hz
#define DLPF_ACCEL_CONFIG 0x01  // ~184Hz
#define ACCEL_CONFIG 0x00       // +/- 2g
#define GYRO_CONFIG 0x18        // +/- 2000 deg/s

#define MPU_WHO_AM_I_9250_A 0x71
#define MPU_WHO_AM_I_9250_B 0x73

static inline void mpu_cs_low(void) {
    // LL_GPIO_ResetOutputPin(MPU_NCS_GPIO_Port, MPU_NCS_Pin);
    LL_SPI_Enable(SPI2);
}

static inline void mpu_cs_high(void) {
    // LL_GPIO_SetOutputPin(MPU_NCS_GPIO_Port, MPU_NCS_Pin);
    while (LL_SPI_IsActiveFlag_BSY(SPI2));
    LL_SPI_Disable(SPI2);
}

static void spi_write(const uint8_t data) {
    while (!LL_SPI_IsActiveFlag_TXE(SPI2));
    LL_SPI_TransmitData8(SPI2, data);

    while (!LL_SPI_IsActiveFlag_RXNE(SPI2));
    (void)LL_SPI_ReceiveData8(SPI2);
}

static uint8_t spi_read(const uint8_t data) {
    while (!LL_SPI_IsActiveFlag_TXE(SPI2));
    LL_SPI_TransmitData8(SPI2, data);

    while (!LL_SPI_IsActiveFlag_RXNE(SPI2));
    return LL_SPI_ReceiveData8(SPI2);
}

static void mpu_write_register(const uint8_t reg, const uint8_t value) {
    mpu_cs_low();
    spi_write(reg & 0x7F);
    spi_write(value);
    mpu_cs_high();
}

static uint8_t mpu_read_register(const uint8_t reg) {
    mpu_cs_low();
    spi_write(reg | 0x80);
    const uint8_t value = spi_read(0x00);
    mpu_cs_high();

    return value;
}

static bool check_who_am_i(void) {
    const uint8_t who_am_i = mpu_read_register(MPU_REG_WHO_AM_I);
    return (who_am_i == MPU_WHO_AM_I_9250_A || who_am_i == MPU_WHO_AM_I_9250_B);
}

bool init_spi(void) {
    mpu_write_register(MPU_REG_PWR_MGMT_1, START_COMMAND);
    delay(100);

    mpu_write_register(MPU_REG_PWR_MGMT_1, CLOCK_SRC);
    mpu_write_register(MPU_ACCEL_CONFIG, ACCEL_CONFIG);
    mpu_write_register(MPU_GYRO_CONFIG, GYRO_CONFIG);

    mpu_write_register(MPU_REG_SMPLRT_DIV, 0x00);
    mpu_write_register(MPU_ACCEL_CONFIG2, DLPF_ACCEL_CONFIG);
    mpu_write_register(MPU_REG_CONFIG, DLPF_CONFIG);

    for (uint8_t i = 0; i < 5; i++) {
        if (!check_who_am_i()) return false;
        delay(10);
    }

    return check_who_am_i();
}

void read_registers(const uint8_t reg, uint8_t* buffer, const uint8_t bytes) {
    mpu_cs_low();
    spi_write(reg | 0x80);

    for (uint8_t i = 0; i < bytes; i++) {
        buffer[i] = spi_read(0x00);  // Send dummy byte to receive data
    }

    mpu_cs_high();
}
