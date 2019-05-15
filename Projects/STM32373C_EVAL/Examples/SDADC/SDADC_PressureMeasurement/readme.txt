/**
  @page SDADC_PressureMeasurement Pressure Measurement using SDADC
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    SDADC/SDADC_PressureMeasurement/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SDADC Pressure Measurement example.
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

This example aims to show how to use the 16-bit resolution Sigma-Delta
Analog-to-Digital converter to perform differential pressure measurement.

This is done using the MPX2102A sensor mounted on the STM32373C-EVAL evaluation board.

 - The SDADC channel 8 is configured in differential mode.
   The external reference VREF (set to 3.3V on STM32373C-EVAL) is used as reference for SDADC.
   The conversion is triggered by TIM19. An interrupt is generated on each end of injected conversion.
   A MPX2102A sensor is connected to SDADC1 channel 8P (PE8) and channel 8N (PE9).
   The MPX2102 sensitivity when powered by 3.3V is 3.3V * 40mV / 10V = 13.2mV/1000mB
                                                                     = 13.2 microV/mB
   To increase the sensitivity an external 45.1 gain using the operation amplifier
   TVS632 mounted on STM32373C-EVAL is used. The same operation amplifier is
   used to shift up the input voltage by 3.3V/10.0f = 0.33V
   Refer to STM32373C-EVAL user manual for more details about how the MPX2102 is
   connected to PE8 and PE9

 - The measured pressure is displayed on the LCD mounted on STM32373C-EVAL.
   In order to get an accurate measurement of pressure a calibration phase 
   should be added using a known pressure value.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - SDADC/SDADC_PressureMeasurement/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - SDADC/SDADC_PressureMeasurement/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - SDADC/SDADC_PressureMeasurement/Inc/main.h                  Header for main.c module  
  - SDADC/SDADC_PressureMeasurement/Src/stm32f3xx_it.c          Interrupt handlers 
  - SDADC/SDADC_PressureMeasurement/Src/stm32f3xx_hal_msp.c.c   HAL MSP module
  - SDADC/SDADC_PressureMeasurement/Src/main.c                  Main program
  - SDADC/SDADC_PressureMeasurement/Src/system_stm32f3xx.c      STM32F3xx system source file
  
@par Hardware and Software environment

  - This example runs on STM32F373xC devices.
  
  - This example has been tested with STMicroelectronics STM32373C-EVAL RevB
    boards and can be easily tailored to any other supported device 
    and development board.

  - STM32373xC-EVAL Set-up
    - Make sure that jumper JP16 and JP17 are fitted.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
