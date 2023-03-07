/**
  @page TSC_BasicAcquisition_Interrupt Touch-Sensing basic acquisition using interrupt

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TSC/TSC_BasicAcquisition_Interrupt/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the TSC basic acquisition interrupt example.
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
  @endverbatim

@par Example Description 

Use of he TSC to perform continuous acquisitions 
of two channels in Interrupt mode.

When the end of acquisition occurs, the acquired data are affected to the
uhTSCAcquisitionValue[] variable.

STM32 Eval board's LEDs are used as follows:
  - LED1, LED2 and LED3 show the touch activity for each channel.
  - LED4 toggles when an error occurs (TSC initialization, TSC start or assert).
 
@par Directory contents 

  - TSC/TSC_BasicAcquisition_Interrupt/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - TSC/TSC_BasicAcquisition_Interrupt/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - TSC/TSC_BasicAcquisition_Interrupt/Inc/main.h                  Header for main.c module  
  - TSC/TSC_BasicAcquisition_Interrupt/Src/stm32f3xx_it.c          Interrupt handlers
  - TSC/TSC_BasicAcquisition_Interrupt/Src/main.c                  Main program
  - TSC/TSC_BasicAcquisition_Interrupt/Src/stm32f3xx_hal_msp.c     HAL MSP file 
  - TSC/TSC_BasicAcquisition_Interrupt/Src/system_stm32f3xx.c      STM32F3xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F373xC devices.
  
  - This example has been tested with STM32373C-EVAL board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
