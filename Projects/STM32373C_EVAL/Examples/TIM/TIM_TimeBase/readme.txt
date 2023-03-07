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

Configuration of the TIM peripheral to generate a timebase of 
one second with the corresponding interrupt request.

In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 2 
      => TIM3CLK = HCLK = SystemCoreClock (Hz)
To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
   - Prescaler = (TIM3CLK / TIM3 counter clock) - 1

SystemCoreClock is set to 72MHz for STM32F3xx Devices.

The TIM3 ARR register value is equal to 10000 - 1, 
Update rate = TIM3 counter clock / (Period + 1) = 1 Hz,
So the TIM3 generates an interrupt each 1 s

When the counter value reaches the auto-reload register value, the TIM update 
interrupt is generated and, in the handler routine, PC.00 is toggled with the 
following frequency: 

- PC.00: 0.5Hz 



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

  - This example runs on STM32F373xC devices.
    
  - This example has been tested with STMicroelectronics STM32373C-EVAL RevB
    boards and can be easily tailored to any other supported device 
    and development board.

  - STM32373C-EVAL RevB Set-up
    - Use LED1 connected to PC.00 pin and connect them on an oscilloscope 
      to show the timebase signal.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
