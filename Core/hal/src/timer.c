#include "hal/timer.h"

#include "stm32f4xx_it.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"

void init_system_timer(void) { LL_SYSTICK_EnableIT(); }

uint32_t get_system_time(void) { return LL_GetTick(); }

uint32_t get_system_time_us(void) {
    const uint32_t load = SysTick->LOAD;

    uint32_t ms1 = LL_GetTick();
    uint32_t val = SysTick->VAL;
    const uint32_t ms2 = LL_GetTick();

    if (ms1 != ms2) {
        // SysTick rolled over during read
        val = SysTick->VAL;
        ms1 = ms2;
    }

    return ms1 * 1000UL + (load - val) * 1000UL / (load + 1UL);
}

bool time_elapsed_ms(const uint32_t start, const uint32_t duration) {
    return (LL_GetTick() - start) >= duration;
}

void delay_ms(const uint32_t ms) { LL_mDelay(ms); }
