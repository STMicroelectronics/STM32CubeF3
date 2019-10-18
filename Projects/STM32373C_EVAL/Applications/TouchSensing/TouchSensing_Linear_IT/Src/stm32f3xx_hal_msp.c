/**
  ******************************************************************************
  * @file    TouchSensing\TouchSensing_Linear_IT\Src\stm32f3xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F3xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
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
  * @brief  Initializes the TSC MSP.
  * @param  None
  * @retval None
  */
void HAL_TSC_MspInit(TSC_HandleTypeDef* htsc)
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
  /* Shield Sampling Capacitor IO = TSC_GROUP7_IO4 = PE5 */
  GPIO_InitStruct.Pin       = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  /* Channel Sampling Capacitor IO = TSC_GROUP8_IO4 = PD15 */
  GPIO_InitStruct.Pin       = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  /*##-3- Configure Channel & Shield IOs (Alternate-Function Output PP) ######*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_TSC;
  /* Shield IO = TSC_GROUP7_IO3 = PE4 */
  GPIO_InitStruct.Pin       = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  /* Channel IO1 = TSC_GROUP8_IO1 = PD12 */
  /* Channel IO2 = TSC_GROUP8_IO2 = PD13 */
  /* Channel IO3 = TSC_GROUP8_IO3 = PD14 */
  GPIO_InitStruct.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*##-4- Configure the synchronization pin in Alternate-Function mode #######*/
  /* Not used in this example. Keep it for reference.
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF3_TSC;
  GPIO_InitStruct.Pin       = GPIO_PIN_8;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); */
  
  /*##-5- Configure the NVIC for TSC #########################################*/
  HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);
}

/**
  * @brief  DeInitializes the TSC MSP.
  * @param  None  
  * @retval None
  */
void HAL_TSC_MspDeInit(TSC_HandleTypeDef* htsc)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_TSC_FORCE_RESET();
  __HAL_RCC_TSC_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the TSC GPIO pins */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4);
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_5);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_13);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_14);
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_15);

  /*##-3- Disable the NVIC for TSC ###########################################*/
  HAL_NVIC_DisableIRQ(EXTI2_TSC_IRQn);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
