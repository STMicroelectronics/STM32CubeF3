/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_BuckBoost_AN4449/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3348_discovery.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
BUCKBOOST_MODE_IDLE = 0,
BUCKBOOST_MODE_BUCK,
BUCKBOOST_MODE_BOOST,
BUCKBOOST_MODE_MIXED 
}BuckBoost_ModeTypeDef;

extern BuckBoost_ModeTypeDef Mode;

/* Exported constants ---------------------------------------------------------*/
/************************************************************************************************************************/
#define VOUT_TARGET ((uint16_t) 5000) // This is the user VOUT voltage target in mV that DC/DC converter will apply on V output
/************************************************************************************************************************/
#define BUCK_PWM_PERIOD ((uint16_t) 18432) /* 250kHz */
#define REAL_3V3 ((uint16_t) 3300) /* Measure 3V3 of application and enter it as reference in mV (default value is 3300) */
#define VIN_RESISTOR_RATIO  ((uint16_t) 2012) /* theoretical bridge resistor Vin ratio (6.8K/(6.8K + 27K))*10000: can be adjusted if needed by measuring the real resistors ratio */
#define VOUT_RESISTOR_RATIO  ((uint16_t) 1988) /* theoretical bridge resistor Vout ratio (3.3K/(3.3K + 13.3K))*10000: can be adjusted if needed by measuring the real resistors ratio */

/***************************** DO NOT MODIFY ********************************/
#define MIN_DUTY_A ((int32_t)(BUCK_PWM_PERIOD * 15)/100) // % MinDuty for Buck
#define MAX_DUTY_A ((int32_t)(BUCK_PWM_PERIOD * 90)/100) // % MaxDuty for Buck

#define MIN_DUTY_B ((int32_t)(BUCK_PWM_PERIOD * 5)/100) // % MinDuty for Boost
#define MAX_DUTY_B ((int32_t)(BUCK_PWM_PERIOD * 90)/100) // % MaxDuty for Boost

#define MIN_DUTY_B_MIXED ((int32_t)(BUCK_PWM_PERIOD * 5)/100) // % MinDuty for Mixed
#define MAX_DUTY_B_MIXED ((int32_t)(BUCK_PWM_PERIOD * 45)/100) // % MaxDuty for Mixed

#define SAT_LIMIT ((int32_t)(BUCK_PWM_PERIOD * 50)/100) // PI Integral term saturation value
/****************************************************************************/

/* Compute Vin & Vout, Min 3V, Max 15V with 100mV margin, other corrections are due to resistors bridges and real 3V3 of application */
#define VINMIN          ((uint32_t) (((2900  * VIN_RESISTOR_RATIO) /10000) * 0x1000) / REAL_3V3)  // Minimum admissible Vin = 3V in mV
#define VINMAX          ((uint32_t) (((15100 * VIN_RESISTOR_RATIO) /10000) * 0x1000) / REAL_3V3)  // Maximum admissible Vin = 15V in mV
#define VOUTMIN         ((uint32_t) (((2900  * VOUT_RESISTOR_RATIO) /10000) * 0x1000) / REAL_3V3)// Minimum admissible Vout = 3V in mV
#define VOUTMAX         ((uint32_t) (((15100 * VOUT_RESISTOR_RATIO) /10000) * 0x1000) / REAL_3V3) // Maximum admissible Vout = 15V in mV

#define DT_RISING       ((uint16_t)230) // Dead time rising edge
#define DT_FALLING      ((uint16_t)230) // Dead time falling edge

#define MAX_ERROR     ((uint16_t)1000) // Max number of repetitive errors
#define MAX_RANGE     ((uint16_t)20000) // Max number of repetitive Out of Range Vin or Vout conditions
#define MAX_OVERLOAD  ((uint16_t)100) // Max number of repetitive Overload conditions (X * 1.6ms reaction time)
#define OVERLOAD_MARGIN ((uint16_t)99) // % Margin between standard operation and overload start conditions

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------- */


extern int32_t PI_Buck(void);
extern int32_t PI_Boost(void);
extern int32_t PI_Mixed(void);
extern void SetHRTIM_BuckMode(void);
extern void SetHRTIM_BoostMode(void);
extern void SetHRTIM_MixedMode(void);

/* Exported variables --------------------------------------------------------- */
extern HRTIM_HandleTypeDef hhrtim;
extern ADC_HandleTypeDef AdcHandle;

extern uint32_t CTMin;
extern uint32_t CTMax; 
extern uint32_t CTRange;
extern uint32_t CurrentDutyA;
extern uint32_t CurrentDutyB;
extern uint32_t CurrentDutyB_Mixed;
extern volatile uint32_t VoutT;
extern volatile uint32_t VoutRange;
extern volatile uint32_t VoutA;
extern volatile uint32_t VoutB;
extern volatile uint32_t VinConversion;
extern volatile uint32_t VoutConversion;
extern volatile uint8_t Idle2Buck;
extern volatile uint8_t Idle2Boost; 
extern volatile uint8_t Run2Stop;



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
