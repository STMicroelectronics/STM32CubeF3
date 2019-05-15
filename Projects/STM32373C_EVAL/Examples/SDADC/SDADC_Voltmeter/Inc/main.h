/**
  ******************************************************************************
  * @file    SDADC/SDADC_Voltmeter/Inc/main.h
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
#include "stdio.h"
#include "string.h"
#include "stm32f3xx_hal.h"
#include "stm32373c_eval.h"
#include "stm32373c_eval_lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Messages to be displayed on LCD */
#define MESSAGE1        "      Voltmeter     "

/* A potentiometer (POT) is connected to PB1 which corresponds to SDADC1 channel 5P */
#define POT_SDADC_CLK_ENABLE  __HAL_RCC_SDADC1_CLK_ENABLE
#define POT_SDADC_CLK_DISABLE __HAL_RCC_SDADC1_CLK_DISABLE
#define POT_PWR_SDADC         PWR_SDADC_ANALOG1
#define POT_SDADC_IRQn        SDADC1_IRQn
#define POT_SDADC_IRQHandler  SDADC1_IRQHandler
#define POT_SDADC             SDADC1
#define POT_SDADC_GPIO_PORT   GPIOB
#define POT_GPIO_CLK_ENABLE   __HAL_RCC_GPIOB_CLK_ENABLE
#define POT_GPIO_CLK_DISABLE  __HAL_RCC_GPIOB_CLK_DISABLE
#define POT_GPIO_PIN          GPIO_PIN_1
#define POT_SDADC_VREF        SDADC_VREF_EXT /* Select External reference */
#define POT_SDADC_GAIN        SDADC_GAIN_1   /* Select Internal gain = 4 */
#define SDADC_GAIN            (uint32_t) 1   /* SDADC internal gain is set to 4: update this define
                                                    according to POT_SDADC_GAIN */
#define POT_SDADC_CHANNEL     SDADC_CHANNEL_5
#define SDADC_RESOL           (uint32_t) 65535 /* 2e16 - 1 */
#define SDADC_INIT_TIMEOUT    30 /* about two SDADC clock cycles after INIT is set */
#define SDADC_CAL_TIMEOUT     4*30720 /*  ~5.12 ms at 6 MHz  in a single calibration sequence */
#define SDADC_VREF            (float) 3300  /* SDADC external reference is set to 3.3V */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
