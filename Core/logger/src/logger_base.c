#include "logger/logger_base.h"

#include "hal/usart.h"

static const char HEX_CHARS[] = "0123456789ABCDEF";
static const uint32_t DIVS[10] = {1U,         10U,        100U,     1000U,
                                  10000U,     100000U,    1000000U, 10000000U,
                                  100000000U, 1000000000U};

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

void print_hex(const uint8_t byte) {
    usart_transmit(HEX_CHARS[(byte >> 4) & 0x0F]);  // High nibble
    usart_transmit(HEX_CHARS[byte & 0x0F]);         // Low nibble
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

    for (int8_t i = 9; i >= 0; i--) {
        if (dword < DIVS[i]) continue;

        for (int8_t j = i; j >= 0; j--) {
            usart_transmit('0' + ((dword / DIVS[j]) % 10));
        }
    }
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
