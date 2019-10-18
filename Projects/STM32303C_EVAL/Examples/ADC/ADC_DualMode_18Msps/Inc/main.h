/**
  ******************************************************************************
  * @file    ADC/ADC_DualModeInterleaved_18MSamplesPerSecond/Inc/main.h
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
#include "stm32f3xx_hal.h"
#include "stm32303c_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor ADCx instance used and associated
   resources */
/* Definition for ADCx clock resources */
#define ADCx                             ADC1
#define ADCx_CLK_ENABLE()                __HAL_RCC_ADC12_CLK_ENABLE()

/* Definition for ADCy clock resources */
#define ADCy                             ADC2
#define ADCy_CLK_ENABLE()                __HAL_RCC_ADC12_CLK_ENABLE()

#define DMAxy_CLK_ENABLE()               __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCxy_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()

#define ADCxy_FORCE_RESET()              __HAL_RCC_ADC12_FORCE_RESET()
#define ADCxy_RELEASE_RESET()            __HAL_RCC_ADC12_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCxy_CHANNEL_PIN                GPIO_PIN_1
#define ADCxy_CHANNEL_GPIO_PORT          GPIOC

/* Definition for ADCx's Channel */
#define ADCxy_CHANNEL                    ADC_CHANNEL_7

/* Definition for ADCx's DMA */
#define ADCxy_DMA_STREAM                 DMA1_Channel1

/* Definition for ADCx's NVIC */
#define ADCxy_DMA_IRQn                   DMA1_Channel1_IRQn
#define ADCxy_DMA_IRQHandler             DMA1_Channel1_IRQHandler


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
