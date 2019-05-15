/**
  ******************************************************************************
  * @file    OPAMP/OPAMP_CALIBRATION/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
/* User can use this section to tailor COMPx instance under use and associated
   resources */

/* ## Definition of OPAMP related resources ################################### */
/* Definition of OPAMPx clock resources */
#define OPAMPx                            OPAMP1
#define OPAMPx_CLK_ENABLE()               __HAL_RCC_OPAMP_CLK_ENABLE()
#define OPAMPx_NON_INVERTING_INPUT_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define OPAMPx_OUTPUT_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()

#define OPAMPx_FORCE_RESET()              __HAL_RCC_OPAMP_FORCE_RESET()
#define OPAMPx_RELEASE_RESET()            __HAL_RCC_OPAMP_RELEASE_RESET()
   
/* Definition of OPAMPx channels pins */
#define OPAMP1_NON_INVERTING_INPUT_PIN        GPIO_PIN_5
#define OPAMP1_NON_INVERTING_INPUT_GPIO_PORT  GPIOA
#define OPAMP1_OUTPUT_PIN                     GPIO_PIN_2
#define OPAMP1_OUTPUT_GPIO_PORT               GPIOA
   
/* ## Definition of DAC related resources ################################### */
/* Definition of DACx clock resources */
#define DACx                            DAC
#define DACx_CLK_ENABLE()               __HAL_RCC_DAC1_CLK_ENABLE()
#define DACx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define DACx_FORCE_RESET()              __HAL_RCC_DAC1_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC1_RELEASE_RESET()

/* Definition of DACx channels */
#define DACx_CHANNELa                   DAC_CHANNEL_2

/* Definition of DACx channels pins */
#define DACx_CHANNELa_PIN               GPIO_PIN_5
#define DACx_CHANNELa_GPIO_PORT         GPIOA

/* Definition of DACx DMA resources */
#define DACx_CHANNELa_DMA_CLK_ENABLE()           __HAL_RCC_DMA2_CLK_ENABLE()
#define DACx_CHANNELa_DMA                        DMA2_Channel4

#define DACx_CHANNELa_DMA_IRQn                   DMA2_Channel4_IRQn
#define DACx_CHANNELa_DMA_IRQHandler             DMA2_Channel4_IRQHandler


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
