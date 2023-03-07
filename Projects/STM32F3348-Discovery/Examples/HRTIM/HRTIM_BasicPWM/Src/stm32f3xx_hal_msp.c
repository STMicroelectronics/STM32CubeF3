/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_BasicPWM/Src/stm32f3xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F3xx_HAL_Examples
  * @{
  */

/** @addtogroup HRTIM_BasicPWM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
* @brief  HAL_HRTIM_MspInit
* @param  None
* @retval None
*/
void HAL_HRTIM_MspInit(HRTIM_HandleTypeDef * hhrtim)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
  
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_HRTIM1;
  PeriphClkInit.Hrtim1ClockSelection = RCC_HRTIM1CLK_PLLCLK;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
  
  /* Enable HRTIM clock*/
  __HAL_RCC_HRTIM1_CLK_ENABLE();

}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
