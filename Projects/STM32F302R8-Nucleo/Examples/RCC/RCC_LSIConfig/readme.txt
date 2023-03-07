/**
  @page RCC_LSIConfig RCC Clock Config example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    RCC/RCC_LSIConfig/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RCC Clock Config example.
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

Enabling/disabling of the low-speed internal (LSI) RC oscillator (about 32 KHz) at run time, using the RCC HAL API. 

Board: STM32F302R8-Nucleo Rev C (embeds a STM32F302R8T6 device)
Microcontroller Clock Output MCO Pin:PA.08

   _________________________                   _______________________ 
  |           ______________|                 |  _______________      |
  |          |RCC           |                 | |               | O   |
  |          |              |                 | |   _   _   _   | O   |
  |          |          MCO |_________________| | _| |_| |_| |_ | --  |
  |          |              |                 | |_______________| --  |    
  |          |              |                 |                       |
  |          |              |                 |_oscilloscope__________|
  |          |              |                    
  |          |              | 
  |          |______________|  
  |                         |
  |                         |
  |_STM32_Board ____________|



In this example, after startup SYSCLK is configured to the max frequency using the PLL with
HSI as clock source, the User push-button (connected to EXTI_Line15_10)
is used to enable/disable LSI:
each time the User push-button is pressed, EXTI_Line15_10 interrupt is generated and the ISR
enables the LSI when it is off, disables it when it is on.

LED_GREEN is toggling as follows according to LSI activation:
- when LSI is on, LED_GREEN is emitting a double flash every half-second
- when LSI is off, LED_GREEN is fast-toggling (every 100 ms) 

LSI clock waveform is available on the MCO pin PA.08 and can be captured
on an oscilloscope.

In case of configuration error, LED_GREEN is slowly toggling (1 sec. period).

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - RCC/RCC_LSIConfig/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - RCC/RCC_LSIConfig/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - RCC/RCC_LSIConfig/Inc/main.h                  Header for main.c module  
  - RCC/RCC_LSIConfig/Src/stm32f3xx_it.c          Interrupt handlers
  - RCC/RCC_LSIConfig/Src/main.c                  Main program
  - RCC/RCC_LSIConfig/Src/system_stm32f3xx.c      STM32F3xx system source file

@par Hardware and Software environment

  - This example runs on STM32F302x8 device.
    
  - This example has been tested with STM32F302R8-Nucleo Rev C 
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
