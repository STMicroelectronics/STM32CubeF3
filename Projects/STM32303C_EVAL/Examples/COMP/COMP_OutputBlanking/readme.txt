/**
  @page COMP_OutputBlanking COMP output blanking example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    COMP/COMP_OutputBlanking/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the COMP output blanking Example
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

How to use the comparator-peripheral output blanking feature. The purpose of the
output blanking feature in motor control is to prevent tripping of the current
regulation upon short current spikes at the beginning of the PWM period.

In this example COMP1 and TIM1 are used:
COMP1 is configured as following:
 - Non Inverting input is connected to PA1
 - Inverting input is connected to VREFINT (1.22V)
 - Output is available on PA0
 - Blanking source is connected internally to TIM1 OC5

TIM1 is configured as following:
 - TIM1 period is APB2 clock / period = 72000000 / 50000 = 1440 Hz.
 - TIM1 OC5 (internal channel: not available on GPIO) configured in PWM mode
   with a frequency equal to 1440 Hz and high level equal to 
   pulse / period = 2000 / 72000000 = 27.7 micro second.
 - Timer channel intended to be connected to an external device (motor, ...) in user application:
   TIM1 CH2 (PA9) configured in PWM mode with a frequency equal to 1440 Hz
   and duty cycle pulse/period = 100 * (37500 / 50000) =  75%.

Connection needed:
 - either connect COMP1 input (PA1) to TIM1 CH2 (PA9).
 - either connect COMP1 input (PA1) to a signal generator, at the frequency of TIM1 period (to have comparator input signal frequency matching blanking signal frequency. To do this, possibility to use TIM1 CH2 (PA9) as trigger of signal generator).

Comparator output signal expected:
 - Between t=0us and t=27.7us: Comparator positive input at level high, blanking source at level high.
   Blanking source is high, therefore comparator output is forced to level low.
   (otherwise, as comparator positive input is higher than negative input, comparator output would have been high.)
 - Between t=27.7us and t=duty cycle pulse: Comparator positive input at level high, blanking source at level low.
   Comparator positive input is higher than negative input, blanking source is low, therefore comparator output is high.
 - Between t=duty cycle pulse and t=period: Comparator positive input at level low, blanking source at level low.
   Comparator positive input is higher than negative input, therefore comparator output is low.


@note Care must be taken when using HAL_Delay(), this function provides 
      accurate delay (in milliseconds) based on variable incremented in SysTick ISR. 
      This implies that if HAL_Delay() is called from a peripheral ISR process, then 
      the SysTick interrupt must have higher priority (numerically lower) than the 
      peripheral interrupt. Otherwise the caller ISR process will be blocked. 
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set 
      to 1 millisecond to have correct HAL operation.

@par Directory contents 

  - COMP/COMP_OutputBlanking/Inc/stm32f3xx_hal_conf.h   HAL configuration file
  - COMP/COMP_OutputBlanking/Inc/stm32f3xx_it.h         COMP interrupt handlers header file
  - COMP/COMP_OutputBlanking/Inc/main.h                 Header for main.c module
  - COMP/COMP_OutputBlanking/Src/stm32f3xx_it.c         COMP interrupt handlers
  - COMP/COMP_OutputBlanking/Src/main.c                 Main program
  - COMP/COMP_OutputBlanking/Src/stm32f3xx_hal_msp.c    HAL MSP file
  - COMP/COMP_OutputBlanking/Src/system_stm32f3xx.c     STM32F3xx system source file


@par Hardware and Software environment

  - This example runs on STM32F303xC devices.

  - This example has been tested with STM32303C-EVAL RevC board and can be
    easily tailored to any other supported device and development board.

  - STM32303C-EVAL RevC Set-up
    - Connect TIM1 CH2 (PA9) to a scope and to the input trigger of a wave generator
    - Connect the wave generator ouput to COMP1 Non Inverting input (PA1)
    - Configure the wave generator to output a pulse at rising edge event with 
      high level greater than VREFINT (1.22V) and pulse duration lower 27.7 micro second
      (as an example you can use 10 micro second). 
      You can change the pulse delay and note that the break event is generated
      only when the pulse is triggered after 27.7 micro second


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
