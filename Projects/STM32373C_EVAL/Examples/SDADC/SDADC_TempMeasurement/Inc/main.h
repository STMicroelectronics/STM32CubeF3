/**
  ******************************************************************************
  * @file    SDADC/SDADC_TempMeasurement/Inc/main.h
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
#define MESSAGE1   "    Temperature       "
#define MESSAGE2   "  Measurement PT100   "

/* Temperature sensor PT100 is connected to PE7 which corresponds to SDADC1 channel 3:
   Refer to STM32373C-EVAL user manual for more details about how the PT100 is
   connected to PE7 */
#define PT100_GPIO_PORT         GPIOE
#define PT100_GPIO_PIN          GPIO_PIN_7
#define PT100_GPIO_CLK_ENABLE   __HAL_RCC_GPIOE_CLK_ENABLE
#define PT100_GPIO_CLK_DISABLE  __HAL_RCC_GPIOE_CLK_DISABLE

#define PT100_SDADC             SDADC1
#define PT100_SDADC_CLK_ENABLE  __HAL_RCC_SDADC1_CLK_ENABLE
#define PT100_SDADC_CLK_DISABLE __HAL_RCC_SDADC1_CLK_DISABLE
#define PT100_SDADC_IRQn        SDADC1_IRQn
#define PT100_SDADC_IRQHandler  SDADC1_IRQHandler
#define PT100_PWR_SDADC         PWR_SDADC_ANALOG1
#define PT100_SDADC_VREF        SDADC_VREF_EXT /* External reference is selected */
#define PT100_SDADC_GAIN        SDADC_GAIN_8   /* Internal gain 8 is seleted */
#define SDADC_GAIN              (uint32_t)8    /* SDADC internal gain is set to 8: update this define
                                                 according to PT100_SDADC_GAIN */
#define PT100_SDADC_CHANNEL     SDADC_CHANNEL_3
#define SDADC_RESOL             (uint32_t) 65535 /* 2e16 - 1 */
#define SDADC_INIT_TIMEOUT      30 /* about two SDADC clock cycles after INIT is set */
#define SDADC_CAL_TIMEOUT       4*30720 /*  ~5.12 ms at 6 MHz  in a single calibration sequence */
#define SDADC_VREF              (float) 3.3f  /* SDADC external reference is set to 3.3V: update
                                                this define according to PT100_SDADC_VREF */
#define REFERENCE_RESISTOR      (uint32_t)1800 /* Reference resistor: labeled R33 on STM32373C-EVAL */
#define RESISTANCE_ZERODEGRE    (uint32_t)100  /* PT100 resistance at 0 deg C */
#define RESISTANCE_COEFFICIENT  (float)0.385f

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
