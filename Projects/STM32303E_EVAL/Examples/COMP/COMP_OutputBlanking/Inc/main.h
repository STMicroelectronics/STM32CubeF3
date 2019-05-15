/**
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/Inc/main.h 
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
#include "stm32303e_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor COMPx instance used and associated 
   resources */
/* Definition for COMPx clock resources */
#define COMPx                           COMP1
#define COMPx_CLK_ENABLE()              __HAL_RCC_SYSCFG_CLK_ENABLE()
#define COMPx_INPUT_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define COMPx_OUTPUT_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define COMPx_FORCE_RESET()             __HAL_RCC_SYSCFG_FORCE_RESET()
#define COMPx_RELEASE_RESET()           __HAL_RCC_SYSCFG_RELEASE_RESET()

/* Definition for COMPx Channel Pin */
#define COMPx_INPUT_PIN                 GPIO_PIN_1
#define COMPx_OUTPUT_PIN                GPIO_PIN_0
#define COMPx_OUTPUT_AF                 GPIO_AF8_COMP1
#define COMPx_INPUT_GPIO_PORT           GPIOA 
#define COMPx_OUTPUT_GPIO_PORT          GPIOA 

/* Definition for TIMx clock resources */
#define TIMx                            TIM1
#define TIMx_CLK_ENABLE()               __HAL_RCC_TIM1_CLK_ENABLE()
#define TIMx_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()

/* Definition for TIMx Pin */
#define TIMx_PIN                        GPIO_PIN_9
#define TIMx_AF                         GPIO_AF6_TIM1
#define TIMx_GPIO_PORT                  GPIOA 

#define TIMx_FORCE_RESET()              __HAL_RCC_TIM1_FORCE_RESET()
#define TIMx_RELEASE_RESET()            __HAL_RCC_TIM1_RELEASE_RESET()
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
