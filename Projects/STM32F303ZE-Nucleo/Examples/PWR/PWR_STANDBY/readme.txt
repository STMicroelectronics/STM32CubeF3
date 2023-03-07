/**
  @page PWR_STANDBY PWR standby example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    PWR/PWR_STANDBY/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR STANDBY example.
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
 
How to enter the Standby mode and wake up from this mode by using an external 
reset or the WKUP pin.

In the associated software, the system clock is set to 64 MHz, an EXTI line
is connected to the User push-button thru PC.13 and configured to generate an 
interrupt on falling edge.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler, LED1 is toggled in order to indicate whether the MCU is in STANDBY or RUN mode.

When a falling edge is detected on the EXTI line, an interrupt is generated. In the 
EXTI handler routine, the wake-up pin PWR_WAKEUP_PIN2 is enabled and the corresponding
wake-up flag cleared. Then, the system enters STANDBY mode causing LED1 to stop toggling. 

A falling edge on WKUP pin will wake-up the system from STANDBY.
Alternatively, an external RESET of the board will lead to a system wake-up as well.

After wake-up from STANDBY mode, program execution restarts in the same way as after
a RESET and LED1 restarts toggling.

Two leds LED1 and LED2 are used to monitor the system state as follows:
 - LED2 ON: configuration failed (system will go to an infinite loop)
 - LED1 toggling: system in RUN mode
 - LED1 off : system in STANDBY mode

These steps are repeated in an infinite loop.

@note To measure the current consumption (IDD), remove jumper JP5
      and replace it by connecting an amperemeter.
      Board configuration requirement: Make sure the solder bridge SB13 is open to have a correct current consumption.


@note This example can not be used in DEBUG mode due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - PWR/PWR_STANDBY/Inc/stm32f3xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY/Inc/stm32f3xx_it.h           Header for stm32f3xx_it.c
  - PWR/PWR_STANDBY/Inc/main.h                   Header file for main.c
  - PWR/PWR_STANDBY/Src/system_stm32f3xx.c       STM32F3xx system clock configuration file
  - PWR/PWR_STANDBY/Src/stm32f3xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY/Src/main.c                   Main program
  - PWR/PWR_STANDBY/Src/stm32f3xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32F303xE devices
    
      
  - This example has been tested with STMicroelectronics STM32F303ZE-Nucleo Rev B
    board and can be easily tailored to any other supported device 
    and development board.

  - STM32F303ZE-Nucleo Rev B Set-up
    - Use LED1 and LED2 connected respectively to PB.00 and PB.07 pins
  
    - User push-button connected to pin PC.13 (EXTI_Line15_10)
    - WakeUp Pin PWR_WAKEUP_PIN2 connected to PC.13

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example



 */
