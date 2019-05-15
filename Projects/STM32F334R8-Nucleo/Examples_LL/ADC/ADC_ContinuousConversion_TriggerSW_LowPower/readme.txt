/**
  @page ADC_ContinuousConversion_TriggerSW_LowPower ADC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/ADC/ADC_ContinuousConversion_TriggerSW_LowPower/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_ContinuousConversion_TriggerSW_LowPower example.
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
This example describes how to use a ADC peripheral with ADC low power features; 
This example is based on the STM32F3xx ADC LL API; 
peripheral initialization done using LL unitary services functions
for optimization purpose (performance and size).

Description of ADC low power features:
ADC conversions occurrences are limited to the minimum necessary
to reduce power consumption.
These features are intended to be used by applications needing ADC
conversions at few occurrences and without latency constraints.
ADC low power features are used most commonly with programming
model: polling.


Example configuration:
ADC is configured to convert a single channel, in continuous conversion mode,
from SW trigger.
ADC low power feature enabled: auto wait.
Note: On some other STM32 series, an additional ADC low feature is available: 
      auto power-off.

Example execution:
From the first press on push button, the ADC converts the selected channel
continuously, but with conversions managed by ADC mode low-power:
After the first trigger (software start in this example), following conversions
are not launched successively automatically: the ADC performs 1 conversion
and remains idle (ADC does not perform any other conversion)
until conversion data has been processed by user software.
Note: this is the goal of low power feature auto-wait: to automatically adapt
      the ADC conversions trigs to the speed of the software that reads the data. 
      Moreover, this avoids risk of overrun for low frequency applications.
At each press of push button, software reads the ADC conversion data
and stores it into a variable.
This action automatically trigs another ADC conversion start.
LED is turned on.
Note: the ADC conversion data read is corresponding to previous
      ADC conversion start, independently of delay during which ADC was idle.
      Therefore, the ADC conversion data may be outdated: does not correspond 
      to the current voltage level on the selected ADC channel.

For debug: variables to monitor with debugger watch window:
 - "uhADCxConvertedData": ADC group regular conversion data
 - "uhADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values

Connection needed:
None.
Note: Optionally, a voltage can be supplied to the analog input pin (cf pin below),
      between 0V and Vdda=3.3V, to perform a ADC conversion on a determined
      voltage level.
      Otherwise, this pin can be let floating (in this case ADC conversion data
      will be undetermined).

Other peripherals used:
  1 GPIO for push button
  1 GPIO for LED
  1 GPIO for analog input: PA.04 (Arduino connector CN8 pin A2, Morpho connector CN7 pin 32)


@par Directory contents 

  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower/Inc/main.h                  Header for main.c module
  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower/Src/stm32f3xx_it.c          Interrupt handlers
  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower/Src/main.c                  Main program
  - ADC/ADC_ContinuousConversion_TriggerSW_LowPower/Src/system_stm32f3xx.c      STM32F3xx system source file


@par Hardware and Software environment

  - This example runs on STM32F334xx devices.
    
  - This example has been tested with STM32F334R8-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
