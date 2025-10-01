#ifndef SERIAL_IN_H
#define SERIAL_IN_H

#include <stdbool.h>

/**
 * @brief Processes serial messages waiting in the USART receive buffer.
 * @return true if at least one message was processed, false otherwise.
 * @note When processing messages, it echos back any received message.
 */
bool process_serial_messages(void);

#endif  // SERIAL_IN_H
