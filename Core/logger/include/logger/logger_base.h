#ifndef LOGGER_BASE_H
#define LOGGER_BASE_H

#include <stdbool.h>
#include <stdint.h>

#define LOG_INTERVAL 1000  // Default log interval

/**
 * @brief Prints a character to the USART.
 * @param c The character to print.
 */
void print_char(const char c);

/**
 * @brief Prints a specific bit of a byte to the USART.
 * @param bit_position The position of the bit to print (0-7).
 * @param byte The byte from which to print the bit.
 */
void print_bit(const uint8_t bit_position, const uint8_t byte);

/**
 * @brief Prints a byte in decimal format to the USART.
 * @param byte The byte to print.
 */
void print_byte(const uint8_t byte);

/**
 * @brief Prints a byte in hexadecimal format to the USART.
 * @param byte The byte to print.
 */
void print_hex(const uint8_t byte);

/**
 * @brief Prints a signed byte in decimal format to the USART.
 * @param byte The byte to print.
 */
void print_signed_byte(int8_t byte);

/**
 * @brief Prints a word (16-bit integer) in decimal format to the USART.
 * @param word The word to print.
 */
void print_word(const uint16_t word);

/**
 * @brief Prints a signed word (16-bit integer) in decimal format to the USART.
 * @param word The word to print.
 */
void print_signed_word(int16_t word);

/**
 * @brief Prints a long (32-bit integer) in decimal format to the USART.
 * @param value The long value to print.
 */
void print_long(const uint32_t dword);

/**
 * @brief Prints a signed long (32-bit integer) in decimal format to the USART.
 * @param value The long value to print.
 */
void print_signed_long(int32_t dword);

/**
 * @brief Prints a byte in binary format to the USART.
 * @param byte The byte to print.
 */
void print_binary(const uint8_t byte);

/**
 * @brief Prints a byte in reverse binary format to the USART.
 * @param byte The byte to print.
 */
void print_reverse_binary(const uint8_t byte);

/**
 * @brief Prints a boolean value to the USART.
 * @param value The boolean value to print.
 */
void print_bool(const bool value);

/**
 * @brief Prints a string to the USART.
 * @param str The string to print.
 * @warning Ensure that the string is null-terminated to avoid undefined
 * behavior.
 * @note This function will block until the entire string is transmitted.
 */
void print_string(const char *str);

/**
 * @brief Prints a newline character to the USART.
 */
void print_new_line(void);

/**
 * @brief Prints a string to the USART.
 * @param str The string to print.
 * @warning Ensure that the string is null-terminated to avoid undefined
 * behavior.
 * @note This function will block until the entire string is transmitted.
 * @note It adds a newline character at the end of the string.
 */
void print(const char *str);

#endif  // LOGGER_BASE_H
