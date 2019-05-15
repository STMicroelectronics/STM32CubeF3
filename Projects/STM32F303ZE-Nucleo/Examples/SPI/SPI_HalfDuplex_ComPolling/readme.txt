/**
  @page SPI_HalfDuplex_ComPolling SPI Half Duplex Polling example
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    SPI/SPI_HalfDuplex_ComPolling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Half Duplex Polling example.
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

This example shows how to ensure SPI data buffer half-duplex transmission/reception 
in Polling mode between two boards.

   _________________________                       __________________________
  |    _________CN7_________|                      |________CN7__________    |
  |   |SPI1                 |                      |                SPI1|    |
  |   |                     |                      |                    |    |
  |   |(pin 15)     CLK(PB3)|______________________|(PB3)CLK    (pin 15)|    |
  |   |                     |                      |                    |    |
  |   |(pin 13)    MOSI(PB5)|______________________|(PB4)MISO   (pin 19)|    |
  |   |_____________________|                      |____________________|    |
  |   |                     |                      |                    |    |
  |    ________ CN8  _______|                      |_______ CN11 _______     |
  |   |(pin 9)         +5V  |______________________| E5V        (pin 6) |    |
  |   |                     |                      |                    |    |
  |   |_____________________|                      |____________________|    |
  |      __                 |                      |                         |
  |     |__|                |                      |     JP3 (E5V) position  |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32   Master _________|                      |_STM32   Slave __________|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 64 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK & 
GPIO). You may update this function to change SPI configuration.

The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.h" file.
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button, SPI Master starts the
communication by sending aTxBuffer through HAL_SPI_Transmit(), at the same time SPI Slave
receives aRxBuffer through HAL_SPI_Receive(). 

Then after 2 seconds delay, SPI Master starts reception of aRxBuffer, meanwhile SPI Slave
will transmit the received buffer.

Finally aRxBuffer and aTxBuffer are compared through Buffercmp() in order to 
check buffers correctness.

STM32 board's LEDs can be used to monitor the master transfer status:
 - LED1 toggles quickly on master board waiting User push-button to be pressed.
 - LED1 turns ON when the transmission process is complete.
 - LED2 turns ON when aTxBuffer and aRxBuffer comparison is correct.
 - LED3 turns ON when there is an error in transmission/reception process. 
 - LED3 toggles slowly when there is a timeout error in transmission/reception process.  

@note SPIx instance used and associated resources can be updated in "main.h"
      file depending hardware configuration used.

@note You need to perform a reset on Slave board, then perform it on Master board
      to have the correct behaviour of this example.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - SPI/SPI_HalfDuplex_ComPolling/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - SPI/SPI_HalfDuplex_ComPolling/Inc/stm32f3xx_it.h          SPI interrupt handlers header file
  - SPI/SPI_HalfDuplex_ComPolling/Inc/main.h                  Header for main.c module  
  - SPI/SPI_HalfDuplex_ComPolling/Src/stm32f3xx_it.c          SPI interrupt handlers
  - SPI/SPI_HalfDuplex_ComPolling/Src/main.c                  Main program
  - SPI/SPI_HalfDuplex_ComPolling/Src/system_stm32f3xx.c      STM32F3xx system source file
  - SPI/SPI_HalfDuplex_ComPolling/Src/stm32f3xx_hal_msp.c     HAL MSP file
  

@par Hardware and Software environment

  - This example runs on STM32F303xE devices.
    
  - This example has been tested with STM32F303ZE-Nucleo Rev B board and can be
    easily tailored to any other supported device and development board.

  -STM32F303ZE-Nucleo Rev B Set-up
    - Connect Master board CLK to Slave Board CLK (pin 15 on CN7 connector)
    - Connect Master board MOSI (pin 13 on CN7 connector) to Slave Board MISO (pin 19 on CN7 connector)
    - Connect Master board GND to Slave Board GND
    - Slave board powered by master board
          - JP3 fitted on E5V on Slave board
          - Connect Master board +5V (pin 9 on CN8 connector) to Slave board E5V (pin 6 on CN11 connector)

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 