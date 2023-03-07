/**
  ******************************************************************************
  * @file    TIM/TIM_InputCapture/Inc/main.h
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
#include "stm32373c_eval.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* User can use this section to tailor TIMx instance used and associated
   resources */
/* Definition for TIMx clock resources */
#define TIMx                           TIM4
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM4_CLK_ENABLE()

/* Definition for TIMx Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define TIMx_GPIO_PORT                 GPIOB
#define TIMx_GPIO_PIN_CHANNEL2         GPIO_PIN_7
#define TIMx_GPIO_AF_TIMx              GPIO_AF2_TIM4

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM4_IRQn
#define TIMx_IRQHandler                TIM4_IRQHandler


/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */
