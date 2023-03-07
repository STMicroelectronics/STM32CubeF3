/**
  ******************************************************************************
  * @file    ADC/ADC_DifferentialMode/Inc/main.h
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
#include "stm32303e_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor ADCx instance under use and associated
   resources */

/* ## Definition of ADC related resources ################################### */
/* Definition of ADCx clock resources */
#define ADCx                            ADC2
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC12_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC12_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC12_RELEASE_RESET()

/* Definition of ADCx channels */
#define ADCx_CHANNEL_DIFF_HIGH            ADC_CHANNEL_1
#define ADCx_CHANNEL_DIFF_LOW            ADC_CHANNEL_2

/* Definition of ADCx channels pins */
#define ADCx_CHANNEL_DIFF_HIGH_PIN        GPIO_PIN_4
#define ADCx_CHANNEL_DIFF_HIGH_GPIO_PORT  GPIOA

#define ADCx_CHANNEL_DIFF_LOW_PIN        GPIO_PIN_5
#define ADCx_CHANNEL_DIFF_LOW_GPIO_PORT  GPIOA

/* ## Definition of DAC related resources ################################### */
/* Definition of DACx clock resources */
#define DACx                            DAC
#define DACx_CLK_ENABLE()               __HAL_RCC_DAC1_CLK_ENABLE()
#define DACx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define DACx_FORCE_RESET()              __HAL_RCC_DAC1_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC1_RELEASE_RESET()

/* Definition of DACx channels */
#define DACx_CHANNEL_TO_ADCx_CHANNEL_DIFF_HIGH            DAC_CHANNEL_1
#define DACx_CHANNEL_TO_ADCx_CHANNEL_DIFF_LOW            DAC_CHANNEL_2

/* Definition of DACx channels pins */
#define DACx_CHANNEL_TO_ADCx_CHANNEL_DIFF_HIGH_PIN        GPIO_PIN_4
#define DACx_CHANNEL_TO_ADCx_CHANNEL_DIFF_HIGH_GPIO_PORT  GPIOA

#define DACx_CHANNEL_TO_ADCx_CHANNEL_DIFF_LOW_PIN        GPIO_PIN_5
#define DACx_CHANNEL_TO_ADCx_CHANNEL_DIFF_LOW_GPIO_PORT  GPIOA




/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
