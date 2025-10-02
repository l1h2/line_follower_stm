#include "hal/encoders.h"

#include "stm32f4xx_ll_tim.h"

void init_encoder_counters(void) {
    LL_TIM_EnableCounter(TIM3);
    LL_TIM_EnableCounter(TIM4);
}

void set_encoder_left(const int16_t value) { LL_TIM_SetCounter(TIM3, value); }

void set_encoder_right(const int16_t value) { LL_TIM_SetCounter(TIM4, value); }

void set_encoder_values(const int16_t left, const int16_t right) {
    set_encoder_left(left);
    set_encoder_right(right);
}

void set_encoders(const int16_t value) {
    set_encoder_left(value);
    set_encoder_right(value);
}

int16_t get_encoder_left(void) { return LL_TIM_GetCounter(TIM3); }

int16_t get_encoder_right(void) { return LL_TIM_GetCounter(TIM4); }

void get_encoder_values(int16_t *left, int16_t *right) {
    if (left) *left = get_encoder_left();
    if (right) *right = get_encoder_right();
}
