/**
  @page TIM_DMA TIM DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TIM/TIM_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM DMA example.
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

Use of the DMA with TIMER Update request 
to transfer data from memory to TIMER Capture Compare Register 3 (TIMx_CCR3).

  This is done using TIM19 Update request and TIM15 CCR1 register.
  The following configuration values are used in this example:

    - TIM15CLK = SystemCoreClock
    - Counter repetion = 3 
    - Prescaler = 0 
    - TIM15 counter clock = SystemCoreClock
    - SystemCoreClock is set to 72 MHz for STM32F3xx

  The objective is to configure TIM15 channel 1 to generate complementary PWM 
  (Pulse Width Modulation) signal with a frequency equal to 17.57 KHz, and a variable 
  duty cycle that is changed by the DMA after a specific number of Update DMA request.

  The number of this repetitive requests is defined by the TIM15 Repetition counter,
  each 4 Update Requests, the TIM15 Channel 1 Duty Cycle changes to the next new 
  value defined by the aCCValue_Buffer.

The PWM waveforms can be displayed using an oscilloscope.



@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TIM/TIM_DMA/Src/main.c                 Main program
  - TIM/TIM_DMA/Src/system_stm32f3xx.c     STM32F3xx system clock configuration file
  - TIM/TIM_DMA/Src/stm32f3xx_it.c         Interrupt handlers 
  - TIM/TIM_DMA/Src/stm32f3xx_hal_msp.c    HAL MSP module
  - TIM/TIM_DMA/Inc/main.h                 Main program header file  
  - TIM/TIM_DMA/Inc/stm32f3xx_hal_conf.h   HAL Configuration file
  - TIM/TIM_DMA/Inc/stm32f3xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This example runs on STM32F373xC devices.
    
  - This example has been tested with STMicroelectronics STM32373C-EVAL RevB
    boards and can be easily tailored to any other supported device 
    and development board.

  - STM32373C-EVAL RevB Set-up
    - Connect the TIM15 pin to an oscilloscope to monitor the different waveforms: 
        - TIM15 CH1  (PB.06)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
