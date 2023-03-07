/**
  @page PWR_STOP Power Stop Mode Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    PWR/PWR_STOP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Power Stop Mode example.
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

How to enter the Stop mode and wake up from this mode by using the RTC wakeup 
timer event or an interrupt.

The RTC Wakeup Timer Event is connected to EXTI_Line22 or User push-button
EXTI_Line0.

In the associated software
  - the system clock is set to 72 MHz
  - the EXTI_Line0 is configured to generate interrupt on falling edge
  - the EXTI_Line22 connected internally to the RTC Wakeup event is configured
    to generate an interrupt on rising edge each 4s
  - the SysTick is programmed to generate an interrupt each 1 ms 
In the SysTick interrupt handler, LED4 is toggled, this is used to indicate whether
the MCU is in STOP or RUN mode.

The system enters STOP mode and will wait for the RTC Wakeup event to be 
generated each 4s, or Key push button is pressed. 
 - If the RTC WakeUp event (EXTI_Line22) is the source of wakeup from STOP, LED3 is toggled.
 - If the User push-button (EXTI_Line0) is the source of wakeup from STOP, LED6 is toggled.

This behavior is repeated in an infinite loop.

LEDs are used to monitor the system state as following:
 - LED3 ON: system woken up from STOP using RTC WakeUp Interrupt
 - LED4 toggling: system in RUN mode
 - LED5 ON: Initialization error (RTC, RCC,...)
 - LED6 ON: system woken up from STOP using EXTI_Line0 (User push-button)

 @note To measure the current consumption in STOP mode, please refer to 
      @subpage PWR_CurrentConsumption example.

@note This example can not be used in DEBUG mode, this is due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disabled

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select the RTC clock source; in this 
       case the Backup domain will be reset in order to modify the RTC Clock source, as consequence RTC  
       registers (including the backup registers) and RCC_BDCR register are set to their reset values.

@par Directory contents 

  - PWR/PWR_STOP/Inc/stm32f3xx_conf.h         Library Configuration file
  - PWR/PWR_STOP/Inc/stm32f3xx_it.h           Header for stm32f3xx_it.c
  - PWR/PWR_STOP/Inc/main.h                   Header file for main.c
  - PWR/PWR_STOP/Src/system_stm32f3xx.c       STM32F3xx system clock configuration file
  - PWR/PWR_STOP/Src/stm32f3xx_it.c           Interrupt handlers
  - PWR/PWR_STOP/Src/main.c                   Main program
  - PWR/PWR_STOP/Src/stm32f3xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32F303xC devices
    
      
  - This example has been tested with STMicroelectronics STM32F3-Discovery RevB
    evaluation board and can be easily tailored to any other supported device 
    and development board.

  - STM32F3-Discovery RevB Set-up
    - Use LED3, LED4, LED5 and LED6 connected respectively to PE.09, PE.08, PE.10 and PE.15 pins
    - Use the User push-button connected to pin PA.00 (EXTI_Line0)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example



 */
