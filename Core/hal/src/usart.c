#include "hal/usart.h"

#include "stm32f4xx_ll_usart.h"

#define TX_BUFFER_SIZE 256
#define RX_BUFFER_SIZE 256

static volatile uint8_t tx_buf[TX_BUFFER_SIZE];
static volatile uint8_t tx_head = 0;
static volatile uint8_t tx_tail = 0;

static volatile uint8_t rx_buf[RX_BUFFER_SIZE];
static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;

static inline uint8_t next_tx_index(const uint8_t index) {
    return (index + 1) & (TX_BUFFER_SIZE - 1);
}

static inline uint8_t next_rx_index(const uint8_t index) {
    return (index + 1) & (RX_BUFFER_SIZE - 1);
}

static void handle_tx_interrupt(void) {
    if (tx_tail != tx_head) {
        LL_USART_TransmitData8(USART1, tx_buf[tx_tail]);
        tx_tail = next_tx_index(tx_tail);
    } else {
        LL_USART_DisableIT_TXE(USART1);
    }
}

static void handle_rx_interrupt(void) {
    const uint8_t next_head = next_rx_index(rx_head);

    if (next_head == rx_tail) {  // Buffer full, discard data
        (void)LL_USART_ReceiveData8(USART1);
        return;
    }

    rx_buf[rx_head] = LL_USART_ReceiveData8(USART1);
    rx_head = next_head;
}

void init_usart(void) { LL_USART_EnableIT_RXNE(USART1); }

void usart1_irq_handler(void) {
    if (LL_USART_IsEnabledIT_TXE(USART1) && LL_USART_IsActiveFlag_TXE(USART1)) {
        handle_tx_interrupt();
    }

    if (LL_USART_IsActiveFlag_RXNE(USART1)) {
        handle_rx_interrupt();
    }
}

void usart_transmit(const uint8_t data) {
    const uint8_t next_head = next_tx_index(tx_head);
    while (next_head == tx_tail);

    const bool buffer_empty = tx_head == tx_tail;

    tx_buf[tx_head] = data;
    tx_head = next_head;

    if (!buffer_empty || !LL_USART_IsActiveFlag_TXE(USART1)) {
        LL_USART_EnableIT_TXE(USART1);
        return;
    }

    // If buffer_empty prime register to skip first interrupt
    LL_USART_TransmitData8(USART1, data);
    tx_tail = next_tx_index(tx_tail);
}

uint8_t usart_read_char(void) {
    if (rx_head == rx_tail) return 0;

    const uint8_t data = rx_buf[rx_tail];
    rx_tail = next_rx_index(rx_tail);
    return data;
}

uint8_t usart_peek_char(void) {
    if (rx_head == rx_tail) return 0;
    return rx_buf[rx_tail];
}

void usart_read_buffer(uint8_t* buffer, uint8_t size) {
    const uint8_t data_size =
        (rx_head - rx_tail + RX_BUFFER_SIZE) & (RX_BUFFER_SIZE - 1);

    if (size > data_size || size == 0) size = data_size;

    for (uint8_t i = 0; i < size; i++) {
        buffer[i] = rx_buf[rx_tail];
        rx_tail = next_rx_index(rx_tail);
    }
}

uint8_t usart_data_available(void) {
    return (rx_head - rx_tail + RX_BUFFER_SIZE) & (RX_BUFFER_SIZE - 1);
}

bool usart_data_received(void) { return rx_head != rx_tail; }

void usart_flush_rx(void) {
    LL_USART_DisableIT_RXNE(USART1);
    rx_head = 0;
    rx_tail = 0;
    LL_USART_EnableIT_RXNE(USART1);
}
