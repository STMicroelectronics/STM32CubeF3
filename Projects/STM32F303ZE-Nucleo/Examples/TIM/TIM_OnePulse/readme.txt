/**
  @page TIM_OnePulse TIM One Pulse example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TIM/TIM_OnePulse/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the TIM One Pulse example.      
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

Use of the TIM peripheral to generate a single pulse when 
an external signal rising edge is received on the timer input pin.

Clock setup for TIM1
================================

  TIM1CLK = SystemCoreClock = 64 MHz.
  
  Prescaler = (TIM1CLK /TIM1 counter clock) - 1
  
  The prescaler value is computed in order to have TIM1 counter clock 
  set at 16000000 Hz.
  
  The Autoreload value is 65535 (TIM1->ARR), so the maximum frequency value to 
  trigger the TIM1 input is 16000000/65535 [Hz].
 
Configuration of TIM1 in One Pulse Mode
===================================================
 
  - The external signal is connected to TIM1_CH2 pin (PE.11), 
    and a rising edge on this input is used to trigger the Timer.
  - The One Pulse signal is output on TIM1_CH1 (PE.09).

  The delay value is fixed to:
   - Delay =  CCR1/TIM1 counter clock 
           = 16383 / 16000000 [sec]
           
  The pulse value is fixed to : 
   - Pulse value = (TIM_Period - TIM_Pulse)/TIM1 counter clock  
                 = (65535 - 16383) / 16000000 [sec]

  The one pulse waveform can be displayed using an oscilloscope and it looks
  like this.
  
                               ____
                               |   |
  CH2 _________________________|   |__________________________________________
 
                                             ___________________________
                                            |                           |
  CH1 ______________________________________|                           |_____
                               <---Delay----><------Pulse--------------->
  


@par Directory contents 

  - TIM/TIM_OnePulse/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - TIM/TIM_OnePulse/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - TIM/TIM_OnePulse/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OnePulse/Src/stm32f3xx_it.c          Interrupt handlers
  - TIM/TIM_OnePulse/Src/main.c                  Main program
  - TIM/TIM_OnePulse/Src/stm32f3xx_hal_msp.c     HAL MSP file
  - TIM/TIM_OnePulse/Src/system_stm32f3xx.c      STM32F3xx system source file


@par Hardware and Software environment

   - This example runs on STM32F303ZE devices.
   - In this example, the clock is set to 64 MHz.
    
  - This example has been tested with STM32F303ZE-Nucleo Rev B board and can be
    easily tailored to any other supported device and development board.

  - STM32F303ZE-Nucleo Rev B Set-up
   - Connect the external signal to the TIM1_CH2 pin (PE.11) (Arduino D5 - connected to pin 6 on CN10 Connector)
   - Connect the TIM1_CH1 pin(PE.09) (Arduino D6 - connected to pin 4 on CN10 Connector) to an oscilloscope to monitor the waveform.  


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
