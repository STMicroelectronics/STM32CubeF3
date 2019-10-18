/**
  @page HRTIM HRTIM_TM_PFC

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    HRTIM/HRTIM_TM_PFC/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of HRTIM_TM_PFC example
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
  @endverbatim

@par Example Description 
  This example shows how to configure the HRTIM to control a transition mode PFC.
   
  This is done with the TD1 output (PB14).
  The TD1 control signal has a constant Ton time and variable frequency.
  
  To exercice fully the demo, it is necessary to simulate the feedback signals:
  - Over-current signal ("OC", on EEV3/PB7)
  - Zero-crossing detection signal ("ZCD", on EEV4/PB6)
  
  OC is a single-shot short pulse ( typically 200ns ) triggered by TD1 rising edge. It has to be 
  delayed to shorten Ton (Ton< Ton max) ==> the delay from TD1 to OC rising edge OC period should be
  less than Ton max.
  If the delay from TD1 to OC rising edge is lower than Ton min, the OC signal is ignored.

  ZCD signal is a periodic signal with a pulse length ~ 200 ns.  The period must be varied to verify
  the 3 operating ranges:
      1. If ZCD signal is not present, the converter will run with a period equal to Ton + Toffmax
         (converter is free-running)
      2. The ZCD signal will serve as external synchronization when its period is: 
         Ton + Toffmin < ZCD period < Tonmax + Toff max (converter is locked)
      3. When the ZCD is < Ton + Toff min, the TD1 will lost synchronization with ZCD, but TD1 period
         will remain >= Ton + Toff min (converter operates at its high frequency limit)
      In the above equation,  Ton min < Ton < Ton max
 
  The FAULT1 input is enabled on PA12 (active low) to demonstrate PWM shut down 
  (low level sensitive).
  When the fault is triggered (PA12 input connected to GND, TD1 signal only is
  stopped. The system can be re-armed by pressing the user button.

  LEDs are indicating the following:
  Green LED: blinks during normal operation
  Orange LED: blinks when FAULT is triggered
  
  For more details, refer to Application Note AN4539 "HRTIM cookbook".


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - HRTIM/HRTIM_TM_PFC/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - HRTIM/HRTIM_TM_PFC/Inc/stm32f3xx_it.h          PPP interrupt handlers header file
  - HRTIM/HRTIM_TM_PFC/Inc/main.h                  Header for main.c module  
  - HRTIM/HRTIM_TM_PFC/Src/stm32f3xx_it.c          PPP interrupt handlers
  - HRTIM/HRTIM_TM_PFC/Src/main.c                  Main program
  - HRTIM/HRTIM_TM_PFC/Src/stm32f3xx_hal_msp.c     HAL MSP file

@par Hardware and Software environment 

  - This example runs on STM32F334x4/STM32F334x6/STM32F334x8 devices.
    
  - This example has been tested with STM32F3348-DISCO
    board and can be easily tailored to any other supported device and development board.
  
  - STM32F3348-DISCO Set-up: PA12 must be briefly tied to ground to simulate a fault event
  
  - Two function generators are needed to generate OC and ZCD signals.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
