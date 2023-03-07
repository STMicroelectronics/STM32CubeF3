/**
  @page HRTIM HRTIM_BasicPWM

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    HRTIM/HRTIM_BasicPWM/readme.txt 
  * @author  MCD Application Team
  * @brief   This example describes how to generate basic PWM waveforms with the 
  * HRTIM, as per HRTIM Cookbook basic examples.
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
  This example describes how to generate basic PWM waveforms with the HRTIM,
  using HRTIM Cookbook basic examples.

  5 basic examples are provided. Its is necessary to uncomment one of the 
  following 5 #define statement at the beginning of the main.c module:

  #define HRTIM_CHECK: basic HRTIM outputs check
  #define SINGLE_PWM: elementary PWM generation
  #define MULTIPLE_PWM: PWM on multiple outputs
  #define PWM_MASTER: PWM generation with timers other than the timing unit itself
  #define ARBITRARY_WAVEFORM: non-PWM waveform example

  The PWM and waveforms are generated on the TD1 and TD2 outputs
  (resp. PB14 and PB15) and TA1 and TA2 outputs (resp. PA8 and PA9), depending 
  on example.

  Green LED: blinks during normal operation
  Red LED: ON when the ERROR handler is entered


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - HRTIM/HRTIM_BasicPWM/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - HRTIM/HRTIM_BasicPWM/Inc/stm32f3xx_it.h          PPP interrupt handlers header file
  - HRTIM/HRTIM_BasicPWM/Inc/main.h                  Header for main.c module  
  - HRTIM/HRTIM_BasicPWM/Src/stm32f3xx_it.c          PPP interrupt handlers
  - HRTIM/HRTIM_BasicPWM/Src/main.c                  Main program
  - HRTIM/HRTIM_BasicPWM/Src/stm32f3xx_hal_msp.c     HAL MSP file

@par Hardware and Software environment 

  - This example runs on STM32F334x4/STM32F334x6/STM32F334x8 devices.
    
  - This example has been tested with STM32F3348-DISCO
    board and can be easily tailored to any other supported device and development board.
  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
