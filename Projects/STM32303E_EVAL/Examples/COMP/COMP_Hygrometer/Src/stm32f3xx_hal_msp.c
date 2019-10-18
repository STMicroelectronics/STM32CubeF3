/**
  ******************************************************************************
  * @file    COMP/COMP_Hygrometer/Src/stm32f3xx_hal_msp.c
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

/** @defgroup COMP_Hygrometer
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
  * @brief  Initializes the DAC MSP.
  * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
  /*## Enable peripheral #####################################################*/
  /* DACx Peripheral clock enable */
  __DACx_CLK_ENABLE();
}

/**
  * @brief  DeInitializes the DAC MSP.
  * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.  
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
  /*##-1- Reset peripherals ##################################################*/
  /* Enable DAC reset state */
  __DACx_FORCE_RESET();
  /* Release DAC from reset state */
  __DACx_RELEASE_RESET();
}


/**
  * @brief TIM MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef      GPIO_InitStruct;
  
  /*## Enable peripherals and GPIO Clocks ####################################*/
  /* Enable GPIO clock */
  HUM_OUT_GPIO_CLK_ENABLE();
  /* TIMx Peripheral clock enable */
  __HAL_RCC_TIM3_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* COMP GPIO pin configuration */
  GPIO_InitStruct.Pin = HUM_OUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = HUM_OUT_AF;
  HAL_GPIO_Init(HUM_OUT_GPIO_PORT, &GPIO_InitStruct);
}


/**
  * @brief DeInitializes the TIM PWM MSP. 
  * @param htim: TIM handle
  * @retval None
  */
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Reset peripherals ##################################################*/
  /* Enable TIM reset state */
  __HAL_RCC_TIM4_FORCE_RESET();
  /* Release TIM from reset state */
  __HAL_RCC_TIM4_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the GPIO pin */
  HAL_GPIO_DeInit(HUM_OUT_GPIO_PORT,HUM_OUT_PIN);

  /* Disable GPIO clock */
  HUM_OUT_GPIO_CLK_DISABLE();
}

/**
  * @brief TIM Input Capture MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  /*## Enable peripherals and GPIO Clocks ####################################*/
  /* TIMx Peripheral clock enable */
  __HAL_RCC_TIM4_CLK_ENABLE();
  
  /*## Configure the NVIC for TIMx ###########################################*/
  HAL_NVIC_SetPriority(TIM4_IRQn,0,0);
  
  /* Enable the TIM4 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
}


/**
  * @brief DeInitializes the TIM Input Capture MSP. 
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Reset peripherals ##################################################*/
  /* Enable TIM reset state */
  __HAL_RCC_TIM4_FORCE_RESET();
  /* Release TIM from reset state */
  __HAL_RCC_TIM4_RELEASE_RESET();
}

/**
  * @brief COMP MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for COMP interrupt request enable
  * @param hcomp: COMP handle pointer
  * @retval None
  */
void HAL_COMP_MspInit(COMP_HandleTypeDef* hcomp)
{
  GPIO_InitTypeDef      GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  HUM_IN_GPIO_CLK_ENABLE();
  /* COMP Periph clock enable via SYSCFG */
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* COMP GPIO pin configuration */
  GPIO_InitStruct.Pin = HUM_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(HUM_IN_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  DeInitializes COMP MSP.
  * @param  hcomp: COMP handle
  * @retval None
  */
void HAL_COMP_MspDeInit(COMP_HandleTypeDef* hcomp)
{
  /*##-1- Reset peripherals ##################################################*/
  /* Warning: this step affects all peripherals controlled via SYSCFG */
  /* so it has a sense if only the current COMP was being used        */
  /* Enable COMP reset state via SYSCFG */
  __HAL_RCC_SYSCFG_FORCE_RESET();
  /* Release COMP from reset state via SYSCFG */
  __HAL_RCC_SYSCFG_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the COMPx GPIO pin */
  HAL_GPIO_DeInit(HUM_IN_GPIO_PORT,HUM_IN_PIN);

  /* Disable GPIO clock */
  HUM_IN_GPIO_CLK_DISABLE();
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
