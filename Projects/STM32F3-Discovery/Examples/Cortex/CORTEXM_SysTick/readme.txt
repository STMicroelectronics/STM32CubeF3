    /**
  @page CORTEXM_SysTick CORTEXM SysTick example

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CORTEXM/CORTEXM_SysTick/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CortexM SysTick example.
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

How to use the default SysTick configuration with a 1 ms timebase to toggle LEDs

  The system clock is set to 72 MHz for STM32F3 devices, the SysTick is 
  clocked by the AHB clock(HCLK).

  A "HAL_Delay" function is implemented based on the SysTick end-of-count event.
  Four LEDs are toggled with a timing defined by the HAL_Delay function.
 
  @note: Caution, the SysTick time base 1 msec must not be changed due to use 
         of these time base by HAL driver.

@par Directory contents 

  - CortexM/CORTEXM_SysTick/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - CortexM/CORTEXM_SysTick/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - CortexM/CORTEXM_SysTick/Inc/main.h                     Header for main.c module  
  - CortexM/CORTEXM_SysTick/Src/stm32f3xx_it.c          Interrupt handlers
  - CortexM/CORTEXM_SysTick/Src/main.c                     Main program
  - CortexM/CORTEXM_SysTick/Src/system_stm32f3xx.c      stm32f3xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F303xC device.

  - This example has been tested with STM32F3_DISCOVERY board and can be
    easily tailored to any other supported device and development board.    


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
