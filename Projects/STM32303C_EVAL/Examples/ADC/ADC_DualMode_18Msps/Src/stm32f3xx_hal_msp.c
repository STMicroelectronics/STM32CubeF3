/**
  ******************************************************************************
  * @file    ADC/ADC_DualModeInterleaved_18MSamplesPerSecond/Src/stm32f3xx_hal_msp.c
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

/** @defgroup ADC_DualModeInterleaved
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
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *          - Configuration of peripheral clock
  *          - Configuration of GPIO associated to the peripheral channels
  *          - Configuration of DMA
  *          - Configuration of interruptions
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
  static DMA_HandleTypeDef  hdma_adc;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock */
  ADCxy_CHANNEL_GPIO_CLK_ENABLE();
  /* ADCx Periph clock enable */
  ADCx_CLK_ENABLE();
  /* ADCy Periph clock enable */
  ADCy_CLK_ENABLE();
  /* Enable DMA clock */
  DMAxy_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* ADCx and ADCy Channel GPIO pin configuration */
  GPIO_InitStruct.Pin = ADCxy_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCxy_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the DMA channels ##########################################*/
  /* Set the parameters to be configured */
  hdma_adc.Instance = ADCxy_DMA_STREAM;                         /* DMA channel of ADC1 (ADC master) */

  hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;  /* Transfer by half-word to match with ADC resolution 6 or 8 bits */
  hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;     /* Transfer by half-word to match with ADC resolution 6 or 8 bits */
  hdma_adc.Init.Mode = DMA_CIRCULAR;                            /* DMA in circular mode to match with ADC-DMA continuous requests */
  hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;

  HAL_DMA_Init(&hdma_adc);

  /* Associate the initialized DMA handle to the the ADC handle */
  __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(ADCxy_DMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADCxy_DMA_IRQn);
}

/**
  * @brief ADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
  static DMA_HandleTypeDef  hdma_adc;

  /*##-1- Reset peripherals ##################################################*/
  ADCxy_FORCE_RESET();
  ADCxy_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC Channel GPIO pin */
  HAL_GPIO_DeInit(ADCxy_CHANNEL_GPIO_PORT, ADCxy_CHANNEL_PIN);

  /*##-3- Disable the DMA channels ############################################*/
  /* De-Initialize the DMA channel associated to transmission process */
  HAL_DMA_DeInit(&hdma_adc);

  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(ADCxy_DMA_IRQn);
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
