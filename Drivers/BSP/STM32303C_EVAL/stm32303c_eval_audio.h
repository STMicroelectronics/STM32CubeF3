/**
  ******************************************************************************
  * @file    stm32303c_eval_audio.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the 
  *          stm32303c_eval_audio.c driver.
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
#ifndef __STM32303C_EVAL_AUDIO_H
#define __STM32303C_EVAL_AUDIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Include AUDIO component driver */
#include "../Components/cs42l52/cs42l52.h"   
#include "stm32303c_eval.h"
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32303C_EVAL
  * @{
  */
    
/** @addtogroup STM32303C_EVAL_AUDIO
  * @{
  */    

/** @defgroup STM32303C_EVAL_AUDIO_Exported_Types Exported Types
  * @{
  */
typedef enum 
{
  AUDIO_OK       = 0x00,
  AUDIO_ERROR    = 0x01,
  AUDIO_TIMEOUT  = 0x02

}AUDIO_StatusTypeDef;
  
/**
  * @}
  */

/** @defgroup STM32303C_EVAL_AUDIO_Exported_Constants Exported Constants
  * @{
  */ 

/* Audio Codec hardware I2C address */ 
#define AUDIO_I2C_ADDRESS                       0x94

/*----------------------------------------------------------------------------
             AUDIO OUT CONFIGURATION
  ----------------------------------------------------------------------------*/

/* I2S peripheral configuration defines */
#define I2Sx                                    SPI3
#define I2Sx_CLK_ENABLE()                       __HAL_RCC_SPI3_CLK_ENABLE()
#define I2Sx_CLK_DISABLE()                      __HAL_RCC_SPI3_CLK_DISABLE()
#define I2Sx_FORCE_RESET()                      __HAL_RCC_SPI3_FORCE_RESET()
#define I2Sx_RELEASE_RESET()                    __HAL_RCC_SPI3_RELEASE_RESET()

#define I2Sx_WS_PIN                             GPIO_PIN_4
#define I2Sx_MCK_PIN                            GPIO_PIN_9
#define I2Sx_SCK_PIN                            GPIO_PIN_10
#define I2Sx_DIN_PIN                            GPIO_PIN_12

#define I2Sx_WS_GPIO_PORT                       GPIOA
#define I2Sx_MCK_GPIO_PORT                      GPIOA
#define I2Sx_SCK_DIN_GPIO_PORT                  GPIOC
#define I2Sx_MCK_WS_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define I2Sx_MCK_WS_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()
#define I2Sx_WS_AF                              GPIO_AF6_SPI3
#define I2Sx_MCK_AF                             GPIO_AF5_SPI3
#define I2Sx_SCK_DIN_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOC_CLK_ENABLE()
#define I2Sx_SCK_DIN_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOC_CLK_DISABLE()
#define I2Sx_SCK_DIN_AF                         GPIO_AF6_SPI3

/* I2S DMA Stream definitions */
#define I2Sx_DMAx_CLK_ENABLE()                  __HAL_RCC_DMA2_CLK_ENABLE()
#define I2Sx_DMAx_CLK_DISABLE()                 __HAL_RCC_DMA2_CLK_DISABLE()
#define I2Sx_DMAx_CHANNEL                       DMA2_Channel2
#define I2Sx_DMAx_IRQ                           DMA2_Channel2_IRQn
#define I2Sx_DMAx_PERIPH_DATA_SIZE              DMA_PDATAALIGN_HALFWORD
#define I2Sx_DMAx_MEM_DATA_SIZE                 DMA_MDATAALIGN_HALFWORD
#define DMA_MAX_SZE                             0xFFFF

/* Select the interrupt preemption priority and subpriority for the DMA interrupt */
#define AUDIO_OUT_IRQ_PREPRIO                   ((uint32_t)0x0E)   /* Select the preemption priority level(0 is the highest) */   
#define AUDIO_OUT_IRQ_SUBPRIO                   0   /* Select the sub-priority level (0 is the highest) */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                    OPTIONAL Configuration defines parameters
------------------------------------------------------------------------------*/
#define AUDIODATA_SIZE        2   /* 16-bits audio data size */

#define DMA_MAX(_X_)          (((_X_) <= DMA_MAX_SZE)? (_X_):DMA_MAX_SZE)
/**
  * @}
  */ 

/** @defgroup STM32303C_EVAL_AUDIO_Exported_Variables Exported Variables
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup STM32303C_EVAL_AUDIO_Exported_Macros Exported Macros
  * @{
  */ 
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup STM32303C_EVAL_AUDIO_Exported_Functions Exported Functions
  * @{
  */
uint8_t        BSP_AUDIO_OUT_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
uint8_t        BSP_AUDIO_OUT_Play(uint16_t* pBuffer, uint32_t Size);
uint8_t        BSP_AUDIO_OUT_ChangeBuffer(uint16_t *pData, uint16_t Size);
uint8_t        BSP_AUDIO_OUT_Pause(void);
uint8_t        BSP_AUDIO_OUT_Resume(void);
uint8_t        BSP_AUDIO_OUT_Stop(uint32_t Option);
uint8_t        BSP_AUDIO_OUT_SetVolume(uint8_t Volume);
uint8_t        BSP_AUDIO_OUT_SetFrequency(uint32_t AudioFreq);
uint8_t        BSP_AUDIO_OUT_SetMute(uint32_t Command);
uint8_t        BSP_AUDIO_OUT_SetOutputMode(uint8_t Output);

/* User Callbacks: user has to implement these functions in his code if they are needed. */
/* This function is called when the requested data has been completely transferred.*/
void     BSP_AUDIO_OUT_TransferComplete_CallBack(void);

/* This function is called when half of the requested buffer has been transferred. */
void     BSP_AUDIO_OUT_HalfTransfer_CallBack(void);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void     BSP_AUDIO_OUT_Error_CallBack(void);
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* __STM32303C_EVAL_AUDIO_H */

