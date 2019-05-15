/**
  @page ADC_DualMode_18MSamplesPerSec ADC1 and ADC2 in Dual interleaved mode, maximum sampling rate, transfer by DMA

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_DualMode_18MSamplesPerSec/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Dual interleaved mode @18MSamplesPerSec Example
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

This example provides a short description of how to use two ADC peripherals to
convert a regular channel in Dual interleaved mode.

ADC1 and ADC2 are configured to have the maximum sampling rate: 18 Msamples/second.
Note: In this example, conversion accuracy is not set as the highest possible (resolution minimum, sampling time minimum.), but it shows the maximum sampling rate possible.
      Conversion accuracy can be improved by increased resolution and sampling time, but this will decrease the sampling rate.

ADC settings:
  System clock is 72MHz
  HCLOCK is 72MHz            (AHB Prescaler = 1 from SystemClock_Config())
  ADC clock = HCLOCK / 1     (ADC_CLOCK_SYNC_PCLK_DIV1 from ADC_Config())
            = 72 MHz
  Sample time:     1.5 ADC clock cycles (set using ADC_SAMPLETIME_1CYCLE_5).
  Conversion time: 6.5 ADC clock cycles (set using ADC_RESOLUTION_6B (refer to reference manual)).
  Delay time:      3   ADC clock cycles (set using ADC_TWOSAMPLINGDELAY_3CYCLES).

  Note: Delay has been chosen to have ADC2 conversion in the mid-delay between ADC1 conversion.
        See document "ADC timing simulation DualMode 18Msps.xls" describing the conversions timing.

ADC sampling rate calculation:
   - There are 2 ADC conversions every 8 ADC clock cycles (sample cycles+ conversion cycles)
   - This is equivalent to 1 ADC conversion every 4 ADC clock cycles.
   => Sampling frequency is 72MHz/4 = 18 Msamples/sec


ADC conversion results:
  ADC and DMA are configured to operate continuously, in circular mode: conversions results are updated continuoulsy.
  ADC conversions results are transfered automatically by DMA, by half words every ADC1+ADC2 conversions.
  Array "aADCDualConvertedValue" contains both ADC results on 16 bits:
   - ADC master results in the 8 LSB [7:0]
   - ADC slave results in the 8 MSB [15:8]


Board settings:
  ADC1 and ADC2 are configured to convert ADC_CHANNEL_7 (pin PC.01).
  You can modify the voltage on this channel by turning potentiometer RV2.

STM32 board's LEDs are be used to monitor the program execution status:
 - Normal operation: LED1 is toggling when the conversion is complete.
                     The toggle frequency depends on Conversion result link to RV2 position.
                     Slow frequency for ADC full scale result
                     Fast frequency for ADC low scale result
 - Error: In case of error, LED3 is toggling at a frequency of 1Hz.

@note See document "ADC timing simulation DualMode 18Msps.xls" describing the conversions timing.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_DualMode_18MSamplesPerSec/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - ADC/ADC_DualMode_18MSamplesPerSec/Inc/stm32f3xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_DualMode_18MSamplesPerSec/Inc/main.h                  Header for main.c module  
  - ADC/ADC_DualMode_18MSamplesPerSec/Src/stm32f3xx_it.c          DMA interrupt handlers
  - ADC/ADC_DualMode_18MSamplesPerSec/Src/main.c                  Main program
  - ADC/ADC_DualMode_18MSamplesPerSec/Src/stm32f3xx_hal_msp.c     HAL MSP file
  - ADC/ADC_DualMode_18MSamplesPerSec/Src/system_stm32f3xx.c      stm32f3xx system source file

@par Hardware and Software environment 
  - This example runs on STM32F303xE devices.

  - This example has been tested with STM32303E-EVAL RevC board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
