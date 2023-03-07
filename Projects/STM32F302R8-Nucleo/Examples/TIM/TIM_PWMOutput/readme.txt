/**
  @page TIM_PWMOutput TIM PWM Output example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TIM/TIM_PWMOutput/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWM signals generation using TIM2
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

Configuration of the TIM peripheral in PWM (pulse width modulation) mode.

SystemCoreClock is set to 64 MHz for STM32F3xx Devices.

In this example TIM2 input clock (TIM2CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM2CLK = 2 * PCLK1  
      PCLK1 = HCLK / 2 
      => TIM2CLK = HCLK = SystemCoreClock
          
    To get TIM2 counter clock at 16 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM2 counter clock) - 1
       Prescaler = (SystemCoreClock /16 MHz) - 1
                                              
    To get TIM3 output clock at 36 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM3 counter clock / TIM3 output clock) - 1
           = 443
                  
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%


The PWM waveforms can be displayed using an oscilloscope.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - TIM/TIM_PWMOutput/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PWMOutput/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMOutput/Inc/main.h                  Header for main.c module  
  - TIM/TIM_PWMOutput/Src/stm32f3xx_it.c          Interrupt handlers
  - TIM/TIM_PWMOutput/Src/main.c                  Main program
  - TIM/TIM_PWMOutput/Src/stm32f3xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PWMOutput/Src/system_stm32f3xx.c      STM32F3xx system source file


@par Hardware and Software environment

  - This example runs on STM32F302R8 devices.
  - In this example, the clock is set to 64 MHz.
    
  - This example has been tested with STMicroelectronics STM32F302R8-Nucleo Rev C 
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32F302R8-Nucleo Rev C Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
        - TIM2_CH1 : PA.15 (connected to pin 17 in CN7 connector)
        - TIM2_CH2 : PB.03 (connected to pin 31 in CN10 connector)
        - TIM2_CH3 : PB.10 (connected to pin 25 in CN10 connector)
        - TIM2_CH4 : PA.10 (connected to pin 33 in CN10 connector)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
