#ifndef LOGGER_H
#define LOGGER_H

#include "config.h"
#include "logger_base.h"

#ifdef DEBUG_MODE
#include "logger_debug.h"
#else
#define debug_print_char(c) ((void)0)
#define debug_print_bit(bit_position, byte) ((void)0)
#define debug_print_byte(byte) ((void)0)
#define debug_print_hex(byte) ((void)0)
#define debug_print_signed_byte(byte) ((void)0)
#define debug_print_word(word) ((void)0)
#define debug_print_signed_word(word) ((void)0)
#define debug_print_long(dword) ((void)0)
#define debug_print_signed_long(dword) ((void)0)
#define debug_print_binary(byte) ((void)0)
#define debug_print_bool(value) ((void)0)
#define debug_print_string(str) ((void)0)
#define debug_print_new_line() ((void)0)
#define debug_print(str) ((void)0)
#define debug_print_central_ir_sensors() ((void)0)
#define debug_print_ir_sensors() ((void)0)
#define debug_print_errors() ((void)0)
#define debug_print_mpu_data() ((void)0)
#define debug_print_diagnostics() ((void)0)
#endif  // DEBUG_MODE

#endif  // LOGGER_H
