/**
  @page STM32F3348-Discovery_Demo  Demonstration 
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Demonstration/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F3348-Discovery Demonstration
  ******************************************************************************
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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

@par Demo Description

This demonstration firmware is based on STM32Cube. It helps you to discover
STM32 Cortex-M devices that can be plugged on a STM32 Discovery board.

In Normal mode (User button not pressed at startup) :
  - OFF Mode :
    - BUCK LED is off, only LEDs are blinking
    - Time between the blinking decreased
  - DIMMER Mode (press USER button) :
    - LED intensity is automatically increased then decreased
    - Red LED is ON when intensity increased
    - Blue LED is ON when intensity decreased
  - FLASH Mode (press USER button) :
    - LED is driven @10Hz frequency (20% time ON) at maximum intensity
    - Green and orange LEDs toggle alternatively
  - Manual Mode (long press on USER button) :
    - LED Intensity is adjusted manually by keeping pressed USER button
    - LED intensity increase or decrease alternatively
    - Always starts with low intensity value for first time


In Test mode (User button pressed at startup) :
  - VIN should be connected to 5V_O
  - Green and blue LEDs blink quickly to indicate test mode
  - BOOST mode test (when releasing User button)
    - VOUT voltage equals to 10V (can be controlled by multimeter)
    - Green LED is ON and blue LED is blinking
  - BUCK mode test (press USER button) :
    - VOUT voltage equals to 2V (can be controlled by multimeter)
    - Blue LED is ON and green LED is blinking
  - LEDs test (press USER button) :
    - All four LEDs are blinking
    
  - If orange LED is blinking :
    - VIN voltage is not available or not in the 4.5V to 5.5V range
    - VIN voltage's ADC acquisition is not correctly done (incorrect ADC values or timeout)
  - If red LED is blinking :
    - BUCK or BOOST mode regulation don't work (target voltage not reached before timeout)
    - VIN voltage released during the BUCK or BOOST mode

  (*) If VIN is not connected to +5V, after green and blue LEDs blink quickly to indicate test mode,
      when releasing User button, all four LEDS are turned ON.


@par Directory contents

  - Demonstration/stm32f3xx_hal_conf.h   HAL Configuration file
  - Demonstration/stm32f3xx_it.c         Interrupt handlers
  - Demonstration/stm32f3xx_it.h         Interrupt handlers header file
  - Demonstration/main.c                 Main program
  - Demonstration/system_stm32f3xx.c     STM32F3xx system source file


@par Hardware and Software environment

  - This Demonstration has been tested with STMicroelectronics STM32F3348-DISCO board 
    and can be easily tailored to any other supported device and development board.
  - This Demonstration runs on STM32F334R8 Devices.  

@par How to use it ? 

In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
