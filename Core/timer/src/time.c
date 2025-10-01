#include "timer/time.h"

#include "hal/timer.h"

uint32_t time(void) { return get_system_time(); }

uint32_t time_us(void) { return get_system_time_us(); }

bool time_elapsed(const uint32_t start, const uint32_t duration) {
    return time_elapsed_ms(start, duration);
}

bool time_elapsed_us(const uint32_t start, const uint32_t duration) {
    return (get_system_time_us() - start) >= duration;
}

void delay(const uint32_t ms) { delay_ms(ms); }

void delay_us(const uint32_t us) {
    const uint32_t start = get_system_time_us();
    while ((get_system_time_us() - start) < us);
}
