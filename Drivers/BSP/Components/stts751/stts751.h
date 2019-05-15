/**
  ******************************************************************************
  * @file    stts751.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    02-December-2014
  * @brief   This file contains all the functions prototypes for the stts751.c
  *          temperature sensor driver.
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
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STTS751_H
#define __STTS751_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "../Common/tsensor.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup STTS751
  * @{
  */
  
/** @defgroup STTS751_Exported_Constants
  * @{
  */
/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/
/***************************** Read Access Only *******************************/
#define STTS751_STATUS                          0x01  /*!< Status Register */
#define STTS751_TEMP_VALUE_HIGH_BYTE            0x00  /*!< Temperature value high byte Register */
#define STTS751_TEMP_VALUE_LOW_BYTE             0x02  /*!< Temperature value low byte Register */

#define STTS751_PRODUCT_ID                      0xFD  /*!< Product Identifier Register */
#define STTS751_MANUFACTURER_ID                 0xFE  /*!< Manufacturer Identifier Register */
#define STTS751_REVISION_ID                     0xFF  /*!< Revision Number Identifier Register */

   /***************************** Write Access Only ******************************/
#define STTS751_ONE_SHOT                        0x0F  /*!< Single Conversion Request Register */

   
/***************************** Read/Write Access ******************************/
#define STTS751_CONFIGURATION                   0x03  /*!< Configuration Register */
#define STTS751_CONVERSION_RATE                 0x04  /*!< Conversion Rate Register */
#define STTS751_TEMP_HIGH_LIMIT_HIGH_BYTE       0x05  /*!< High Temperature Limit high byte Register */
#define STTS751_TEMP_HIGH_LIMIT_LOW_BYTE        0x06  /*!< High Temperature Limit low byte Register */
#define STTS751_TEMP_LOW_LIMIT_HIGH_BYTE        0x07  /*!< Low Temperature Limit high byte Register */
#define STTS751_TEMP_LOW_LIMIT_LOW_BYTE         0x08  /*!< Low Temperature Limit low byte Register */


#define STTS751_THERM_LIMIT                     0x20  /*!< THERM Limit Register */
#define STTS751_THERM_HYSTERISIS                0x21  /*!< THERM Hysteris Register */
#define STTS751_SMBUS_TIMEOUT                   0x22  /*!< SMBUS Timeout Enabling Register */


/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/
/** @defgroup EVENT_Pin_selection 
  * @{
  */
#define STTS751_ALERT_ENABLE                    ((uint8_t)0x00)
#define STTS751_ALERT_DISABLE                   ((uint8_t)0x80)
/**
  * @}
  */

/** @defgroup Conversion_Mode_Selection 
  * @{
  */
#define STTS751_CONTINUOUS_MODE                 ((uint8_t)0x00)
#define STTS751_ONE_SHOT_MODE                   ((uint8_t)0x40)
/**
  * @}
  */

/** @defgroup Conversion_Bits_Resolution_Selection
  * @{
  */
#define STTS751_CONV_9BITS                      ((uint8_t)0x08)
#define STTS751_CONV_10BITS                     ((uint8_t)0x00)
#define STTS751_CONV_11BITS                     ((uint8_t)0x04)
#define STTS751_CONV_12BITS                     ((uint8_t)0x0C)
/**
  * @}
  */

/** @defgroup Conversion_Rate_Per_Second_Selection
  * @{
  */
#define STTS751_ONE_SIXTEENTH_PER_SECOND        ((uint8_t)0x00) /* 0.0625 conversion/Second     */
#define STTS751_ONE_EIGHTH_PER_SECOND           ((uint8_t)0x01) /* 0.125 conversion/Second      */
#define STTS751_ONE_QUATER_PER_SECOND           ((uint8_t)0x02) /* 0.25 conversion/Second       */
#define STTS751_HALF_PER_SECOND                 ((uint8_t)0x03) /* 0.5 conversion/Second        */
#define STTS751_ONE_PER_SECOND                  ((uint8_t)0x04) /* 1 conversion/Second          */
#define STTS751_TWO_PER_SECOND                  ((uint8_t)0x05) /* 2 conversions/Second         */
#define STTS751_FOUR_PER_SECOND                 ((uint8_t)0x05) /* 4 conversions/Second         */
#define STTS751_HEIGH_PER_SECOND                ((uint8_t)0x07) /* 8 conversions/Second         */
#define STTS751_SIXTEEN_PER_SECOND              ((uint8_t)0x08) /* 16 conversions/Second        */
#define STTS751_THIRTY_TWO_PER_SECOND           ((uint8_t)0x09) /* 32 conversions/Second        */
/**
  * @}
  */

/**
  * @}
  */
 
/** @defgroup STTS751_Exported_Functions
  * @{
  */
/* Sensor Configuration Functions */ 
void                            STTS751_Init(uint16_t DeviceAddr, TSENSOR_InitTypeDef *pInitStruct);
uint8_t                         STTS751_IsReady(uint16_t DeviceAddr, uint32_t Trials);
/* Sensor Request Functions */
uint8_t                         STTS751_ReadStatus(uint16_t DeviceAddr);
uint16_t                        STTS751_ReadTemp(uint16_t DeviceAddr);

/* Temperature Sensor driver structure */
extern TSENSOR_DrvTypeDef       Stts751Drv;

/* Temperature Sensor IO functions */
void                            TSENSOR_IO_Init(void);
void                            TSENSOR_IO_Write(uint16_t DevAddress, uint8_t* pBuffer, uint8_t WriteAddr, uint16_t Length);
void                            TSENSOR_IO_Read(uint16_t DevAddress, uint8_t* pBuffer, uint8_t ReadAddr, uint16_t Length);
uint16_t                        TSENSOR_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);
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
  
#endif /* __STTS751_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
