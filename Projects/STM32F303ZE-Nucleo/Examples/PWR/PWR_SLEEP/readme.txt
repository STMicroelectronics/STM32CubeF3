/**
  @page PWR_SLEEP Power sleep Mode Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    PWR/PWR_SLEEP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power Sleep Mode example.
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

How to enter the Sleep mode and wake up from this mode by using an interrupt.

In the associated software, the system clock is set to 64 MHz.
an EXTI line is connected to the user button thru PC.13 and configured 
to generate an interrupt on falling edge upon key press.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick 
interrupt handler, LED1 is toggled in order to indicate whether the MCU is in SLEEP mode 
or RUN mode.

5 seconds after start-up, the system automatically enters SLEEP mode and 
LED1 stops toggling.
The User push-button can be pressed at any time to wake-up the system. 
The software then comes back in RUN mode for 5 sec. before automatically entering SLEEP mode again. 

LED1 is used to monitor the system state as follows:
 - LED1 toggling: system in RUN mode
 - LED1 off : system in SLEEP mode

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

  - PWR/PWR_SLEEP/Inc/stm32f3xx_conf.h         HAL Configuration file
  - PWR/PWR_SLEEP/Inc/stm32f3xx_it.h           Header for stm32f3xx_it.c
  - PWR/PWR_SLEEP/Inc/main.h                   Header file for main.c
  - PWR/PWR_SLEEP/Src/system_stm32f3xx.c       STM32F3xx system clock configuration file
  - PWR/PWR_SLEEP/Src/stm32f3xx_it.c           Interrupt handlers
  - PWR/PWR_SLEEP/Src/main.c                   Main program

@par Hardware and Software environment

  - This example runs on STM32F3xx devices

  - This example has been tested with STMicroelectronics STM32F303ZE-Nucleo Rev B
    board and can be easily tailored to any other supported device 
    and development board.

  - STM32F303ZE-Nucleo Rev B set-up:
    - LED1 connected to PB.00 pin
    - Use the User push-button connected to pin PC.13 (EXTI_Line15_10)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
