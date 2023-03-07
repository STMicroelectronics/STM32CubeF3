/**
  ******************************************************************************
  * @file    Demonstrations/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32f3348_discovery.h"
#include <math.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define STATE_MAN       0
#define STATE_OFF       1
#define STATE_DIMMER    2
#define STATE_FLASH     3
#define MAX_STATE       4

#define TRUE 0x01
#define FALSE 0x00

#define BUCK 1
#define BOOST 2
#define BUCKBOOSTLOW 3
#define BUCKBOOSTHIGH 4

/* Buck-boost converter */
#define QB_OFF_DEADTIME 1075
#define QB_ON_DEADTIME  275 

#define QD_OFF_DEADTIME 1225 
#define QD_ON_DEADTIME 75

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);

#endif /* __MAIN_H */
