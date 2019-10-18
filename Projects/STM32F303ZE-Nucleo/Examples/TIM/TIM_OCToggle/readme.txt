/**
  @page TIM_OCToggle TIM_OCToggle example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    TIM/TIM_OCToggle/readme.txt 
  * @author  MCD Application Team
  * @brief   This example shows how to configure the Timer to generate four different 
  *          signals with four different frequencies.
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

Configuration of the TIM peripheral to generate four different 
signals at four different frequencies.

  The TIM1 frequency is set to SystemCoreClock, and the objective is
  to get TIM1 counter clock at 16 MHz so the Prescaler is computed as following:
     - Prescaler = (TIM1CLK /TIM1 counter clock) - 1
   
  SystemCoreClock is set to 64 MHz for STM32F3xx Devices.

  The TIM1 CCR1 register value is equal to 40961:
  CC1 update rate = TIM1 counter clock / CCR1_Val = 390.615 Hz,
  so the TIM1 Channel 1 generates a periodic signal with a frequency equal to 195.307 Hz.

  The TIM1 CCR2 register value is equal to 20480:
  CC2 update rate = TIM1 counter clock / CCR2_Val = 781.25 Hz,
  so the TIM1 channel 2 generates a periodic signal with a frequency equal to 390.625 Hz.

  The TIM1 CCR3 register value is equal to 10240:
  CC3 update rate = TIM1 counter clock / CCR3_Val = 1562.5 Hz,
  so the TIM1 channel 3 generates a periodic signal with a frequency equal to 781.25 Hz.

  The TIM1 CCR4 register value is equal to 5120:
  CC4 update rate = TIM1 counter clock / CCR4_Val =  3125 Hz,
  so the TIM1 channel 4 generates a periodic signal with a frequency equal to 1562.5 Hz.

@note PWM signal frequency values mentioned above are theoretical (obtained when the system clock frequency
      is exactly 64 MHz). Since the generated system clock frequency may vary from one board to another observed
      PWM signal frequency might be slightly different.
	  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TIM/TIM_OCToggle/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - TIM/TIM_OCToggle/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - TIM/TIM_OCToggle/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OCToggle/Src/stm32f3xx_it.c          Interrupt handlers
  - TIM/TIM_OCToggle/Src/main.c                  Main program
  - TIM/TIM_OCToggle/Src/stm32f3xx_hal_msp.c     HAL MSP file
  - TIM/TIM_OCToggle/Src/system_stm32f3xx.c      STM32F3xx system source file


@par Hardware and Software environment

  - This example runs on STM32F303ZE devices.
    
  - This example has been tested with STMicroelectronics STM32F303ZE-Nucleo Rev B 
    board and can be easily tailored to any other supported device 
    and development board.      

  - STM32F303ZE-Nucleo Rev B Set-up
   Connect the following pins to an oscilloscope to monitor the different waveforms:
      - PE.09: (TIM1_CH1) (Arduino D6 - connected to pin 4 on CN10 Connector)
      - PE.11: (TIM1_CH2) (Arduino D5 - connected to pin 6 on CN10 Connector)
      - PE.13: (TIM1_CH3) (Arduino D3 - connected to pin 10 on CN10 Connector)
      - PE.14: (TIM1_CH4) (connected to pin 28 on CN10 Connector)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
