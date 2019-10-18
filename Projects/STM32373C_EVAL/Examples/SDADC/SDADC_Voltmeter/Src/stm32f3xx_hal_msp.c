/**
  ******************************************************************************
  * @file    SDADC/SDADC_Voltmeter/Src/stm32f3xx_hal_msp.c
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

/** @defgroup SDADC_Voltmeter
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
  * @brief SDADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock
  *           - Peripheral's GPIO
  *           - Peripheral's TIMER
  * @param hsdadc: SDADC handle pointer
  * @retval None
  */
void HAL_SDADC_MspInit(SDADC_HandleTypeDef *hsdadc)
{
  static GPIO_InitTypeDef   gpio;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Enable SDADC clock #################################################*/
  /* Configure Clock prescaler : The SDADC should run @6MHz */
  /* If Sysclk is 72MHz, SDADC divider should be 12 */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDADC;
  PeriphClkInitStruct.SdadcClockSelection  = RCC_SDADCSYSCLK_DIV12;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable SDADC clock */
  POT_SDADC_CLK_ENABLE();

  /*##-2- Power On SDADC  ####################################################*/
  HAL_PWREx_EnableSDADC(POT_PWR_SDADC);

  /*##-3- Enable the SDADC interrupt  ########################################*/
  HAL_NVIC_SetPriority(POT_SDADC_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(POT_SDADC_IRQn);

  /*##-4- Configure GPIO  ####################################################*/
  /* Configure POT_GPIO_PIN_P and POT_GPIO_PIN_N in analog mode*/
  POT_GPIO_CLK_ENABLE();
  gpio.Mode  = GPIO_MODE_ANALOG;
  gpio.Pin   = POT_GPIO_PIN;
  gpio.Speed = GPIO_SPEED_FREQ_LOW;
  gpio.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(POT_SDADC_GPIO_PORT, &gpio);

}

/**
  * @brief SDADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hsdadc: SDADC handle pointer
  * @retval None
  */
void HAL_SDADC_MspDeInit(SDADC_HandleTypeDef *hsdadc)
{
  /*##-1- Power Off SDADC  ###################################################*/
  HAL_PWREx_DisableSDADC(POT_PWR_SDADC);

  /*##-2- Disable power clock  ###############################################*/
  __HAL_RCC_PWR_CLK_DISABLE();

  /*##-3- Power Off SDADC  ###################################################*/
  POT_GPIO_CLK_DISABLE();

  /*##-4- Disable SDADC clock  ###############################################*/
  POT_SDADC_CLK_DISABLE();

  /*##-5- Disable interrupt  #################################################*/
  HAL_NVIC_DisableIRQ(POT_SDADC_IRQn);
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
