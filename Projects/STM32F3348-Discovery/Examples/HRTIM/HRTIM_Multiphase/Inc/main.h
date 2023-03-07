/**
  ******************************************************************************
  * @file    HRTIM\HRTIM_Multiphase/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32f3348_discovery.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants ---------------------------------------------------------*/
#define MULTIPHASE_PERIOD ((uint16_t)0xA000) // 
#define PHASE_SHIFT_INIT ((uint16_t)MULTIPHASE_PERIOD / 5 )
#define DUTY_CYCLE_INIT ((uint16_t)PHASE_SHIFT_INIT / 2)

#define ADC_CONVERSION_TIME     ((uint16_t)0x480)
typedef enum {
  FIVE_PHASE_INIT, FOUR_PHASE_INIT, THREE_PHASE_INIT, TWO_PHASE_INIT, ONE_PHASE_INIT, BURST_INIT,
  FIVE_PHASE, FOUR_PHASE, THREE_PHASE, TWO_PHASE, ONE_PHASE, BURST, FAULT} DemoModeTypeDef;
        

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------- */

extern DemoModeTypeDef DemoState;
extern HRTIM_HandleTypeDef hhrtim;


#endif /* __MAIN_H */
