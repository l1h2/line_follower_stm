#include "logger/logger_base.h"

#include <stdint.h>

#include "hal/usart.h"

void print_char(const char c) { usart_transmit((uint8_t)c); }

void print_bit(const uint8_t bit_position, const uint8_t byte) {
    if (bit_position > 7) return;  // Invalid bit position

    usart_transmit((byte & (1 << bit_position)) ? '1' : '0');
}

void print_byte(const uint8_t byte) {
    usart_transmit('0' + byte / 100);
    usart_transmit('0' + (byte % 100) / 10);
    usart_transmit('0' + byte % 10);
}

void print_signed_byte(int8_t byte) {
    if (byte & (1 << 7)) {
        usart_transmit('-');
        byte = -byte;
    }
    print_byte(byte);
}

void print_word(const uint16_t word) {
    usart_transmit('0' + (word / 10000));        // Ten-thousands
    usart_transmit('0' + ((word / 1000) % 10));  // Thousands
    usart_transmit('0' + ((word / 100) % 10));   // Hundreds
    usart_transmit('0' + ((word / 10) % 10));    // Tens
    usart_transmit('0' + (word % 10));           // Ones
}

void print_signed_word(int16_t word) {
    if (word & (1 << 15)) {
        usart_transmit('-');
        word = -word;
    }
    print_word(word);
}

void print_long(const uint32_t dword) {
    if (dword == 0) {
        usart_transmit('0');
        return;
    }

    usart_transmit('0' + (dword / 1000000000));        // Billions
    usart_transmit('0' + ((dword / 100000000) % 10));  // Hundred-millions
    usart_transmit('0' + ((dword / 10000000) % 10));   // Ten-millions
    usart_transmit('0' + ((dword / 1000000) % 10));    // Millions
    usart_transmit('0' + ((dword / 100000) % 10));     // Hundred-thousands
    usart_transmit('0' + ((dword / 10000) % 10));      // Ten-thousands
    usart_transmit('0' + ((dword / 1000) % 10));       // Thousands
    usart_transmit('0' + ((dword / 100) % 10));        // Hundreds
    usart_transmit('0' + ((dword / 10) % 10));         // Tens
    usart_transmit('0' + (dword % 10));
}

void print_signed_long(int32_t dword) {
    if (dword < 0) {
        usart_transmit('-');
        dword = -dword;
    }
    print_long((uint32_t)dword);
}

void print_binary(const uint8_t byte) {
    for (int8_t i = 7; i >= 0; i--) {
        usart_transmit((byte & (1 << i)) ? '1' : '0');
    }
}

void print_reverse_binary(const uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        usart_transmit((byte & (1 << i)) ? '1' : '0');
    }
}

void print_bool(const bool value) { usart_transmit(value ? '1' : '0'); }

void print_string(const char *str) {
    while (*str) usart_transmit(*str++);
}

void print_new_line(void) {
    // usart_transmit('\r');
    usart_transmit('\n');
}

void print(const char *str) {
    print_string(str);
    print_new_line();
}
