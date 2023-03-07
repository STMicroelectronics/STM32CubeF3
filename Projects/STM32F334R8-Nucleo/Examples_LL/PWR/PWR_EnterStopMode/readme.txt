/**
  @page PWR_EnterStopMode PWR standby example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/PWR/PWR_EnterStopMode/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR STOP_LPREGU mode example.
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

How to enter the STOP_LPREGU mode.

After start-up LED2 is toggling during 5 seconds, then the system automatically 
enter in STOP_LPREGU mode (Final state).

LED2 is used to monitor the system state as follows:
 - LED2 toggling : system in RUN mode
 - LED2 off : system in STOP_LPREGU mode

@note To measure the current consumption in STOP_LPREGU mode, remove JP6 jumper 
      and connect an ampere meter to JP6 to measure IDD current.     

@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled.

@par Directory contents 

  - PWR/PWR_EnterStopMode/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - PWR/PWR_EnterStopMode/Inc/main.h                  Header for main.c module
  - PWR/PWR_EnterStopMode/Inc/stm32_assert.h          Template file to include assert_failed function
  - PWR/PWR_EnterStopMode/Src/stm32f3xx_it.c          Interrupt handlers
  - PWR/PWR_EnterStopMode/Src/main.c                  Main program
  - PWR/PWR_EnterStopMode/Src/system_stm32f3xx.c      STM32F3xx system source file

@par Hardware and Software environment

  - This example runs on STM32F334R8 devices.
    
  - This example has been tested with STMicroelectronics STM32F334R8-Nucleo Rev C
    board and can be easily tailored to any other supported device
    and development board.

  - STM32F334R8-Nucleo Rev C Set-up
    - LED2 connected to PA.05 pin

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
