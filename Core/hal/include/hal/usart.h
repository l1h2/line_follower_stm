#ifndef USART_H
#define USART_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initializes the USART peripheral and its interrupts.
 */
void init_usart(void);

/**
 * @brief Handler for USART1 interrupts.
 */
void usart1_irq_handler();

/**
 * @brief Transmits a byte of data over USART.
 * @param data The byte to transmit.
 * @warning This function can block if the transmit buffer is full.
 */
void usart_transmit(const uint8_t data);

/**
 * @brief Reads a byte of data from the USART receive buffer.
 * @return The received byte, or 0 if no data is available.
 */
uint8_t usart_read_char(void);

/**
 * @brief Peeks at the next byte in the USART receive buffer without removing
 * it.
 * @return The next byte, or 0 if no data is available.
 */
uint8_t usart_peek_char(void);

/**
 * @brief Reads multiple bytes from the USART receive buffer into a provided
 * buffer.
 * @param buffer Pointer to the buffer where received data will be stored.
 * @param size The number of bytes to read. If 0, all available data will be
 * read.
 * @note If the requested size exceeds the available data, only the available
 * data will be read.
 * @warning Make sure the provided buffer is large enough to hold the
 * requested data.
 */
void usart_read_buffer(uint8_t* buffer, uint8_t size);

/**
 * @brief Checks how much data is available in the USART receive buffer.
 * @return The number of bytes available.
 */
uint8_t usart_data_available(void);

/**
 * @brief Checks if any data has been received in the USART receive buffer.
 * @return true If data is available, false otherwise.
 */
bool usart_data_received(void);

/**
 * @brief Flushes the USART receive buffer, discarding all received data.
 */
void usart_flush_rx(void);

#endif  // USART_H
