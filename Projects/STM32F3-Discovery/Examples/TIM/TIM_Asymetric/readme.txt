/**
  @page TIM_Asymetric TIM asymmetric example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TIM/TIM_Asymetric/readme.txt 
  * @author  MCD Application Team
  * @brief   TIM asymmetric Example Description.
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

Configuration of the TIM peripheral to generate an asymmetric signal.
TIM8 is configured to generate an asymmetric signal with a programmable phase-shifted signal on TIM8_CH2.
TIM8 Channel 1 is configured in PWM2 mode.
TIM8 Channel 2 is configured in Asymmetric PWM2 mode.
The counter mode is center aligned.
The pulse length and the phase shift are programmed consecutively in TIM8_CCR2 and TIM8_CCR1.

TIM1 is configured to generating the reference signal on Channel1 used by TIM8:
   - TIM1 is generating a PWM signal with frequency equal to 1.6363 KHz.
   - TIM1 is used as master for TIM8, the update event of TIM1 genarates the Reset counter
     of TIM8 to synchronize the two signals: the refernce signal (TIM1_CH1) and the shifted signal (TIM8_CH2). 

In this example TIM1 and TIM8 input clock (TIM8CLK) is set to APB2 clock (PCLK2)    
TIM1 and TIM8 signals are at frequency of  (SystemCoreClock / (Period + 1))  
               
TIM8 is generating a signal with the following caracteristics:
  - Pulse lenght = (TIM8_CCR1 + TIM8_CCR2) / TIM8_CLK
  - Phase shift = TIM8_CCR2/TIM8_CLK
 with TIM8_CLK = (SystemCoreClock / (Period + 1)), as the prescaler is equal to zero. 
 The Pulse is low level of the output signal of Channel 2 because TIM cannel is configured as asymmetric PWM2 mode.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TIM/TIM_Asymetric/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - TIM/TIM_Asymetric/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - TIM/TIM_Asymetric/Inc/main.h                  Header for main.c module  
  - TIM/TIM_Asymetric/Src/stm32f3xx_it.c          Interrupt handlers
  - TIM/TIM_Asymetric/Src/main.c                  Main program
  - TIM/TIM_Asymetric/Src/stm32f3xx_hal_msp.c     HAL MSP file
  - TIM/TIM_Asymetric/Src/system_stm32f3xx.c      STM32F3xx system source file


@par Hardware and Software environment

  - This example runs on STM32F303xC devices.
    
  - This example has been tested with STMicroelectronics STM32F3-Discovery RevB
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32F3-Discovery RevB Set-up
    - Connect the following pins to an oscilloscope to monitor the different 
      waveforms:
       - PA.08 (TIM1_CH1)
       - PB.06 (TIM8_CH1)
       - PC.07 (TIM8_CH2)
    The shift is measured using the TIM1_CH1 as reference signal.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
