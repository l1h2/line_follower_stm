#include "serial/serial_base.h"

#include "hal/usart.h"

static inline uint8_t get_message_size(const SerialMessages message) {
    return (message < SERIAL_MESSAGE_COUNT) ? SERIAL_MESSAGE_SIZES[message] : 0;
}

static inline uint8_t get_checksum(const SerialMessages msg,
                                   const uint8_t* const payload,
                                   const uint8_t size) {
    uint8_t c = 0;
    c ^= (uint8_t)msg;
    for (uint8_t i = 0; i < size; i++) {
        c ^= payload[i];
    }
    return c;
}

SerialMessage serial_message(const SerialMessages message,
                             const uint8_t* const payload) {
    SerialMessage msg;
    msg.message = message;
    msg.size = get_message_size(message);
    for (uint8_t i = 0; i < msg.size; i++) msg.payload[i] = payload[i];

    return msg;
}

void update_message(SerialMessage* msg, const SerialMessages type,
                    const uint8_t* const payload) {
    if (!msg || type >= SERIAL_MESSAGE_COUNT) return;
    msg->message = type;
    msg->size = get_message_size(type);
    for (uint8_t i = 0; i < msg->size; i++) msg->payload[i] = payload[i];
}

void send_data(const SerialMessages msg, const uint8_t* const payload) {
    if (msg >= SERIAL_MESSAGE_COUNT) return;

    usart_transmit(SERIAL_FRAME_START);
    usart_transmit((uint8_t)msg);
    const uint8_t size = get_message_size(msg);
    for (uint8_t i = 0; i < size; i++) usart_transmit(payload[i]);
    usart_transmit(get_checksum(msg, payload, size));
}

void send_data_from_message(const SerialMessage* msg) {
    if (!msg || msg->message >= SERIAL_MESSAGE_COUNT) return;

    usart_transmit(SERIAL_FRAME_START);
    usart_transmit((uint8_t)msg->message);
    for (uint8_t i = 0; i < msg->size; i++) usart_transmit(msg->payload[i]);
    usart_transmit(get_checksum(msg->message, msg->payload, msg->size));
}
