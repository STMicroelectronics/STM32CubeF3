/**
  @page ADC_AnalogWatchdog ADC conversion example with analog watchdog, using 
   related peripherals (GPIO, DMA, Timer), voltage input from DAC, user control 
   by push button and LED

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_AnalogWatchdog/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC conversion example
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

This example provides a short description of how to use the ADC peripheral to
perform conversions with analog watchdog and out-of-window interruptions enabled.

Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - Timer peripheral is used to trig ADC conversions.
 - DMA peripheral is used to transfer ADC conversions data.

ADC settings:
  Trig of conversion start done by external event (timer at 1kHz).
  Continuous mode disabled to have only 1 conversion at each conversion trig.
  Analog watchdog thresholds are set between 1/8 and 5/8 of full range (between
  0.41V and 2.06V with full range of 3.3V).
  
ADC conversion results:
 - ADC conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedValues".
 - When DMA transfer half-buffer and buffer length are reached, callbacks 
   HAL_ADC_ConvCpltCallback() and HAL_ADC_ConvCpltCallback() are called.

Board settings:
 - ADC is configured to convert ADC_CHANNEL_1 (pin PA.04).
 - The voltage input on ADC channel is provided from DAC channel.
   ADC and DAC channel have been chosen to have the same pad shared at device level: pin PA.04.
   ==> Therefore, there is no external connection needed to run this example.
 - Voltage is increasing at each click on push button, from 0 to maximum range in 4 steps.
   Clicks on push button follow circular cycles: At clicks counter maximum value reached, counter is set back to 0.


STM32F303RE-Nucleo Rev C board's LED is be used to monitor the program execution status:
 - Normal operation: LED2 is turned-on/off in function of ADC conversion
   result.
    - Turned-off if voltage into of AWD window
    - Turned-on if voltage is out of AWD window
 - Error: In case of error, LED2 is toggling at a frequency of 1Hz.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_AnalogWatchdog/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - ADC/ADC_AnalogWatchdog/Inc/stm32f3xx_it.h          HAL interrupt handlers header file
  - ADC/ADC_AnalogWatchdog/Inc/main.h                  Header for main.c module  
  - ADC/ADC_AnalogWatchdog/Src/stm32f3xx_it.c          HAL interrupt handlers
  - ADC/ADC_AnalogWatchdog/Src/main.c                  Main program
  - ADC/ADC_AnalogWatchdog/Src/stm32f3xx_hal_msp.c     HAL MSP file
  - ADC/ADC_AnalogWatchdog/Src/system_stm32f3xx.c      STM32F3xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F303RE devices.

  - This example has been tested with STM32F303RE-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
