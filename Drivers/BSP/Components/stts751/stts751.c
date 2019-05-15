/**
  ******************************************************************************
  * @file    stts7510.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    02-December-2014
  * @brief   This file provides a set of functions needed to manage the STTS751
  *          Temperature Sensor.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "stts751.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup STTS751
  * @brief     This file provides a set of functions needed to drive the 
  *            STTS751 Temperature Sensor.
  * @{
  */

/** @defgroup STTS751_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup STTS751_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup STTS751_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup STTS751_Private_Variables
  * @{
  */ 
TSENSOR_DrvTypeDef Stts751Drv =
{
  STTS751_Init,
  STTS751_IsReady,
  STTS751_ReadStatus,
  STTS751_ReadTemp,
};

/**
  * @}
  */

/** @defgroup STTS751_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup STTS751_Private_Functions
  * @{
  */

/**
  * @brief  Set STTS751 Initialization.
  * @param  DeviceAddr : Device ID address.
  * @param  pInitStruct: pointer to a STTS751_InitTypeDef structure 
  *         that contains the configuration setting for the STTS751.
  * @retval None
  */
void STTS751_Init(uint16_t DeviceAddr, TSENSOR_InitTypeDef *pInitStruct)
{  
  uint8_t tmp = 0;

  /* Set the Configuration Register */
  tmp = (uint8_t)(pInitStruct->AlertMode | pInitStruct->ConversionMode | pInitStruct->ConversionResolution);
  TSENSOR_IO_Write(DeviceAddr, &tmp, STTS751_CONFIGURATION, 1);

  /* Set the Conversion Rate */
  tmp = (uint8_t)(pInitStruct->ConversionRate);
  TSENSOR_IO_Write(DeviceAddr, &tmp, STTS751_CONVERSION_RATE, 1);

  /* Set the Temperature Registers */
  tmp = (uint8_t)(pInitStruct->TemperatureLimitHigh);
  TSENSOR_IO_Write(DeviceAddr, &tmp, STTS751_TEMP_HIGH_LIMIT_HIGH_BYTE, 1);

  tmp = (uint8_t)(pInitStruct->TemperatureLimitLow);
  TSENSOR_IO_Write(DeviceAddr, &tmp, STTS751_TEMP_LOW_LIMIT_HIGH_BYTE, 1);
}

/**
  * @brief  Read ID address of STTS751
  * @param  DeviceAddr : Device ID address.
  * @param  Trials: Number of trials
  * @retval ID name
  */
uint8_t STTS751_IsReady(uint16_t DeviceAddr, uint32_t Trials)
{
  /* Configure the low level interface ---------------------------------------*/
  TSENSOR_IO_Init();
  
  /* Check is Temperature Sensor is Ready to use */
  return TSENSOR_IO_IsDeviceReady(DeviceAddr, Trials);
}

/**
  * @brief  Read The Temperature Sensor Status
  * @param  DeviceAddr : Device ID address.
  * @retval Status
  */
uint8_t STTS751_ReadStatus(uint16_t DeviceAddr)
{
  uint8_t tmp = 0;

  /* Read Status register */
  TSENSOR_IO_Read(DeviceAddr, &tmp, STTS751_STATUS, 1);

  /* Return Temperature Sensor Status */
  return (uint8_t)(tmp);
}

/**
  * @brief  Read ID address of STTS751
  * @param  DeviceAddr : Device ID address.
  * @retval ID name
  */
uint16_t STTS751_ReadTemp(uint16_t DeviceAddr)
{
  uint8_t bufferrx[2] ={0,0};
  uint16_t tmp = 0;

  /* Read Temperature registers */
  TSENSOR_IO_Read(DeviceAddr, &bufferrx[0], STTS751_TEMP_VALUE_HIGH_BYTE, 1);
  TSENSOR_IO_Read(DeviceAddr, &bufferrx[1], STTS751_TEMP_VALUE_LOW_BYTE, 1);

  tmp = (uint16_t)(bufferrx[0] << 8);
  tmp |= bufferrx[1];      
  /* Return Temperature value */
  return (uint16_t)(tmp >> 4);
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/     
