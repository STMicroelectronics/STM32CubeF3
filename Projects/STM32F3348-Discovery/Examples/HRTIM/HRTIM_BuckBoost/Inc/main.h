/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_BuckBoost/Inc/main.h 
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

/* Exported constants ---------------------------------------------------------*/
#define BUCK_PWM_PERIOD ((uint16_t)18432) // 250kHz
#define BUCK            0
#define BOOST           1
#define DE_ENERGIZE     2
#define FAULT           3


/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------- */

extern uint32_t DemoState;
extern HRTIM_HandleTypeDef hhrtim;


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
