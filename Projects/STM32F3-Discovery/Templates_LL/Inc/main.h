/**
  ******************************************************************************
  * @file    Templates_LL/Inc/main.h 
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
/* LL drivers common to all LL examples */
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_ll_utils.h"
#include "stm32f3xx_ll_pwr.h"
#include "stm32f3xx_ll_exti.h"
#include "stm32f3xx_ll_gpio.h"
/* LL drivers specific to LL examples IPs */
#include "stm32f3xx_ll_adc.h"
#include "stm32f3xx_ll_comp.h"
#include "stm32f3xx_ll_cortex.h"
#include "stm32f3xx_ll_crc.h"
#include "stm32f3xx_ll_dac.h"
#include "stm32f3xx_ll_dma.h"
#include "stm32f3xx_ll_hrtim.h"
#include "stm32f3xx_ll_i2c.h"
#include "stm32f3xx_ll_iwdg.h"
#include "stm32f3xx_ll_opamp.h"
#include "stm32f3xx_ll_rtc.h"
#include "stm32f3xx_ll_spi.h"
#include "stm32f3xx_ll_tim.h"
#include "stm32f3xx_ll_usart.h"
#include "stm32f3xx_ll_wwdg.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
  * @brief LED3
  */
#define LED3_PIN                           LL_GPIO_PIN_9
#define LED3_GPIO_PORT                     GPIOE
#define LED3_GPIO_CLK_ENABLE()             LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE)

/**
  * @brief LED4
  */
#define LED4_PIN                           LL_GPIO_PIN_8
#define LED4_GPIO_PORT                     GPIOE
#define LED4_GPIO_CLK_ENABLE()             LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE)

/**
  * @brief LED5
  */
#define LED5_PIN                           LL_GPIO_PIN_10
#define LED5_GPIO_PORT                     GPIOE
#define LED5_GPIO_CLK_ENABLE()             LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE)

/**
  * @brief LED6
  */
#define LED6_PIN                           LL_GPIO_PIN_15
#define LED6_GPIO_PORT                     GPIOE
#define LED6_GPIO_CLK_ENABLE()             LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE)

/**
  * @brief LED7
  */
#define LED7_PIN                           LL_GPIO_PIN_11
#define LED7_GPIO_PORT                     GPIOE
#define LED7_GPIO_CLK_ENABLE()             LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE)

/**
  * @brief LED8
  */
#define LED8_PIN                           LL_GPIO_PIN_14
#define LED8_GPIO_PORT                     GPIOE
#define LED8_GPIO_CLK_ENABLE()             LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE)

/**
  * @brief LED9
  */
#define LED9_PIN                           LL_GPIO_PIN_12
#define LED9_GPIO_PORT                     GPIOE
#define LED9_GPIO_CLK_ENABLE()             LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE)

/**
  * @brief LED10
  */
#define LED10_PIN                           LL_GPIO_PIN_13
#define LED10_GPIO_PORT                     GPIOE
#define LED10_GPIO_CLK_ENABLE()             LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE)

/**
  * @brief Key push-button
  */
#define USER_BUTTON_PIN                         LL_GPIO_PIN_0
#define USER_BUTTON_GPIO_PORT                   GPIOA
#define USER_BUTTON_GPIO_CLK_ENABLE()           LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)   
#define USER_BUTTON_EXTI_LINE                   LL_EXTI_LINE_0
#define USER_BUTTON_EXTI_IRQn                   EXTI0_IRQn
#define USER_BUTTON_EXTI_LINE_ENABLE()          LL_EXTI_EnableIT_0_31(USER_BUTTON_EXTI_LINE)   
#define USER_BUTTON_EXTI_FALLING_TRIG_ENABLE()  LL_EXTI_EnableFallingTrig_0_31(USER_BUTTON_EXTI_LINE)   
#define USER_BUTTON_SYSCFG_SET_EXTI()           do {                                                                     \
                                                  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);                  \
                                                  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE0);  \
                                                } while(0)
#define USER_BUTTON_IRQHANDLER                  EXTI0_IRQHandler
/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

/**
  * @brief Toggle periods for various blinking modes
  */
#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
