/**
  @page HRTIM HRTIM_Multiphase

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    HRTIM/HRTIM_Multiphase/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of HRTIM_DualBuck example.
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
  This example shows how to configure the HRTIM to control a multiphase 
  buck converter. It handles here 5-phases on timer unit A, B C and D
  and outputs TA2, TB1, TC2, TD1, TD2.
  
  The FAULT1 input is enabled on PA12 (active low) to demonstrate PWM shut down 
  (low level sensitive), for all outputs.
  When the fault is triggered (PA12 input connected to GND),
  TA2, TB1, TC2, TD1, TD2 (resp. PA9, PA10, PB13, PB14 and PB15) signals
  are shut-down. The system can be re-armed by pressing the user button.
  
  The ADC is configured to have conversions triggered in the middle of the
  converter ON time of each of 5 phases, on PA2 input, for this example
  (usually a sequence of 5 conversion on 5 inputs).
  
  The demo starts in 5-phase mode.
  If the push-button is pressed, the demo mode changes so that all phase shedding
  options are scanned: from 5 to 1-phase, and finally burst mode.

  LEDs are indicating the following:
  Green LED5 blinking: 5-phase operation
  Blue LED6 blinking: 4-phase operation
  Green LED5 continuous: 3-phase operation
  Blue LED6 continuous: 2-phase operation
  Both Blue and Green LEDs continuous: 1-phase operation
  Both Blue and Green LEDs blinking: Burst mode operation
  Red LED3: blinks when FAULT is triggered
  Orange LED4: indicates the occurrence and duration of the PWM update ISR.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - HRTIM/HRTIM_Multiphase/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - HRTIM/HRTIM_Multiphase/Inc/stm32f3xx_it.h          PPP interrupt handlers header file
  - HRTIM/HRTIM_Multiphase/Inc/main.h                  Header for main.c module  
  - HRTIM/HRTIM_Multiphase/Src/stm32f3xx_it.c          PPP interrupt handlers
  - HRTIM/HRTIM_Multiphase/Src/main.c                  Main program
  - HRTIM/HRTIM_Multiphase/Src/stm32f3xx_hal_msp.c     HAL MSP file

@par Hardware and Software environment 

  - This example runs on STM32F334x4/STM32F334x6/STM32F334x8 devices.
    
  - This example has been tested with STM32F3348-DISCO
    board and can be easily tailored to any other supported device and development board.
  
  - STM32F3348-DISCO Set-up: PA12 must be briefly tied to ground to simulate a fault event

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
