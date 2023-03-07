/**
  ******************************************************************************
  * @file    stm32303e_eval_tsensor.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the I2C TS751 
  *          temperature sensor mounted on STM32303E-EVAL board . 
  *          It implements a high level communication layer for read and write 
  *          from/to this sensor. The needed STM323F30x hardware resources (I2C and 
  *          GPIO) are defined in stm32303e_eval.h file, and the initialization is 
  *          performed in TSENSOR_IO_Init() function declared in stm32303e_eval.c 
  *          file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          TSENSOR_IO_Init() function. 
  *
  *     +--------------------------------------------------------------------+
  *     |                        Pin assignment                              |                 
  *     +----------------------------------------+--------------+------------+
  *     |  STM32F30x I2C Pins                    |   STTS751    |   Pin      |
  *     +----------------------------------------+--------------+------------+
  *     | .                                      |   Addr/Therm |    1       |
  *     | .                                      |   GND        |    2 (0V)  |
  *     | .                                      |   VDD        |    3 (3.3V)|
  *     | TSENSOR_I2C_SCL_PIN/ SCL               |   SCL        |    4       |
  *     | TSENSOR_I2C_SMBUSALERT_PIN/ SMBUS ALERT|   SMBUS ALERT|    5       |
  *     | TSENSOR_I2C_SDA_PIN/ SDA               |   SDA        |    6       |
  *     +----------------------------------------+--------------+------------+
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

/* Includes ------------------------------------------------------------------*/
#include "stm32303e_eval_tsensor.h"

/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup STM32303E_EVAL
  * @{
  */

/** @addtogroup STM32303E_EVAL_TSENSOR
  * @brief      This file includes the TS751 Temperature Sensor driver of 
  *             STM32303E-EVAL boards.
  * @{
  */ 

/** @addtogroup STM32303E_EVAL_TSENSOR_Private_Variables
  * @{
  */ 
static TSENSOR_DrvTypeDef  *tsensor_drv; 
/**
  * @}
  */ 

/** @addtogroup STM32303E_EVAL_TSENSOR_Private_Functions
  * @{
  */ 

/**
  * @brief  Initializes peripherals used by the I2C Temperature Sensor driver.
  * @retval TSENSOR status
  */
uint32_t BSP_TSENSOR_Init(void)
{ 
  uint8_t ret = TSENSOR_ERROR;
  TSENSOR_InitTypeDef STTS751_InitStructure;

  /* Temperature Sensor Initialization */
  if(Stts751Drv.IsReady(TSENSOR_I2C_ADDRESS, TSENSOR_MAX_TRIALS) == HAL_OK)
  {
    /* Initialize the temperature sensor driver structure */
    tsensor_drv = &Stts751Drv;

    /* Configure Temperature Sensor : Conversion 12 bits in continuous mode at one conversion per second */
    /* Alert outside range Limit Temperature 12� <-> 24�c */
    STTS751_InitStructure.AlertMode             = STTS751_ALERT_ENABLE;
    STTS751_InitStructure.ConversionMode        = STTS751_CONTINUOUS_MODE;
    STTS751_InitStructure.ConversionResolution  = STTS751_CONV_12BITS;
    STTS751_InitStructure.ConversionRate        = STTS751_ONE_PER_SECOND;           
    STTS751_InitStructure.TemperatureLimitHigh  = 24;
    STTS751_InitStructure.TemperatureLimitLow   = 12;
        
    /* TSENSOR Init */   
    tsensor_drv->Init(TSENSOR_I2C_ADDRESS, &STTS751_InitStructure);

    ret = TSENSOR_OK;
  }
  else
  {
    ret = TSENSOR_ERROR;
  }
  
  return ret;
}

/**
  * @brief  Returns the Temperature Sensor status.
  * @retval The Temperature Sensor status.
  */
uint8_t BSP_TSENSOR_ReadStatus(void)
{
  return (tsensor_drv->ReadStatus(TSENSOR_I2C_ADDRESS));
}

/**
  * @brief  Read Temperature register of TS751.
  * @retval STTS751 measured temperature value.
  */
uint16_t BSP_TSENSOR_ReadTemp(void)
{ 
  return tsensor_drv->ReadTemp(TSENSOR_I2C_ADDRESS);

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

