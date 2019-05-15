/**
  @page PWR_PVD PWR Programmable Voltage Detector (PVD) example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    PWR/PWR_PVD/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR Programmable Voltage Detector (PVD) example
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
This example shows how to configure the programmable voltage detector using
an external interrupt line.

External DC supply has to be used to power Vdd.
In this example, EXTI line 16 is configured to generate 
an interrupt on each rising or falling edge of the PVD output signal (which 
indicates that the Vdd voltage is moving below or above the PVD threshold).
As long as the voltage is above the target threshold, the green LED (LED5) is blinking; 
when the voltage drops below the threshold, the green LED stops blinking and remains 
constantly on;
when the voltage moves back above the target threshold, the green LED starts blinking again.

If the green LED is slowly toggling (1 sec. period), it means that the system has generated an error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - PWR/PWR_PVD/Inc/stm32f3xx_hal_conf.h HAL Configuration file
  - PWR/PWR_PVD/Inc/stm32f3xx_it.h       Header for stm32f3xx_it.c
  - PWR/PWR_PVD/Inc/main.h               Header file for main.c
  - PWR/PWR_PVD/Src/system_stm32f3xx.c   STM32F3xx system clock configuration file
  - PWR/PWR_PVD/Src/stm32f3xx_it.c       Interrupt handlers
  - PWR/PWR_PVD/Src/main.c               Main program

@par Hardware and Software environment

  - This example runs on STM32F334xx device.   
  - This example has been tested with STMicroelectronics STM32 F334 Discovery
    kit and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
