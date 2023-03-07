/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/Inc/main.h
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
#include "stm32303c_eval.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "stm32303c_eval_sd.h"
#include "usbd_cdc_interface.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* It is possible to remap the USB interrupts (USB_LP and USB_WKUP) on interrupt
   lines 75 and 76.
   Uncomment the line below to select your USB Interrupt Line */

#define USE_USB_INTERRUPT_DEFAULT   1
/*#define USE_USB_INTERRUPT_REMAPPED        1*/

#if !defined (USE_USB_INTERRUPT_DEFAULT) && !defined (USE_USB_INTERRUPT_REMAPPED)
 #error "Missing define Please Define Your Interrupt Mode By UnComment Line in main.h file"
#endif
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
