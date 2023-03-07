/**
  ******************************************************************************
  * @file    Demonstrations/Inc/main.h 
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
#ifndef __STM32F3_DISCOVERY_DEMO_H
#define __STM32F3_DISCOVERY_DEMO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3_discovery.h"
#include "stm32f3_discovery_accelerometer.h"
#include "stm32f3_discovery_gyroscope.h"
#include "selftest.h"
#include "usbd_desc.h"
#include "usbd_hid.h" 
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define KEY_PRESSED     0x01
#define KEY_NOT_PRESSED 0x00

/* Exported macro ------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x

/* It is possible to remap the USB interrupts (USB_LP and USB_WKUP) on interrupt 
   lines 75 and 76.
   Uncomment the line below to select your USB Interrupt Line */

/* #define USE_USB_INTERRUPT_DEFAULT   1 */
#define USE_USB_INTERRUPT_REMAPPED        1

#if !defined (USE_USB_INTERRUPT_DEFAULT) && !defined (USE_USB_INTERRUPT_REMAPPED)
 #error "Missing define Please Define Your Interrupt Mode By UnComment Line in main.h file"
#endif

/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* __STM32F3_DISCOVERY_DEMO_H */
