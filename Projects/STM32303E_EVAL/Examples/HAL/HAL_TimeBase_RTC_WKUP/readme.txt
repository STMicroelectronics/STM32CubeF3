/**
  @page HAL_TimeBase_RTC_WKUP HAL TimeBase RTC WakeUp
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    HAL/HAL_TimeBase_RTC_WAKUP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the HAL TimeBase RTC WakeUp example.
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

How to customize HAL using RTC wakeup as main source of time base, 
instead of Systick.

The Key push-button is used to suspend or resume tick increment. 

Each time the button is pressed; an interrupt is generated (EXTI_Line9-5)
and in the ISR the uwIncrementState is checked:
  1- If the uwIncrementState = 0: the tick increment is suspended by calling 
     HAL_SuspendTick() API (RTC wakeup timer interrupt is disabled).
  2- If the uwIncrementState = 1: the tick increment is Resumed by calling 
     HAL_ResumeTick() API(RTC wakeup timer interrupt is enabled).

The wakeup feature is configured to assert an interrupt each 1ms  

The example brings, in user file, a new implementation of the following HAL weak functions:

HAL_InitTick() 
HAL_SuspendTick()
HAL_ResumeTick()

This implementation will overwrite native implementation in stm32f3xx_hal.c
and so user functions will be invoked instead when called.

The following time base functions are kept as implemented natively:

HAL_IncTick()
HAL_Delay()

In an infinite loop, LED1 toggles spaced out over 500ms delay, except when tick increment is suspended.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents  

  - HAL/HAL_TimeBase_RTC_WKUP/Inc/stm32f3xx_hal_conf.h                HAL configuration file
  - HAL/HAL_TimeBase_RTC_WKUP/Inc/stm32f3xx_it.h                      Interrupt handlers header file
  - HAL/HAL_TimeBase_RTC_WKUP/Inc/main.h                              Header for main.c module  
  - HAL/HAL_TimeBase_RTC_WKUP/Src/stm32f3xx_it.c                      Interrupt handlers
  - HAL/HAL_TimeBase_RTC_WKUP/Src/main.c                              Main program
  - HAL/HAL_TimeBase_RTC_WKUP/Src/stm32f3xx_hal_timebase_rtc_wakeup.c HAL time base functions
  - HAL/HAL_TimeBase_RTC_WKUP/Src/system_stm32f3xx.c                  STM32F3xx system source file

@par Hardware and Software environment

  - This example runs on STM32F303xE devices.
    
  - This example has been tested with STMicroelectronics STM32303E-EVAL RevC board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
