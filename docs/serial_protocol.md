# Serial Protocol (line_follower_stm)

This document describes the lightweight serial protocol used between the controller (PC/mobile) and the robot.

## Contents

- [Overview](#overview)
- [Scope](#scope)
- [Packet Structure](#packet-structure)
  - [Byte Diagrams](#byte-diagrams)
- [Payload Definitions](#payload-definitions)
- [Messages](#messages)
  - [Operation Data](#operation-data)
  - [Acknowledgment](#acknowledgment)
- [Timing and Performance](#timing-and-performance)
- [Examples](#examples)

## Overview

The robot uses a custom lightweight serial communication protocol for data exchange between the robot and a controller application via `USART`. This protocol is implemented in the [serial module](../Core/serial) and provides functions for sending and receiving commands and data packets.

All currently supported commands and packets can be seen in the [serial protocol header file](../Core/serial/include/serial/serial_base.h), which includes commands for starting/stopping the robot, requesting status information, and sending configuration parameters.

## Scope

This protocol is intended for use with the line_follower_stm robot platform. It defines the format and types of messages exchanged between the robot and the controller app. For this purpose it assumes that a `USART` serial interface is established between both ends.

Even though the robot is capable of communicating at any baud rate, the default configuration for the `STM32` firmware and the `HC-05` Bluetooth module is set to `115200 bps`, `8 data bits`, `1 stop bit` and `no parity`. This configuration provides optimized message times considering the usual operation loops of the robot.

## Packet Structure

Since the protocol uses the same `USART` interface as the standard logs, it implements a simple mechanism to differentiate between log messages and protocol messages. This is done by using a specific start byte (`0xAA`) for protocol messages, allowing the receiver to identify and parse them correctly.

The protocol defines static sizes for all available messages to reduce average packet sizes and simplify parsing. This means that size is not sent along with the data, so both the sender and receiver must know the expected size of each message type in advance.

For extra reliability, the protocol also includes a simple checksum mechanism to verify the integrity of received messages. The checksum is calculated by performing a bitwise `XOR` operation on all bytes of the message, including the message type and payload. The calculated checksum is then compared to the checksum byte included in the received message to ensure data integrity.

For received messages, since the robot only allows for protocol messages to be received, and it doesn't share the `USART` stream with other loggers, the protocol implementation assumes that all incoming data is part of a protocol message. Therefore, it doesn't implement any special handling to differentiate between log messages and protocol messages for incoming data, nor performs checksum verification.

### Byte Diagrams

Robot → Controller

|       Offset | Field    | Size | Description                                               |
| -----------: | :------- | :--: | :-------------------------------------------------------- |
|            0 | Start    |  1   | Start byte 0xAA (identifies protocol message)             |
|            1 | Msg ID   |  1   | Message identifier (defines payload length N)             |
| 2 .. (2+N-1) | Payload  |  N   | Message payload (N bytes; N predefined per Msg ID)        |
|        (2+N) | Checksum |  1   | XOR of Msg ID and all payload bytes (start byte excluded) |

Controller → Robot

| Offset | Field   | Size | Description                                        |
| -----: | :------ | :--: | :------------------------------------------------- |
|      0 | Msg ID  |  1   | Message identifier (defines payload length N)      |
| 1 .. N | Payload |  N   | Message payload (N bytes; N predefined per Msg ID) |

- No start byte or checksum used for incoming messages; payload length N is known from the message ID.

## Payload Definitions

For payload data the protocol uses `little-endian` byte order for multi-byte values. However, for floating-point values (`float`), the protocol uses a reduced 2 byte representation by sending the values as `int16_t` after multiplying them by a predefined scale factor. This approach reduces the amount of data transmitted while maintaining sufficient precision for the robot's operation. This causes a reduced range and precision for floating-point values:

- **Unsigned float**:

| Precision (decimal places) | Range (0 — max) |
| :------------------------: | --------------: |
|             0              |      0 — 65,535 |
|             1              |     0 — 6,553.5 |
|             2              |      0 — 655.35 |
|             3              |      0 — 65.535 |
|             4              |      0 — 6.5535 |

- **Signed float**:

| Precision (decimal places) | Range (-max — +max) |
| :------------------------: | ------------------: |
|             0              |   -32,767 — +32,767 |
|             1              | -3,276.7 — +3,276.7 |
|             2              |   -327.67 — +327.67 |
|             3              |   -32.767 — +32.767 |
|             4              |   -3.2767 — +3.2767 |

The parsing functions can be found in [serial_out.c](../Core/serial/src/serial_out.c#L19) for sent messages and [serial_in.c](../Core/serial/src/serial_in.c#L45) for received messages.

## Messages

The protocol defines a set of messages for communication between the controller and the robot. Each message has a unique identifier, a predefined payload size, and a specific data type for its payload. The following table summarizes the available messages:

| Message         |  Id | Payload Size | Data Type  | Description                     | Obs                                    |
| --------------- | --: | -----------: | :--------- | ------------------------------- | :------------------------------------- |
| INVALID_MESSAGE |   0 |            0 | N/A        | Invalid/unknown message         | —                                      |
| PING            |   1 |            0 | N/A        | Keep-alive / ping               | —                                      |
| START           |   2 |            0 | N/A        | Start signal                    | —                                      |
| STOP            |   3 |            0 | N/A        | Stop signal                     | —                                      |
| STATE           |   4 |            1 | uint8_t    | Robot state from state machine  | enum value                             |
| RUNNING_MODE    |   5 |            1 | uint8_t    | Running mode                    | enum value                             |
| STOP_MODE       |   6 |            1 | uint8_t    | Stop mode                       | enum value                             |
| LAPS            |   7 |            1 | uint8_t    | Stop after laps                 | lap count                              |
| STOP_TIME       |   8 |            1 | uint8_t    | Stop after time                 | seconds                                |
| STOP_DISTANCE   |   9 |            2 | uint16_t   | Stop after distance             | centimeters                            |
| LOG_DATA        |  10 |            1 | uint8_t    | Enable/disable operation logs   | boolean (0/1)                          |
| PID_KP          |  11 |            1 | uint8_t    | PID proportional gain           | -                                      |
| PID_KI          |  12 |            1 | uint8_t    | PID integral gain               | -                                      |
| PID_KD          |  13 |            2 | uint16_t   | PID derivative gain             | -                                      |
| PID_KB          |  14 |            1 | uint8_t    | Base PWM PID break factor       | Kp for Base PWM                        |
| PID_KFF         |  15 |            1 | uint8_t    | Base PWM PID feedforward gain   | -                                      |
| PID_ALPHA       |  16 |            2 | float      | PID filter alpha for Kd         | 0 - 100%, with 2 decimal places        |
| PID_CLAMP       |  17 |            2 | uint16_t   | PID clamp limit                 | -                                      |
| PID_ACCEL       |  18 |            2 | uint16_t   | Base PWM PID acceleration limit | -                                      |
| PID_BASE_PWM    |  19 |            2 | uint16_t   | Base PWM value                  | PWM units (0 - 1000)                   |
| PID_MAX_PWM     |  20 |            2 | uint16_t   | Base PWM max value              | PWM units (0 - 1000)                   |
| TURBINE_PWM     |  21 |            2 | uint16_t   | Turbine PWM value               | PWM units (0 - 1000)                   |
| SPEED_KP        |  22 |            2 | uint16_t   | Speed PID proportional gain     | -                                      |
| SPEED_KI        |  23 |            2 | float      | Speed PID integral gain         | 4 decimal places                       |
| SPEED_KD        |  24 |            2 | uint16_t   | Speed PID derivative gain       | -                                      |
| SPEED_KFF       |  25 |            2 | uint16_t   | Speed feedforward gain          | -                                      |
| BASE_SPEED      |  26 |            2 | float      | Base speed value                | cm/s, with 2 decimal places            |
| LOOKAHEAD       |  27 |            1 | uint8_t    | Pure-pursuit lookahead distance | centimeters                            |
| CURVATURE_GAIN  |  28 |            2 | float      | Wheel base correction           | 0 - 3, with 2 decimal places           |
| IMU_ALPHA       |  29 |            2 | float      | IMU filter alpha                | 0 - 100%, with 2 decimal places        |
| OPERATION_DATA  |  30 |            8 | uint8_t[8] | Operation/telemetry data packet | composite telemetry struct (see below) |

These messages can be used to change the robot's configuration, control its operation, and retrieve status information.

### Operation Data

The `OPERATION_DATA` message (ID 30) is used to send a composite telemetry data packet from the robot to the controller. This packet contains various operational parameters and sensor readings that provide insight into the robot's current state during operation. The payload structure for this message is defined as follows:

| Offset | Field    | Size | Description                      | Obs                                               |
| -----: | :------- | :--: | :------------------------------- | :------------------------------------------------ |
|      0 | IR State |  1   | Central IR sensors state bitmask | Bits 0..7: central sensors (0 = black, 1 = white) |
|      1 | Flags    |  1   | Left/right IR + reserved bits    | Bit 0: Left; Bit 1: Right; Bits 2–7: reserved     |
|      2 | X Pos    |  2   | X position                       | in mm                                             |
|      4 | Y Pos    |  2   | Y position                       | in mm                                             |
|      6 | Heading  |  2   | Heading angle                    | in radians [-π, π], with 4 decimal places         |

- The IR State field uses a bitmask to represent the state of the central infrared sensors, where each bit corresponds to a specific sensor from left to right at a top-down view of the robot.
- The Flags field uses individual bits to represent the state of the left and right infrared sensors, with additional bits reserved for future use.

The parsing and construction of this message can be found in [serial_out.c](../Core/serial/src/serial_out.c#L29).

### Acknowledgment

After receiving any message the robot responds with an echo of the same message containing the updated value or state to acknowledge the command. This allows the controller to verify that the command was received and processed correctly.

## Timing and Performance

The protocol is designed to be lightweight and efficient, minimizing the overhead of communication while ensuring reliable data exchange. The use of fixed-size messages and a simple checksum mechanism helps to reduce processing time and complexity.

At a baud rate of `115200`, the time taken to transmit a single byte is approximately `86.8 µs`. Considering every message exchanged in the `USART` protocol needs to send `1 starting bit` and the configuration for this implementation is `8 data bits`, `no parity bit`, and `1 stop bit`, the total transmission time for each message can be calculated based on its size:

- **Sent messages**: `T_sent = (N + 3) * 86.8 µs`
- **Received messages**: `T_received = (N + 1) * 86.8 µs`

Where N is the payload size in bytes.

| Message        | Payload Size (N) | Sent Time (µs) | Received Time (µs) |
| -------------- | ---------------: | -------------: | -----------------: |
| PING           |                0 |          260.4 |               86.8 |
| START          |                0 |          260.4 |               86.8 |
| STOP           |                0 |          260.4 |               86.8 |
| STATE          |                1 |          347.2 |              173.6 |
| RUNNING_MODE   |                1 |          347.2 |              173.6 |
| STOP_MODE      |                1 |          347.2 |              173.6 |
| LAPS           |                1 |          347.2 |              173.6 |
| STOP_TIME      |                1 |          347.2 |              173.6 |
| STOP_DISTANCE  |                2 |          434.0 |              260.4 |
| LOG_DATA       |                1 |          347.2 |              173.6 |
| PID_KP         |                1 |          347.2 |              173.6 |
| PID_KI         |                1 |          347.2 |              173.6 |
| PID_KD         |                2 |          434.0 |              260.4 |
| PID_KB         |                1 |          347.2 |              173.6 |
| PID_KFF        |                1 |          347.2 |              173.6 |
| PID_ALPHA      |                2 |          434.0 |              260.4 |
| PID_CLAMP      |                2 |          434.0 |              260.4 |
| PID_ACCEL      |                2 |          434.0 |              260.4 |
| PID_BASE_PWM   |                2 |          434.0 |              260.4 |
| PID_MAX_PWM    |                2 |          434.0 |              260.4 |
| TURBINE_PWM    |                2 |          434.0 |              260.4 |
| SPEED_KP       |                2 |          434.0 |              260.4 |
| SPEED_KI       |                2 |          434.0 |              260.4 |
| SPEED_KD       |                2 |          434.0 |              260.4 |
| SPEED_KFF      |                2 |          434.0 |              260.4 |
| BASE_SPEED     |                2 |          434.0 |              260.4 |
| LOOKAHEAD      |                1 |          347.2 |              173.6 |
| CURVATURE_GAIN |                2 |          434.0 |              260.4 |
| IMU_ALPHA      |                2 |          434.0 |              260.4 |
| OPERATION_DATA |                8 |          954.8 |              781.2 |

The robot is configured to handle `USART` transmissions asynchronously using interrupts and ring buffers as seen in [usart.c](../Core/hal/src/usart.c), allowing it to process incoming and outgoing messages without blocking its main operation loop. However, to ensure no messages are skipped during transmission, once the buffer is full, the sending function will block until there is space available in the buffer to add the new data. This means that if the buffer fills up faster than it flushes data, the sending function may introduce delays to the main program flow.

Considering the main operation loops of the robot run at a `1 kHz` frequency (every `1 ms`), the hardware is able to send on average up to `11 bytes/ms` (~11.52 bytes/ms) at `115200` baud. This means that as long as the total size of messages sent within each loop does not exceed `11 bytes`, there will be no delays introduced by the sending function.

It is important to remember that the protocol shares the same `USART` interface as the standard logs, so the total amount of data sent (logs + protocol messages) must be considered when calculating the average data rate to avoid delays. Preferentially, log messages should be avoided entirely inside high-frequency loops to ensure no delays occur.

## Examples

Here are some examples of how to construct and parse messages using the defined protocol.

### Example 1: Sending a START command

To send a START command from the controller to the robot, the message would be constructed as follows:

| Offset | Field  | Value | Description          |
| -----: | :----- | :---- | :------------------- |
|      0 | Msg ID | 0x02  | Message ID for START |

The complete byte sequence to send would be: `0x02`

### Example 2: Receiving a START command acknowledgment

When the robot receives the START command, it responds with an acknowledgment message. The received message would be:

| Offset | Field    | Value | Description           |
| -----: | :------- | :---- | :-------------------- |
|      0 | Start    | 0xAA  | Start byte            |
|      1 | Msg ID   | 0x02  | Message ID for START  |
|      2 | Checksum | 0x02  | Checksum (0x02 XOR 0) |

The complete byte sequence received would be: `0xAA 0x02 0x02`

### Example 3: Receiving OPERATION_DATA

When the robot sends an OPERATION_DATA message, the received message would be structured as follows:

| Offset | Field    | Value     | Description                                                                             |
| -----: | :------- | :-------- | :-------------------------------------------------------------------------------------- |
|      0 | Start    | 0xAA      | Start byte                                                                              |
|      1 | Msg ID   | 0x1E      | Message ID for OPERATION_DATA                                                           |
|      2 | IR State | 0x5A      | Example IR sensor state bitmask                                                         |
|      3 | Flags    | 0x03      | Example flags (left/right IR)                                                           |
|      4 | X Pos    | 0x10 0x27 | Example X position (10000 mm)                                                           |
|      6 | Y Pos    | 0x20 0x4E | Example Y position (20000 mm)                                                           |
|      8 | Heading  | 0x30 0x75 | Example heading (30000 in 0.0001 radians)                                               |
|     10 | Checksum | 0x4C      | Checksum (0x1E XOR 0x5A XOR 0x03 XOR 0x10 XOR 0x27 XOR 0x20 XOR 0x4E XOR 0x30 XOR 0x75) |

The complete byte sequence received would be: `0xAA 0x1E 0x5A 0x03 0x10 0x27 0x20 0x4E 0x30 0x75 0x4C`

### Example 4: Sending a PID_KP update

To send a PID_KP update from the controller to the robot, the message would be constructed as follows:

| Offset | Field   | Value | Description           |
| -----: | :------ | :---- | :-------------------- |
|      0 | Msg ID  | 0x0B  | Message ID for PID_KP |
|      1 | Payload | 0x32  | New PID_KP value (50) |

The complete byte sequence to send would be: `0x0B 0x32`

### Example 5: Receiving a PID_KP update acknowledgment

When the robot receives the PID_KP update, it responds with an acknowledgment message. The received message would be:

| Offset | Field    | Value | Description               |
| -----: | :------- | :---- | :------------------------ |
|      0 | Start    | 0xAA  | Start byte                |
|      1 | Msg ID   | 0x0B  | Message ID for PID_KP     |
|      2 | Payload  | 0x32  | Acknowledged PID_KP value |
|      3 | Checksum | 0x39  | Checksum (0x0B XOR 0x32)  |

The complete byte sequence received would be: `0xAA 0x0B 0x32 0x39`
