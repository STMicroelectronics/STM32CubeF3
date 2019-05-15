/**
  @page TIM_Combined TIM combined example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TIM/TIM_Asymetric/readme.txt 
  * @author  MCD Application Team
  * @brief   TIM Combined Example Description.
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example shows how to configure the TIM peripheral to generate 3 PWM combined 
signals.

This is done using TIM1 peripheral to generate 3 PWM combined signals with TIM1 Channel5.

TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)=> TIM1CLK = PCLK2 = SystemCoreClock
TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
SystemCoreClock is set to 72 MHz for STM32F303xC devices.

The objective is to generate 3 combined PWM signal at 8.78 KHz (in center aligned mode):
    - TIM1_Period = (SystemCoreClock / (8.78*2)) - 1
 The channel 1  duty cycle is set to 50%.
 The channel 2  duty cycle is set to 37.5%.
 The channel 3  duty cycle is set to 25%.

The Timer pulse is calculated as follows:
  - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
  
The channel 5  is used in PWM2 mode with duty cycle set to 6.22%.

The 3 resulting signals are made of an AND logical combination of two reference PWMs:
  - Channel 1 and Channel 5
  - Channel 2 and Channel 5
  - Channel 3 and Channel 5

The TIM1 waveform can be displayed using an oscilloscope.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TIM/TIM_Combined/Src/main.c                 Main program
  - TIM/TIM_Combined/Src/system_stm32f3xx.c     STM32F3xx system clock configuration file
  - TIM/TIM_Combined/Src/stm32f3xx_it.c         Interrupt handlers 
  - TIM/TIM_Combined/Src/stm32f3xx_hal_msp.c    HAL MSP module
  - TIM/TIM_Combined/Inc/main.h                 Main program header file  
  - TIM/TIM_Combined/Inc/stm32f3xx_hal_conf.h   HAL Configuration file
  - TIM/TIM_Combined/Inc/stm32f3xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This example runs on STM32F303xC devices.
    
  - This example has been tested with STMicroelectronics STM32F3-Discovery RevB
    boards and can be easily tailored to any other supported device 
    and development board.

  - STM32F3-Discovery RevB Set-up
    - Connect the following pins to an oscilloscope to monitor the different 
      waveforms:
       - PA.08 (TIM1_CH1)
       - PA.09 (TIM1_CH2)
       - PA.10 (TIM1_CH3)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
