/**
  ******************************************************************************
  * @file    Examples_LL/HRTIM/HRTIM_BuckBoost/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_ll_utils.h"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_exti.h"
#include "stm32f3xx_ll_pwr.h"
#include "stm32f3xx_ll_cortex.h"
#include "stm32f3xx_ll_adc.h"
#include "stm32f3xx_ll_hrtim.h"
#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
typedef enum {
  LED3 = 0,
  LED4 = 1,
  LED5 = 2,
  LED6 = 3,

  LED_GREEN  = LED5,
  LED_ORANGE = LED4, 
  LED_RED    = LED3, 
  LED_BLUE   = LED6
} Led_t;

typedef enum {
  DEMO_STATE_BUCK = 0,
  DEMO_STATE_BOOST,
  DEMO_STATE_DE_ENERGIZE,
  DEMO_STATE_FAULT
} DemoState_t;

/* Exported constants --------------------------------------------------------*/
/**
  * @brief LED3 
  */
#define LED3_PIN               LL_GPIO_PIN_6
#define LED3_GPIO_PORT         GPIOB
#define LED3_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)  

/**
  * @brief LED4 
  */
#define LED4_PIN               LL_GPIO_PIN_8
#define LED4_GPIO_PORT         GPIOB
#define LED4_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)  

/**
  * @brief LED5 
  */
#define LED5_PIN               LL_GPIO_PIN_9
#define LED5_GPIO_PORT         GPIOB
#define LED5_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)  

/**
  * @brief LED6 
  */
#define LED6_PIN               LL_GPIO_PIN_7
#define LED6_GPIO_PORT         GPIOB
#define LED6_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)  

/**
  * @brief User push-button
  */
#define USER_BUTTON_PIN               LL_GPIO_PIN_0
#define USER_BUTTON_GPIO_PORT         GPIOA
#define USER_BUTTON_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)

/**
  * @brief ADC1_IN2 
  */
#define ADC1_IN2_PIN               LL_GPIO_PIN_1
#define ADC1_IN2_GPIO_PORT         GPIOA
#define ADC1_IN2_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)  

/**
  * @brief ADC1_IN4 
  */
#define ADC1_IN4_PIN               LL_GPIO_PIN_3
#define ADC1_IN4_GPIO_PORT         GPIOA
#define ADC1_IN4_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA) 

/**
  * @brief HRTIM1_CHA1 
  */
#define HRTIM1_CHA1_PIN               LL_GPIO_PIN_8
#define HRTIM1_CHA1_GPIO_PORT         GPIOA
#define HRTIM1_CHA1_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA) 

/**
  * @brief HRTIM1_CHA2 
  */
#define HRTIM1_CHA2_PIN               LL_GPIO_PIN_9
#define HRTIM1_CHA2_GPIO_PORT         GPIOA
#define HRTIM1_CHA2_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)  

/**
  * @brief HRTIM1_CHB1 
  */
#define HRTIM1_CHB1_PIN               LL_GPIO_PIN_10
#define HRTIM1_CHB1_GPIO_PORT         GPIOA
#define HRTIM1_CHB1_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)  

/**
  * @brief HRTIM1_CHB2 
  */
#define HRTIM1_CHB2_PIN               LL_GPIO_PIN_11
#define HRTIM1_CHB2_GPIO_PORT         GPIOA
#define HRTIM1_CHB2_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)  

/**
  * @brief HRTIM1_FLT1 
  */
#define HRTIM1_FLT1_PIN               LL_GPIO_PIN_12
#define HRTIM1_FLT1_GPIO_PORT         GPIOA
#define HRTIM1_FLT1_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)  

/**
  * @brief Toggle periods for various blinking modes
  */
#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000


#define BUCK_PWM_PERIOD ((uint16_t)18432) // 250kHz

/* Exported macro ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
extern DemoState_t DemoState;


/* Exported functions ------------------------------------------------------- */
extern void LED_Init(Led_t Led);
extern void LED_On(Led_t Led);
extern void LED_Off(Led_t Led);
extern void LED_Toggle(Led_t Led);
extern void LED_Toggle(Led_t Led);
extern void LED_Blinking(uint32_t Period);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
