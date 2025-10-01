#include "hal/pwm.h"

#include "main.h"

void init_pwm(void) {
    LL_TIM_EnableCounter(TIM2);

    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH3);
    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH4);
}

void set_pwm(const Motors motor, uint32_t pwm) {
    if (pwm > MAX_PWM) pwm = MAX_PWM;
    if (pwm < MIN_PWM) pwm = MIN_PWM;

    switch (motor) {
        case MOTOR_LEFT:
            LL_TIM_OC_SetCompareCH2(TIM2, pwm);
            break;
        case MOTOR_RIGHT:
            LL_TIM_OC_SetCompareCH3(TIM2, pwm);
            break;
        case MOTOR_TURBINE:
            LL_TIM_OC_SetCompareCH4(TIM2, pwm);
            break;
        default:
            break;
    }
}

uint32_t get_pwm(const Motors motor) {
    switch (motor) {
        case MOTOR_LEFT:
            return LL_TIM_OC_GetCompareCH2(TIM2);
        case MOTOR_RIGHT:
            return LL_TIM_OC_GetCompareCH3(TIM2);
        case MOTOR_TURBINE:
            return LL_TIM_OC_GetCompareCH4(TIM2);
        default:
            return 0;
    }
}

void set_motor_left_dir(const bool forward) {
    if (forward) {
        LL_GPIO_ResetOutputPin(MOTOR_LEFT_IN_1_3_GPIO_Port,
                               MOTOR_LEFT_IN_1_3_Pin);
        LL_GPIO_SetOutputPin(MOTOR_LEFT_IN_2_4_GPIO_Port,
                             MOTOR_LEFT_IN_2_4_Pin);
    } else {
        LL_GPIO_SetOutputPin(MOTOR_LEFT_IN_1_3_GPIO_Port,
                             MOTOR_LEFT_IN_1_3_Pin);
        LL_GPIO_ResetOutputPin(MOTOR_LEFT_IN_2_4_GPIO_Port,
                               MOTOR_LEFT_IN_2_4_Pin);
    }
}

void set_motor_right_dir(const bool forward) {
    if (forward) {
        LL_GPIO_ResetOutputPin(MOTOR_RIGHT_IN_1_3_GPIO_Port,
                               MOTOR_RIGHT_IN_1_3_Pin);
        LL_GPIO_SetOutputPin(MOTOR_RIGHT_IN_2_4_GPIO_Port,
                             MOTOR_RIGHT_IN_2_4_Pin);
    } else {
        LL_GPIO_SetOutputPin(MOTOR_RIGHT_IN_1_3_GPIO_Port,
                             MOTOR_RIGHT_IN_1_3_Pin);
        LL_GPIO_ResetOutputPin(MOTOR_RIGHT_IN_2_4_GPIO_Port,
                               MOTOR_RIGHT_IN_2_4_Pin);
    }
}
