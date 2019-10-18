/**
  @page TIM_TimeBase Timebase example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TIM/TIM_TimeBase/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM Timebase example
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

Configuration of the TIM peripheral to generate a timebase of 
one second with the corresponding interrupt request.

In this example TIM2 input clock (TIM2CLK) is set to 2 * APB1 clock (PCLK1), 
since APB1 prescaler is different from 1.   
      TIM2CLK = 2 * PCLK1  
      PCLK1 = HCLK / 2 
      => TIM2CLK = HCLK = SystemCoreClock (Hz)
To get TIM2 counter clock at 10 KHz, the Prescaler is computed as following:
   - Prescaler = (TIM2CLK / TIM2 counter clock) - 1

SystemCoreClock is set to 64 MHz for STM32F302R8 Nucleo board.

The TIM2 ARR register value is equal to 10000 - 1, 
Update rate = TIM2 counter clock / (Period + 1) = 1 Hz,
So the TIM2 generates an interrupt each 1 s

When the counter value reaches the auto-reload register value, the TIM upadate 
interrupt is generated and, in the handler routine, PA.15 is toggled with the 
following frequency: 
                        
- PA.15: 0.5Hz 

STM32 Nucleo board's LED2 can be used to monitor the error: when Error_Handler is called, the LED2 toggles 
with a frequency of 1Hz. 
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick timebase is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TIM/TIM_TimeBase/Src/main.c                 Main program
  - TIM/TIM_TimeBase/Src/system_stm32f3xx.c     STM32F3xx system clock configuration file
  - TIM/TIM_TimeBase/Src/stm32f3xx_it.c         Interrupt handlers 
  - TIM/TIM_TimeBase/Src/stm32f3xx_hal_msp.c    HAL MSP module
  - TIM/TIM_TimeBase/Inc/main.h                 Main program header file  
  - TIM/TIM_TimeBase/Inc/stm32f3xx_hal_conf.h   HAL Configuration file
  - TIM/TIM_TimeBase/Inc/stm32f3xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This example runs on STM32F302R8 device.
    
  - This example has been tested with STMicroelectronics STM32F302R8-Nucleo RevC
    boards and can be easily tailored to any other supported device 
    and development board.

  - STM32F302R8-Nucleo RevC Set-up
    - Connect on PA.15 an oscilloscope to show the timebase signal.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
