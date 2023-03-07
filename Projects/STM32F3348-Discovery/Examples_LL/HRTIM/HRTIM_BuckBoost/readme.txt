/**
  @page HRTIM_BuckBoost HRTIM example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/HRTIM/HRTIM_BuckBoost/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HRTIM_BuckBoost example.
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

  This example shows how to configure the HRTIM to control a non-inverting 
  buck-boost converter timer.

  This is done using timer unit A and B and TA1/TA2/TB1/TB2 outputs
  (resp. PA8, PA9, PA10, PA11).
  
  The FAULT1 input is enabled on PA12 (active low) to demonstrate PWM shut down 
  (low level sensitive), for all outputs.
  When the fault is triggered (PA12 input connected to GND, TA1, TA2, TB1, TB2
  signals are shut-down. The system can be re-armed by pressing the user button.
  
  The ADC is configured to have conversions triggered in the middle of the
  converter ON time, on PA1 (Vin) and PA3 (Vout) inputs.
  
  To run the demo, the Vin input pin of the BUCK-BOOST converter must be connected
  to the 5V_O supply. The resulting voltage is available on Vout pin.

  The demo starts in BUCK mode, and the duty cycle is slowly adjusted in the 
  TIMA IRQ handler to have Vout continuously varying below Vin value.
  If the push-button is pressed and the voltage is below 5V, the boost mode is 
  enabled (this is to prevent exceeding the discovery kit max output voltage.
  The voltage is increased above Vin value (with a fixed duty cycle).
  If the push-button is pressed again, the output capacitor is first de-energized
  down to 2.5V before re-enabling the buck mode.

  LEDs are indicating the following:
  Green LED5: blinks during BUCK operation
  Blue LED6: blinks during BOOST operation
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

  - HRTIM/HRTIM_BuckBoost/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - HRTIM/HRTIM_BuckBoost/Inc/main.h                  Header for main.c module
  - HRTIM/HRTIM_BuckBoost/Inc/stm32_assert.h          Template file to include assert_failed function
  - HRTIM/HRTIM_BuckBoost/Src/stm32f3xx_it.c          Interrupt handlers
  - HRTIM/HRTIM_BuckBoost/Src/main.c                  Main program
  - HRTIM/HRTIM_BuckBoost/Src/system_stm32f3xx.c      STM32F3xx system source file


@par Hardware and Software environment

  - This example runs on STM32F334x4/STM32F334x6/STM32F334x8 devices.
    
  - This example has been tested with STMicroelectronics STM32F3348-Discovery RevB board and can be
    easily tailored to any other supported device and development board.

  - STMicroelectronics STM32F3348-Discovery RevB Set-up: PA12 should be connected the VDD while testing
    the BUCK and BOOST modes. PA12 must be briefly tied to ground to simulate a fault event.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
