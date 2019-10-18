/**
  ******************************************************************************
  * @file    Demonstrations/Inc/selftest.h 
  * @author  MCD Application Team
  * @brief   Header for selftest.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
