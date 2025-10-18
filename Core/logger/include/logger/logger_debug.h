#ifndef LOGGER_DEBUG_H
#define LOGGER_DEBUG_H

#include <stdbool.h>
#include <stdint.h>

#include "sensors/sensors_base.h"

/**
 * @brief Debug wrapper for the print_char function.
 * @param c The character to print.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_char(const char c);

/**
 * @brief Debug wrapper for the print_bit function.
 * @param bit_position The position of the bit to print.
 * @param byte The byte containing the bit to print.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_bit(const uint8_t bit_position, const uint8_t byte);

/**
 * @brief Debug wrapper for the print_byte function.
 * @param byte The byte to print.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_byte(const uint8_t byte);

/**
 * @brief Debug wrapper for the print_hex function.
 * @param byte The byte to print in hexadecimal format.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_hex(const uint8_t byte);

/**
 * @brief Debug wrapper for the print_signed_byte function.
 * @param byte The signed byte to print.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_signed_byte(int8_t byte);

/**
 * @brief Debug wrapper for the print_word function.
 * @param word The word to print.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_word(const uint16_t word);

/**
 * @brief Debug wrapper for the print_signed_word function.
 * @param word The signed word to print.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_signed_word(int16_t word);

/**
 * @brief Debug wrapper for the print_long function.
 * @param dword The long value to print.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_long(const uint32_t dword);

/**
 * @brief Debug wrapper for the print_signed_long function.
 * @param dword The signed long value to print.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_signed_long(int32_t dword);

/**
 * @brief Debug wrapper for the print_float function.
 * @param value The float value to print.
 * @param decimal_places Number of decimal places to print (0-4).
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_float(float value, uint8_t decimal_places);

/**
 * @brief Debug wrapper for the print_binary function.
 * @param byte The byte to print in binary format.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_binary(const uint8_t byte);

/**
 * @brief Debug wrapper for the print_bool function.
 * @param value The boolean value to print.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_bool(const bool value);

/**
 * @brief Debug wrapper for the print_string function.
 * @param str The string to print.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_string(const char* str);

/**
 * @brief Debug wrapper for the print_new_line function.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print_new_line(void);

/**
 * @brief Debug wrapper for the print function.
 * @param str The string to print.
 * @note This function can be used to add print statements that won't be
 * compiled without the DEBUG_MODE directive.
 */
void debug_print(const char* str);

/**
 * @brief Prints the current state of the central IR sensors in a human-readable
 * format.
 */
void debug_print_central_ir_sensors(void);

/**
 * @brief Prints detailed information about all IR sensors (central and side) in
 * a human-readable format.
 */
void debug_print_ir_sensors(void);

/**
 * @brief Prints the current error states, including the error byte, current
 * error, last error, and error sum.
 */
void debug_print_errors(void);

/**
 * @brief Prints the MPU-9250 sensor data, including accelerometer, gyroscope,
 * and temperature readings.
 */
void debug_print_mpu_data(void);

/**
 * @brief Prints the encoder pulse counts for the left, right, and totals.
 */
void debug_print_encoder_pulses(void);

/**
 * @brief Prints the encoder distances for the left, right, and totals.
 */
void debug_print_encoder_distances(void);

/**
 * @brief Prints the encoder speeds for the left, right, and average.
 */
void debug_print_encoder_speeds(void);

/**
 * @brief Prints the encoder data, including left and right encoder values.
 */
void debug_print_encoder_data(void);

/**
 * @brief Prints periodic diagnostics information.
 * @return true if diagnostics were printed, false otherwise.
 * @note This function uses a predefined interval (LOG_INTERVAL) to limit
 * the frequency of logs.
 * @note This function also updates all data used in the diagnostics
 * printout. , so it doesn't need to be called in a separate update
 * function.
 */
bool debug_print_diagnostics(void);

#endif  // LOGGER_DEBUG_H
