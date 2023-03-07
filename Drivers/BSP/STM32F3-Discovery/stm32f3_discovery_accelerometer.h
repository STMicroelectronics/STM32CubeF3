/**
  ******************************************************************************
  * @file    stm32f3_discovery_accelerometer.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for stm32f3_discovery_accelerometer.c
  *          firmware driver.
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
#ifndef __STM32F3_DISCOVERY_ACCELERO_H
#define __STM32F3_DISCOVERY_ACCELERO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3_discovery.h"
/* Include Gyroscope component driver */
#include "../Components/lsm303dlhc/lsm303dlhc.h"
#include "../Components/lsm303agr/lsm303agr.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F3_DISCOVERY
  * @{
  */

/** @addtogroup STM32F3_DISCOVERY_ACCELEROMETER
  * @{
  */

/** @defgroup STM32F3_DISCOVERY_ACCELEROMETER_Exported_Types Exported Types
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32F3_DISCOVERY_ACCELEROMETER_Exported_Constants Exported Constants
  * @{
  */
typedef enum
{
  ACCELERO_OK = 0,
  ACCELERO_ERROR = 1,
  ACCELERO_TIMEOUT = 2
}
ACCELERO_StatusTypeDef;

/**
  * @}
  */

/** @defgroup STM32F3_DISCOVERY_ACCELEROMETER_Exported_Macros Exported Macros
  * @{
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup STM32F3_DISCOVERY_ACCELEROMETER_Exported_Functions Exported Functions
  * @{
  */
/* Acc functions */
uint8_t   BSP_ACCELERO_Init(void);
void      BSP_ACCELERO_Reset(void);
void      BSP_ACCELERO_GetXYZ(int16_t *pDataXYZ);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __STM32F3_DISCOVERY_ACCELERO_H */
