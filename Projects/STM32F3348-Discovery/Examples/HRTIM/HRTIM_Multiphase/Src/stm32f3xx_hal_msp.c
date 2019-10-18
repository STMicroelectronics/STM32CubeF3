/**
  ******************************************************************************
  * @file    HRTIM\HRTIM_Multiphase/Src/stm32f3xx_hal_msp.c
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

/** @addtogroup HRTIM_Multiphase
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
  GPIO_InitTypeDef GPIO_InitStruct;


  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  
  /* For inputs only (GPIO outputs initialized later in the code */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* HRTIMER clock init */
  /* Use the PLLx2 clock for HRTIM */
  __HAL_RCC_HRTIM1_CONFIG(RCC_HRTIM1CLK_PLLCLK);
  /* Enable HRTIM clock*/
  __HAL_RCC_HRTIM1_CLK_ENABLE();


  /*##-2- Configure peripheral GPIO ##########################################*/
  /* Configure HRTIM input: FAULT1 (PA12) */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;;  
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;;  
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /*##-3- Configure the DMA streams ##########################################*/

  /*##-4- Configure the NVIC #################################################*/
  /* Configure and enable HRTIM MASTER timer interrupt channel in NVIC */
  HAL_NVIC_SetPriority(HRTIM1_Master_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(HRTIM1_Master_IRQn);

}


/**
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
  static DMA_HandleTypeDef  DmaHandle;


  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();

  __HAL_RCC_ADC12_CONFIG(RCC_ADC12PLLCLK_DIV1);
  /* ADC2 Periph clock enable */
  __HAL_RCC_ADC12_CLK_ENABLE();

  /* Enable clock of DMA associated to the peripheral */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* Configure PA2 as analog input */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /*##-3- Configure the DMA streams ##########################################*/
  /* Configure DMA parameters */
  DmaHandle.Instance = DMA1_Channel1;

  DmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
  DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;       /* Transfer from ADC by word to match with ADC resolution 10 or 12 bits */
  DmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;   /* Transfer to memory by half-word to match with buffer variable type: half-word */
  DmaHandle.Init.Mode                = DMA_CIRCULAR;
  DmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;
  
  /* Deinitialize  & Initialize the DMA for new transfer */
  HAL_DMA_DeInit(&DmaHandle);  
  HAL_DMA_Init(&DmaHandle);

  /* Associate the initialized DMA handle to the ADC handle */
  __HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);

  
  /*##-4- Configure the NVIC #################################################*/
  /* NVIC configuration for DMA transfer complete interrupt (not used here) */
//  HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 1, 0);
//  HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
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
