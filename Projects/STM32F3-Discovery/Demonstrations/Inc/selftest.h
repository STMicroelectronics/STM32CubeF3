/**
  ******************************************************************************
  * @file    Demonstrations/Inc/selftest.h 
  * @author  MCD Application Team
  * @brief   Header for selftest.c module
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
#ifndef __SELFTEST_H
#define __SELFTEST_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
void ACCELERO_MEMS_Test(void);
void GYRO_MEMS_Test(void);
void USB_Test(void);
void USB_Demo(void);
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#endif /* __SELFTEST_H*/
