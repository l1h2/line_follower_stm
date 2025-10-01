#include "hal/spi.h"

#include "main.h"

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

static inline void mpu_cs_low(void) {
    LL_GPIO_ResetOutputPin(MPU_NCS_GPIO_Port, MPU_NCS_Pin);
}

static inline void mpu_cs_high(void) {
    LL_GPIO_SetOutputPin(MPU_NCS_GPIO_Port, MPU_NCS_Pin);
}

static uint8_t spi_transfer(const uint8_t data) {
    while (!LL_SPI_IsActiveFlag_TXE(SPI2));
    LL_SPI_TransmitData8(SPI2, data);

    while (!LL_SPI_IsActiveFlag_RXNE(SPI2));
    return LL_SPI_ReceiveData8(SPI2);
}

void init_spi(void) { LL_SPI_Enable(SPI2); }

void read_registers(const uint8_t reg, uint8_t* buffer, const uint8_t bytes) {
    mpu_cs_low();
    spi_transfer(reg | 0x80);

    for (uint8_t i = 0; i < bytes; i++) {
        buffer[i] = spi_transfer(0x00);  // Send dummy byte to receive data
    }

    mpu_cs_high();
}
