#ifndef SERIAL_OUT_H
#define SERIAL_OUT_H

#include <stdint.h>

#include "pid/pid_base.h"
#include "serial/serial_base.h"
#include "state_machine/state_machine_base.h"
#include "track/track_base.h"

/**
 * @brief Initializes the serial output module with references to the state
 * machine, PID controller, and track counters.
 * @param state_machine Pointer to the StateMachine struct.
 * @param pid_struct Pointer to the PidStruct struct.
 * @param track_counters Pointer to the TrackCounters struct.
 */
void init_serial_out(const StateMachine* const state_machine,
                     const PidStruct* const pid_struct,
                     const TrackCounters* const track_counters);

/**
 * @brief Sends a message over the serial interface.
 * @param msg The message to send.
 */
void send_message(const SerialMessages msg);

/**
 * @brief Sends multiple messages over the serial interface.
 * @param msgs Pointer to an array of messages to send.
 * @param count Number of messages in the array.
 */
void send_messages(const SerialMessages* msgs, const uint8_t count);

/**
 * @brief Sends all messages over the serial interface.
 * @note This function skips command signals like START and STOP.
 */
void send_all_messages(void);

/**
 * @brief Sends all messages asynchronously at defined intervals.
 */
void send_all_messages_async(const uint32_t interval);

#endif  // SERIAL_OUT_H
