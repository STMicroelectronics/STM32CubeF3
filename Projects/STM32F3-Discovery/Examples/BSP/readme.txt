/**
  @page BSP  Example on how to use the F3-DK BSP
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
  ******************************************************************************
  *@attention
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

@par Example Description 

This example provides a description of how to use the different BSP drivers. 

- Connect the STM32F3-DISCOVERY board to a PC with a 'USB type A to Mini-B' cable
through USB connector CN2 or CN1 to power the board. Then red led LD1 (PWR) and
LD2 (COM) light up.
- All 8 leds between B1 and B2 are blinking as a chenilard.
- Press User Button B1 (Button left corner of the board) then accelerometer MEMS sensor is
enabled, move the board horizontaly and vertically and observe the four LEDs blinking 
according to the acceleration.
- Press User Button B1 (Button left corner of the board) then gyroscope MEMS sensor is
enabled, move the board and observe the four LEDs blinking according to the motion
direction.
- Press User Button B1 (Button left corner of the board) then Compass MEMS sensor is
enabled, move the board horizontaly and observe the north direction. If you take the
board and lean it then all leds are blinking.

@par Directory contents 

  - Examples/BSP/stm32f3xx_hal_conf.h HAL Configuration file
  - Examples/BSP/stm32f3xx_it.c       Interrupt handlers
  - Examples/BSP/stm32f3xx_it.h       Interrupt handlers header file
  - Examples/BSP/main.c               Main program
  - Examples/BSP/main.h               Main program header file
  - Examples/BSP/mems.c               Mems feature
  - Examples/BSP/mems.h               Header for mems.c
  - Examples/BSP/system_stm32f3xx.c   STM32F3xx system source file

@note The "system_stm32f3xx.c" file contains the system clock configuration for
      STM32F3xx devices, and is customized for use with STM32F3-Discovery Kit. 
      The STM32F3xx is configured to run at 72 MHz.          

@par Hardware and Software environment

  - This Demo runs on STM32F303xC devices.
  - This example has been tested with STMicroelectronics STM32F3-Discovery (MB1035) 
    and can be easily tailored to any other supported device and development board.

  - STM32F3-Discovery Set-up
    - None.
      
@par How to use it ? 

In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
