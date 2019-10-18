/**
  @page FreeRTOS_ThreadCreation FreeRTOS Thread Creation example
 
  @verbatim
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_ThreadCreation/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Thread Creation example.
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

@par Application Description

How to implement thread creation using CMSIS RTOS API. 

This example creates two threads with the same priority, which execute in 
a periodic cycle of 15 seconds. 

In the first 5 seconds, the thread 1 toggles LED5 each 200 ms and the 
thread 2 toggles LED6 each 500 ms.
In the following 5 seconds, the thread 1 suspends itself and the thread 2
continue toggling LED6.
In the last 5 seconds, the thread 2 resumes execution of thread 1 then 
suspends itself, the thread 1 toggles the LED5 each 500 ms.    

@note Care must be taken when using HAL_Delay(), this function provides accurate
      delay (in milliseconds) based on variable incremented in HAL time base ISR.
      This implies that if HAL_Delay() is called from a peripheral ISR process, then
      the HAL time base interrupt must have higher priority (numerically lower) than
      the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority()
      function.
 
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".


@par Directory contents
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/main.c                       Main program
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/stm32f3xx_hal_timebase_tim.c HAL timebase file
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/stm32f3xx_it.c               Interrupt handlers
    - FreeRTOS/FreeRTOS_ThreadCreation/Src/system_stm32f3xx.c           STM32F3xx system clock configuration file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/main.h                       Main program header file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/stm32f3xx_hal_conf.h         HAL Library Configuration file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/stm32f3xx_it.h               Interrupt handlers header file
    - FreeRTOS/FreeRTOS_ThreadCreation/Inc/FreeRTOSConfig.h             FreeRTOS Configuration file

@par Hardware and Software environment

  - This example runs on STM32F334x8 devices.
    
  - This example has been tested with STMicroelectronics STM32F3348-Discovery RevB board and can be
    easily tailored to any other supported device and development board.
    

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
