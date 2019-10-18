/**
  ******************************************************************************
  * @file    SDADC/SDADC_PressureMeasurement/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#define MESSAGE1   "     Pressure       "
#define MESSAGE2   "Measurement MPX2102A"

/* Pressure sensor MPX2102 is connected to PE8 and PE9 which corresponds to SDADC1 channel 8:
   Refer to STM32373C-EVAL user manual for more details about how the MPX2102 is
   connected to PE8 and PE9 */
#define MPX2102_SDADC_CLK_ENABLE  __HAL_RCC_SDADC1_CLK_ENABLE
#define MPX2102_SDADC_CLK_DISABLE __HAL_RCC_SDADC1_CLK_DISABLE
#define MPX2102_PWR_SDADC         PWR_SDADC_ANALOG1
#define MPX2102_SDADC_IRQn        SDADC1_IRQn
#define MPX2102_SDADC_IRQHandler  SDADC1_IRQHandler
#define MPX2102_SDADC             SDADC1
#define MPX2102_SDADC_GPIO_PORT   GPIOE
#define MPX2102_GPIO_CLK_ENABLE   __HAL_RCC_GPIOE_CLK_ENABLE
#define MPX2102_GPIO_CLK_DISABLE  __HAL_RCC_GPIOE_CLK_DISABLE
#define MPX2102_GPIO_PIN_P        GPIO_PIN_8
#define MPX2102_GPIO_PIN_N        GPIO_PIN_9
#define MPX2102_TIMER             TIM19
#define MPX2102_TIMER_CLK_ENABLE  __HAL_RCC_TIM19_CLK_ENABLE
#define MPX2102_TIMER_CLK_DISABLE __HAL_RCC_TIM19_CLK_DISABLE
#define MPX2102_TIMER_CHANNEL     TIM_CHANNEL_2
#define MPX2102_SDADC_VREF        SDADC_VREF_EXT /* Select External reference */
#define MPX2102_SDADC_GAIN        SDADC_GAIN_4   /* Select Internal gain = 4 */
#define SDADC_GAIN                (uint32_t) 4   /* SDADC internal gain is set to 4: update this define
                                                    according to MPX2102_SDADC_GAIN */
#define MPX2102_SDADC_CHANNEL     SDADC_CHANNEL_8
#define SDADC_RESOL               (uint32_t) 65535 /* 2e16 - 1 */
#define SDADC_INIT_TIMEOUT        30 /* about two SDADC clock cycles after INIT is set */
#define SDADC_CAL_TIMEOUT         4*30720 /*  ~5.12 ms at 6 MHz  in a single calibration sequence */
#define SDADC_VREF                (float) 3.3f /* SDADC external reference is set to 3.3V */
#define MPX2102_SENSITIVITY       (float) 13.2 /* MPX2102 sensitivity when powered by 3.3V
                                                  is 3.3V * 40mV / 10V = 13.2mV/1000mB = 13.2 microV/mB */
#define EXTERNGAIN                (float) 45.1  /* External gain equal to 2*(22K/1K)+1
                                                   is set using TVS632 mounted on STM32373C-EVAL */

/* An offset equal to VDD * R200 / R205 = VDD * 22/220 = VDD/10 */
#define OFFSET_VOLTAGE            (float)(SDADC_VREF/10.0f)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
