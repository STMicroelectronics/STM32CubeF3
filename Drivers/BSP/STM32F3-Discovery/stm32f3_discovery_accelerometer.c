/**
  ******************************************************************************
  * @file    stm32f3_discovery_accelerometer.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to manage the ACCELEROMETER
  *          MEMS available on STM32F3-Discovery Kit.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
uint8_t BSP_ACCELERO_Init(void)
{  
  uint8_t ret = ACCELERO_ERROR;
  uint16_t ctrl = 0x0000;
  ACCELERO_InitTypeDef LSM303DLHC_InitStructure;
  ACCELERO_FilterConfigTypeDef LSM303DLHC_FilterStructure;
 
  if(Lsm303dlhcDrv.ReadID() == I_AM_LMS303DLHC)
  {
    /* Initialize the gyroscope driver structure */
    AccelerometerDrv = &Lsm303dlhcDrv;
  
  /* MEMS configuration ------------------------------------------------------*/
   /* Fill the accelerometer structure */
    LSM303DLHC_InitStructure.Power_Mode = LSM303DLHC_NORMAL_MODE;
    LSM303DLHC_InitStructure.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
    LSM303DLHC_InitStructure.Axes_Enable= LSM303DLHC_AXES_ENABLE;
    LSM303DLHC_InitStructure.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
    LSM303DLHC_InitStructure.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
    LSM303DLHC_InitStructure.Endianness=LSM303DLHC_BLE_LSB;
    LSM303DLHC_InitStructure.High_Resolution=LSM303DLHC_HR_ENABLE;
    
    /* Configure MEMS: data rate, power mode, full scale and axes */
    ctrl |= (LSM303DLHC_InitStructure.Power_Mode | LSM303DLHC_InitStructure.AccOutput_DataRate | \
                       LSM303DLHC_InitStructure.Axes_Enable);
    
    ctrl |= ((LSM303DLHC_InitStructure.BlockData_Update | LSM303DLHC_InitStructure.Endianness | \
                      LSM303DLHC_InitStructure.AccFull_Scale | LSM303DLHC_InitStructure.High_Resolution) << 8);
    
  /* Configure the accelerometer main parameters */
    AccelerometerDrv->Init(ctrl);
  
  /* Fill the accelerometer LPF structure */
    LSM303DLHC_FilterStructure.HighPassFilter_Mode_Selection =LSM303DLHC_HPM_NORMAL_MODE;
    LSM303DLHC_FilterStructure.HighPassFilter_CutOff_Frequency = LSM303DLHC_HPFCF_16;
    LSM303DLHC_FilterStructure.HighPassFilter_AOI1 = LSM303DLHC_HPF_AOI1_DISABLE;
    LSM303DLHC_FilterStructure.HighPassFilter_AOI2 = LSM303DLHC_HPF_AOI2_DISABLE;
    
    /* Configure MEMS: mode, cutoff frquency, Filter status, Click, AOI1 and AOI2 */
    ctrl = (uint8_t) (LSM303DLHC_FilterStructure.HighPassFilter_Mode_Selection |\
                      LSM303DLHC_FilterStructure.HighPassFilter_CutOff_Frequency|\
                      LSM303DLHC_FilterStructure.HighPassFilter_AOI1|\
                      LSM303DLHC_FilterStructure.HighPassFilter_AOI2);

  /* Configure the accelerometer LPF main parameters */
    AccelerometerDrv->FilterConfig(ctrl);

    ret = ACCELERO_OK;
  }  
  else
  {
    ret = ACCELERO_ERROR;
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
  * @param pDataXYZ Pointeur on 3 angular accelerations 
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
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/     
