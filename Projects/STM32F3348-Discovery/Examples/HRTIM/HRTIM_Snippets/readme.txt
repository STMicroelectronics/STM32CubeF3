/**
  @page HRTIM HRTIM_Snippets

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    HRTIM/HRTIM_Snippets/readme.txt 
  * @author  MCD Application Team
  * @brief   This example describes how to generate basic PWM waveforms with the 
  * HRTIM, as per HRTIM Cookbook basic examples.
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
  This example describes how to generate basic PWM waveforms with the HRTIM,
  using HRTIM Cookbook basic examples.

  5 basic examples are provided. Its is necessary to uncomment one of the 
  following 5 #define statement at the beginning of the main.c module:

  #define HRTIM_CHECK: basic HRTIM outputs check
  #define SINGLE_PWM: elementary PWM generation
  #define MULTIPLE_PWM: PWM on multiple outputs
  #define PWM_MASTER: PWM generation with timers other than the timing unit itself
  #define ARBITRARY_WAVEFORM: non-PWM waveform example

  The PWM and waveforms are generated on the TD1 and TD2 outputs
  (resp. PB14 and PB15) and TA1 and TA2 outputs (resp. PA8 and PA9), depending 
  on example.

  Green LED: blinks during normal operation
  Red LED: blinks when the ERROR handler is entered


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - HRTIM/HRTIM_Snippsets/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - HRTIM/HRTIM_Snippsets/Inc/stm32f3xx_it.h          PPP interrupt handlers header file
  - HRTIM/HRTIM_Snippsets/Inc/main.h                  Header for main.c module  
  - HRTIM/HRTIM_Snippsets/Src/stm32f3xx_it.c          PPP interrupt handlers
  - HRTIM/HRTIM_Snippsets/Src/main.c                  Main program
  - HRTIM/HRTIM_Snippsets/Src/stm32f3xx_hal_msp.c     HAL MSP file

@par Hardware and Software environment 

  - This example runs on STM32F334x4/STM32F334x6/STM32F334x8 devices.
    
  - This example has been tested with STM32F3348-DISCO
    board and can be easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
