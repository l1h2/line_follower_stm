#ifndef SERIAL_BASE_H
#define SERIAL_BASE_H

#include <stdint.h>

/**
 * @brief Macro to define serial messages and their sizes.
 * @param X A macro that takes two parameters: the message name and its size in
 * bytes.
 */
#define SERIAL_MESSAGES_TABLE(X) \
    X(INVALID_MESSAGE, 0)        \
    X(START, 0)                  \
    X(STOP, 0)                   \
    X(STATE, 1)                  \
    X(RUNNING_MODE, 1)           \
    X(STOP_MODE, 1)              \
    X(LAPS, 1)                   \
    X(STOP_TIME, 1)              \
    X(LOG_DATA, 1)               \
    X(PID_KP, 1)                 \
    X(PID_KI, 1)                 \
    X(PID_KD, 2)                 \
    X(PID_KFF, 1)                \
    X(PID_KB, 1)                 \
    X(PID_BASE_PWM, 2)           \
    X(PID_MAX_PWM, 2)            \
    X(TURBINE_PWM, 2)

// Maximum payload size among all messages
#define SERIAL_MESSAGE_MAX_PAYLOAD 4

#define SERIAL_FRAME_START 0xAA  // Start byte for framing

/**
 * @enum SerialMessages
 * @brief Enumeration of serial messages.
 */
typedef enum {
#define X(name, size) name,
    SERIAL_MESSAGES_TABLE(X)
#undef X
        SERIAL_MESSAGE_COUNT
} SerialMessages;

/**
 * @brief Array containing the sizes of each serial message.
 * @note The index corresponds to the SerialMessages enum values.
 */
static const uint8_t SERIAL_MESSAGE_SIZES[SERIAL_MESSAGE_COUNT] = {
#define X(name, size) [name] = (uint8_t)(size),
    SERIAL_MESSAGES_TABLE(X)
#undef X
};

/**
 * @brief Array containing the string names of each serial message.
 * @note The index corresponds to the SerialMessages enum values.
 */
static const char* const SERIAL_MESSAGE_NAMES[SERIAL_MESSAGE_COUNT] = {
#define X(name, size) [name] = #name,
    SERIAL_MESSAGES_TABLE(X)
#undef X
};

/**
 * @struct SerialMessage
 * @brief Structure representing a serial message.
 */
typedef struct {
    SerialMessages message;                       // Message type
    uint8_t size;                                 // Size of the payload
    uint8_t payload[SERIAL_MESSAGE_MAX_PAYLOAD];  // Payload data
} SerialMessage;

/**
 * @brief Create a SerialMessage struct.
 * @param message The type of the serial message.
 * @param payload Pointer to the payload data.
 * @return A SerialMessage struct with the specified type and payload.
 * @note The payload size is determined by the message type. If the message
 * type is invalid, the size will be set to 0 and the payload will be empty.
 */
SerialMessage serial_message(const SerialMessages message,
                             const uint8_t* const payload);

/**
 * @brief Update an existing SerialMessage struct with new payload data.
 * @param msg Pointer to the SerialMessage struct to update.
 * @param payload Pointer to the new payload data.
 */
void update_message(SerialMessage* msg, const SerialMessages type,
                    const uint8_t* const payload);

/**
 * @brief Sends a serial message.
 * @param msg The type of the serial message.
 * @param payload Pointer to the payload data.
 */
void send_data(const SerialMessages msg, const uint8_t* const payload);

/**
 * @brief Sends a serial message from a SerialMessage struct.
 * @param msg Pointer to the SerialMessage struct.
 */
void send_data_from_message(const SerialMessage* msg);

#endif  // SERIAL_BASE_H
