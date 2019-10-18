/**
  ******************************************************************************
  * @file    BSP/Inc/mems.h 
  * @author  MCD Application Team
  * @brief   Header for mems.c module
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
#ifndef __MEMS_H
#define __MEMS_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ACCELERO_MEMS_Test(void);
void GYRO_MEMS_Test(void);
#endif /* __MEMS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
