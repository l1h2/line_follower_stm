/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin LL_GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define MOTOR_RIGHT_IN_2_4_Pin LL_GPIO_PIN_14
#define MOTOR_RIGHT_IN_2_4_GPIO_Port GPIOC
#define MOTOR_RIGHT_IN_1_3_Pin LL_GPIO_PIN_15
#define MOTOR_RIGHT_IN_1_3_GPIO_Port GPIOC
#define MOTOR_LEFT_PWM_Pin LL_GPIO_PIN_1
#define MOTOR_LEFT_PWM_GPIO_Port GPIOA
#define MOTOR_RIGHT_PWM_Pin LL_GPIO_PIN_2
#define MOTOR_RIGHT_PWM_GPIO_Port GPIOA
#define TURBINE_PWM_Pin LL_GPIO_PIN_3
#define TURBINE_PWM_GPIO_Port GPIOA
#define SENSOR_IR_RIGHT_Pin LL_GPIO_PIN_4
#define SENSOR_IR_RIGHT_GPIO_Port GPIOA
#define SENSOR_IR_LEFT_Pin LL_GPIO_PIN_5
#define SENSOR_IR_LEFT_GPIO_Port GPIOA
#define MOTOR_RIGHT_ENCODER_C2_Pin LL_GPIO_PIN_6
#define MOTOR_RIGHT_ENCODER_C2_GPIO_Port GPIOA
#define MOTOR_RIGHT_ENCODER_C1_Pin LL_GPIO_PIN_7
#define MOTOR_RIGHT_ENCODER_C1_GPIO_Port GPIOA
#define SENSOR_IR_0_Pin LL_GPIO_PIN_0
#define SENSOR_IR_0_GPIO_Port GPIOB
#define SENSOR_IR_1_Pin LL_GPIO_PIN_1
#define SENSOR_IR_1_GPIO_Port GPIOB
#define SENSOR_IR_2_Pin LL_GPIO_PIN_10
#define SENSOR_IR_2_GPIO_Port GPIOB
#define MPU_NCS_Pin LL_GPIO_PIN_12
#define MPU_NCS_GPIO_Port GPIOB
#define MPU_SCL_Pin LL_GPIO_PIN_13
#define MPU_SCL_GPIO_Port GPIOB
#define MPU_ADO_Pin LL_GPIO_PIN_14
#define MPU_ADO_GPIO_Port GPIOB
#define MPU_SDA_Pin LL_GPIO_PIN_15
#define MPU_SDA_GPIO_Port GPIOB
#define SENSOR_IR_INPUT_Pin LL_GPIO_PIN_8
#define SENSOR_IR_INPUT_GPIO_Port GPIOA
#define BLUETOOTH_TX_Pin LL_GPIO_PIN_9
#define BLUETOOTH_TX_GPIO_Port GPIOA
#define BLUETOOTH_RX_Pin LL_GPIO_PIN_10
#define BLUETOOTH_RX_GPIO_Port GPIOA
#define MOTOR_LEFT_IN_2_4_Pin LL_GPIO_PIN_11
#define MOTOR_LEFT_IN_2_4_GPIO_Port GPIOA
#define MOTOR_LEFT_IN_1_3_Pin LL_GPIO_PIN_12
#define MOTOR_LEFT_IN_1_3_GPIO_Port GPIOA
#define SENSOR_IR_3_Pin LL_GPIO_PIN_3
#define SENSOR_IR_3_GPIO_Port GPIOB
#define SENSOR_IR_4_Pin LL_GPIO_PIN_4
#define SENSOR_IR_4_GPIO_Port GPIOB
#define SENSOR_IR_5_Pin LL_GPIO_PIN_5
#define SENSOR_IR_5_GPIO_Port GPIOB
#define MOTOR_LEFT_ENCODER_C2_Pin LL_GPIO_PIN_6
#define MOTOR_LEFT_ENCODER_C2_GPIO_Port GPIOB
#define MOTOR_LEFT_ENCODER_C1_Pin LL_GPIO_PIN_7
#define MOTOR_LEFT_ENCODER_C1_GPIO_Port GPIOB
#define SENSOR_IR_6_Pin LL_GPIO_PIN_8
#define SENSOR_IR_6_GPIO_Port GPIOB
#define SENSOR_IR_7_Pin LL_GPIO_PIN_9
#define SENSOR_IR_7_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
