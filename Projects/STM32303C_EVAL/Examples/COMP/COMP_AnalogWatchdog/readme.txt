/**
  @page COMP_AnalogWatchdog COMP Analog Watchdog example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    COMP/COMP_AnalogWatchdog/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the COMP Analog Watchdog Example
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

How to use a pair of comparator peripherals to compare a voltage level applied on
a GPIO pin to two thresholds: the internal voltage reference (VREFINT) and a fraction
of the internal voltage reference (VREFINT/4), in interrupt mode.

  - The upper threshold is set to VREFINT = 1.22V
  - The lower threshold is set to VREFINT / 4 = 1.22V = 0.305V
  - The input voltage is configured to be connected to PA.01

STM32 Eval board's LEDs can be used to monitor the transfer status:
- LD3 and LD1 are turned On, if the input voltage is above the higher threshold.
- LD2 and LD4 are turned On, if the input voltage is under the lower threshold.
- If the input voltage is within the thresholds, the MCU remains in STOP mode.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - COMP/COMP_AnalogWatchdog/Inc/stm32f3xx_hal_conf.h   HAL configuration file
  - COMP/COMP_AnalogWatchdog/Inc/stm32f3xx_it.h         COMP interrupt handlers header file
  - COMP/COMP_AnalogWatchdog/Inc/main.h                 Header for main.c module
  - COMP/COMP_AnalogWatchdog/Src/stm32f3xx_it.c         COMP interrupt handlers
  - COMP/COMP_AnalogWatchdog/Src/main.c                 Main program
  - COMP/COMP_AnalogWatchdog/Src/system_stm32f3xx.c     STM32F3xx system source file
  - COMP/COMP_AnalogWatchdog/Src/stm32f3xx_hal_msp.c    HAL MSP file


@par Hardware and Software environment

  - This example runs on STM32F3xxx devices.
    
  - This example has been tested with STM32303C-EVAL RevB evaluation board and can be
    easily tailored to any other supported device and development board.    

  - STM32303C-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PE.08, PE.09 ,PE.10
      and PE.11 pins
      @note This example run in stand alone mode.
    - Use a wire to connect Potentiometer RV2 (PC.01 pin) to COMP2 non inverting input (PA.01)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
