/**
  ******************************************************************************
  * @file    TSC/TSC_BasicAcquisition_Polling/Src/stm32f3xx_hal_msp.c
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

/** @defgroup TSC_BasicAcquisition_Polling
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
  * @brief TSC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htsc: TSC handle pointer
  * @retval None
  */
void HAL_TSC_MspInit(TSC_HandleTypeDef *htsc)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /*##-1- Enable TSC and GPIO clocks #########################################*/
  __HAL_RCC_TSC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*##-2- Configure Sampling Capacitor IOs (Alternate-Function Open-Drain) ###*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_TSC;
  /* Shield Sampling Capacitor IO = GROUP7_IO4 = PE5 */
  GPIO_InitStruct.Pin       = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  /* Channel Sampling Capacitor IO = GROUP8_IO4 = PD15 */
  GPIO_InitStruct.Pin       = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  /*##-3- Configure Channel & Shield IOs (Alternate-Function Output PP) ######*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_TSC;
  /* Shield IO = GROUP7_IO3 = PE4 */
  GPIO_InitStruct.Pin       = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  /* Channel1 IO = GROUP8_IO1 = PD12 */
  /* Channel2 IO = GROUP8_IO2 = PD13 - NOT USED IN THIS EXAMPLE */
  /* Channel3 IO = GROUP8_IO3 = PD14 - NOT USED IN THIS EXAMPLE */
  GPIO_InitStruct.Pin       = GPIO_PIN_12;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/**
  * @brief TSC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param htsc: TSC handle pointer
  * @retval None
  */
void HAL_TSC_MspDeInit(TSC_HandleTypeDef *htsc)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_TSC_FORCE_RESET();
  __HAL_RCC_TSC_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the TSC GPIO pins */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12);
  /* HAL_GPIO_DeInit(GPIOD, GPIO_PIN_13); NOT USED IN THIS EXAMPLE */
  /* HAL_GPIO_DeInit(GPIOD, GPIO_PIN_14); NOT USED IN THIS EXAMPLE */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_15);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_5);
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
