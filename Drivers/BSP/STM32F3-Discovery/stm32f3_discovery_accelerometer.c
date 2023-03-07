/**
  ******************************************************************************
  * @file    stm32f3_discovery_accelerometer.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the
  *          MEMS accelerometer available on STM32F3-Discovery Kit.
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
#include "stm32f3_discovery_accelerometer.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F3_DISCOVERY
  * @{
  */

/** @defgroup STM32F3_DISCOVERY_ACCELEROMETER STM32F3-DISCOVERY ACCELEROMETER
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/** @defgroup STM32F3_DISCOVERY_ACCELEROMETER_Private_Types Private Types
  * @{
  */
/**
  * @}
  */

/* Private defines ------------------------------------------------------------*/
/** @defgroup STM32F3_DISCOVERY_ACCELEROMETER_Private_Constants Private Constants
  * @{
  */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup STM32F3_DISCOVERY_ACCELEROMETER_Private_Macros Private Macros
  * @{
  */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup STM32F3_DISCOVERY_ACCELEROMETER_Private_Variables Private Variables
  * @{
  */
static ACCELERO_DrvTypeDef *AccelerometerDrv;


/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @addtogroup STM32F3_DISCOVERY_ACCELEROMETER_Private_FunctionPrototypes Private Functions
  * @{
  */
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @addtogroup STM32F3_DISCOVERY_ACCELEROMETER_Exported_Functions
  * @{
  */

/**
  * @brief  Set accelerometer Initialization.
  * @retval ACCELERO_OK if no problem during initialization
  */
uint8_t BSP_ACCELERO_Init(void)
{
  uint8_t ret = ACCELERO_ERROR;
  uint16_t ctrl = 0x0000;
  ACCELERO_InitTypeDef         Accelero_InitStructure;
  ACCELERO_FilterConfigTypeDef Accelero_FilterStructure = {0,0,0,0};

  if(Lsm303dlhcDrv.ReadID() == I_AM_LMS303DLHC)
  {
    /* Initialize the accelerometer driver structure */
    AccelerometerDrv = &Lsm303dlhcDrv;

    /* MEMS configuration ----------------------------------------------------*/
    /* Fill the accelerometer structure */
    Accelero_InitStructure.Power_Mode         = LSM303DLHC_NORMAL_MODE;
    Accelero_InitStructure.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
    Accelero_InitStructure.Axes_Enable        = LSM303DLHC_AXES_ENABLE;
    Accelero_InitStructure.AccFull_Scale      = LSM303DLHC_FULLSCALE_2G;
    Accelero_InitStructure.BlockData_Update   = LSM303DLHC_BlockUpdate_Continous;
    Accelero_InitStructure.Endianness         = LSM303DLHC_BLE_LSB;
    Accelero_InitStructure.High_Resolution    = LSM303DLHC_HR_ENABLE;

    /* Configure MEMS: data rate, power mode, full scale and axes */
    ctrl |= (Accelero_InitStructure.Power_Mode | Accelero_InitStructure.AccOutput_DataRate | \
             Accelero_InitStructure.Axes_Enable);

    ctrl |= ((Accelero_InitStructure.BlockData_Update | Accelero_InitStructure.Endianness | \
              Accelero_InitStructure.AccFull_Scale    | Accelero_InitStructure.High_Resolution) << 8);

    /* Configure the accelerometer main parameters */
    AccelerometerDrv->Init(ctrl);

    /* Fill the accelerometer LPF structure */
    Accelero_FilterStructure.HighPassFilter_Mode_Selection   = LSM303DLHC_HPM_NORMAL_MODE;
    Accelero_FilterStructure.HighPassFilter_CutOff_Frequency = LSM303DLHC_HPFCF_16;
    Accelero_FilterStructure.HighPassFilter_AOI1             = LSM303DLHC_HPF_AOI1_DISABLE;
    Accelero_FilterStructure.HighPassFilter_AOI2             = LSM303DLHC_HPF_AOI2_DISABLE;

    /* Configure MEMS: mode, cutoff frquency, Filter status, Click, AOI1 and AOI2 */
    ctrl = (uint8_t) (Accelero_FilterStructure.HighPassFilter_Mode_Selection   |\
                      Accelero_FilterStructure.HighPassFilter_CutOff_Frequency |\
                      Accelero_FilterStructure.HighPassFilter_AOI1             |\
                      Accelero_FilterStructure.HighPassFilter_AOI2);

    /* Configure the accelerometer LPF main parameters */
    AccelerometerDrv->FilterConfig(ctrl);

    ret = ACCELERO_OK;
  }
  else if(Lsm303agrDrv.ReadID() == I_AM_LSM303AGR)
  {
    /* Initialize the accelerometer driver structure */
    AccelerometerDrv = &Lsm303agrDrv;

    /* MEMS configuration ----------------------------------------------------*/
    /* Fill the accelerometer structure */
    Accelero_InitStructure.Power_Mode         = LSM303AGR_NORMAL_MODE;
    Accelero_InitStructure.AccOutput_DataRate = LSM303AGR_ODR_50_HZ;
    Accelero_InitStructure.Axes_Enable        = LSM303AGR_AXES_ENABLE;
    Accelero_InitStructure.AccFull_Scale      = LSM303AGR_FULLSCALE_2G;
    Accelero_InitStructure.BlockData_Update   = LSM303AGR_BlockUpdate_Continous;
    Accelero_InitStructure.Endianness         = LSM303AGR_BLE_LSB;
    Accelero_InitStructure.High_Resolution    = LSM303AGR_HR_ENABLE;

    /* Configure MEMS: data rate, power mode, full scale and axes */
    ctrl |= (Accelero_InitStructure.Power_Mode | Accelero_InitStructure.AccOutput_DataRate | \
             Accelero_InitStructure.Axes_Enable);

    ctrl |= ((Accelero_InitStructure.BlockData_Update | Accelero_InitStructure.Endianness | \
              Accelero_InitStructure.AccFull_Scale    | Accelero_InitStructure.High_Resolution) << 8);

    /* Configure the accelerometer main parameters */
    AccelerometerDrv->Init(ctrl);

    /* Fill the accelerometer LPF structure */
    Accelero_FilterStructure.HighPassFilter_Mode_Selection   = LSM303AGR_HPM_NORMAL_MODE;
    Accelero_FilterStructure.HighPassFilter_CutOff_Frequency = LSM303AGR_HPFCF_16;
    Accelero_FilterStructure.HighPassFilter_AOI1             = LSM303AGR_HPF_AOI1_DISABLE;
    Accelero_FilterStructure.HighPassFilter_AOI2             = LSM303AGR_HPF_AOI2_DISABLE;

    /* Configure MEMS: mode, cutoff frquency, Filter status, Click, AOI1 and AOI2 */
    ctrl = (uint8_t) (Accelero_FilterStructure.HighPassFilter_Mode_Selection   |\
                      Accelero_FilterStructure.HighPassFilter_CutOff_Frequency |\
                      Accelero_FilterStructure.HighPassFilter_AOI1             |\
                      Accelero_FilterStructure.HighPassFilter_AOI2);

    /* Configure the accelerometer LPF main parameters */
    AccelerometerDrv->FilterConfig(ctrl);

    ret = ACCELERO_OK;
  }

  return ret;
}

/**
  * @brief  Reboot memory content of ACCELEROMETER
  * @retval None
  */
void BSP_ACCELERO_Reset(void)
  {
  if(AccelerometerDrv->Reset != NULL)
{
    AccelerometerDrv->Reset();
}
}

/**
  * @brief  Get XYZ acceleration
  * @param pDataXYZ Pointer on 3 angular accelerations
  *                 pDataXYZ[0] = X axis, pDataXYZ[1] = Y axis, pDataXYZ[2] = Z axis
* @retval None
*/
void BSP_ACCELERO_GetXYZ(int16_t *pDataXYZ)
{
  if(AccelerometerDrv->GetXYZ!= NULL)
  {
    AccelerometerDrv->GetXYZ(pDataXYZ);
  }
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


