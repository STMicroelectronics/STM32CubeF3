/**
  ******************************************************************************
  * @file    ADC/ADC_Conv_Differential/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32303c_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor ADCx instance used and associated
   resources */
   
/* ## Definition of ADC related constants ################################### */
/* Definition of ADCx clock resources */
#define ADCx                            ADC2
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC12_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC12_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC12_RELEASE_RESET()

/* Definition of ADCx Channel Pin */
#define ADCx_CHANNEL_PIN_DIFF_HIGH      GPIO_PIN_4
#define ADCx_CHANNEL_GPIO_PORT          GPIOA

/* Definition of ADCx Channel Pin */
#define ADCx_CHANNEL_PIN_DIFF_LOW       GPIO_PIN_5
#define ADCx_CHANNEL_GPIO_PORT          GPIOA

/* Definition of ADCx Channel */
#define ADCx_CHANNEL_DIFF_HIGH          ADC_CHANNEL_1

/* Definition of ADCx Channel */
#define ADCx_CHANNEL_DIFF_LOW           ADC_CHANNEL_2

/* Definition of ADCx's NVIC */
#define ADCx_IRQn                       ADC1_2_IRQn
#define ADCx_IRQHandler                 ADC1_2_IRQHandler

/* ## Definition of EXTI related constants ################################## */
#define __HAL_RCC_GPIOx_CLK_ENABLE()            __HAL_RCC_GPIOE_CLK_ENABLE()
     
/* ## Definition of DAC related constants ################################### */
/* Definition for DACx clock resources */
#define DACx                            DAC
#define DACx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define DACx_CLK_ENABLE()               __HAL_RCC_DAC1_CLK_ENABLE()
#define DACx_FORCE_RESET()              __HAL_RCC_DAC1_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC1_RELEASE_RESET()

/* Definition for DACx Channel Pin */
#define DACx_CHANNEL_1_PIN              GPIO_PIN_4
#define DACx_CHANNEL_1_GPIO_PORT        GPIOA

#define DACx_CHANNEL_2_PIN              GPIO_PIN_5
#define DACx_CHANNEL_2_GPIO_PORT        GPIOA
     
/* Definition for DACx's Channel */     
#define DACx_CHANNEL_TO_ADCx_CHANNEL_DIFF_HIGH  DAC_CHANNEL_1
#define DACx_CHANNEL_TO_ADCx_CHANNEL_DIFF_LOW   DAC_CHANNEL_2



/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
