/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_LLC_HalfBridge/Inc/main.h 
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
#include "stm32f3xx_hal.h"
#include "stm32f3348_discovery.h"

/* Exported types ------------------------------------------------------------*/
typedef enum { RUN, FAULT} DemoModeTypeDef;

/* Exported constants ---------------------------------------------------------*/

/* This defines the minimum LLC operating frequency, here 70.35kHz */
#define LLC_PWM_PERIOD ((uint16_t)0xFFDF)

/* This value is used to position the ADC sampling point before and after the 
commutation on the synchronous rectification FETs */
/* It corresponds to 250ns (0x480/32) x (1/144MHz), slightly above ADC
conversion time */
#define ADC_CONVERSION_TIME     ((uint16_t)0x480)

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------- */

extern DemoModeTypeDef DemoState;
extern HRTIM_HandleTypeDef hhrtim;


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
