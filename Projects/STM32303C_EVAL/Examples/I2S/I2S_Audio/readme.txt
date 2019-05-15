/**
  @page I2S_AUDIO  How to use the Audio features with I2S peripheral
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    I2S/I2S_Audio/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2S Audio Example.
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

This example provides basic implementation of audio features.
It allows to play an audio file through the I2S peripheral and DMA-based transfer
and using the external codec implemented on the STM32303C_EVAL board.

In this example the I2S input clock, provided by a dedicated PLL (PLLI2S),  is
configured to have an audio sampling frequency at 8 KHz with Master clock enabled.

This example uses an audio codec driver which consists of two independent layers:
 - Codec Hardware layer: which controls and communicates with the audio codec 
   (CS43L22) implemented on the evaluation board.
 - The high layer: which implements overall control and interfacing API allowing
   to perform basic audio operations (Init, Play, Pause, Resume, Stop, Volume 
   control and audio file pointer management)
  
In this example the audio file is stored in the internal flash memory (Mono, 
16-bit, 8 KHz). The analog output device is automatically detected when the Headphone 
is plugged/unplugged in/from the audio jack of the evaluation board. The example also 
manages information display and control interface through push buttons: 
  - Key          : Pause/Resume
  - Joystick up  : Volume UP
  - Joystick down: Volume DOWN

This example plays the audio file stored in internal flash memory and replays it
when it reaches end of file. But it can be tailored to used different media storage
devices (SDCard (through SDIO), external Memory (through FSMC) ...) or to play 
in streaming mode (ie. from USB port in device or host mode). In this case, Circular
DMA mode should be preferred (by enabling the define DMA_CIRCULAR in
I2Sx_MspInit() API of stm32303c_eval_audio.c file).  

List of Known Limitations and more detailed user notes are provided in file 
stm32303c_eval_audio.c   (under Drivers\BSP\STM32303C_EVAL)

The provided sample Audio file (stored in internal Flash memory) is extracted from:
  - Title: Partita E major, Gavotte en rondeau  
  - Artist/Composer: J.-S. Bach


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - I2S/I2S_Audio/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - I2S/I2S_Audio/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - I2S/I2S_Audio/Inc/main.h                  Header for main.c module  
  - I2S/I2S_Audio/Src/stm32f3xx_it.c          Interrupt handlers
  - I2S/I2S_Audio/Src/main.c                  Main program
  - I2S/I2S_Audio/Src/system_stm32f3xx.c      STM32F3xx system source file
  - I2S/I2S_Audio/Src/stm32f3xx_hal_msp.c     HAL MSP file
  - I2S/I2S_Audio/Src/audio_sample.c          Audio Sample file (in tab format)
      
@par Hardware and Software environment  
  - This example runs on STM32F303xx/SRM32F313xx devices.    
  - This example has been tested with STMicroelectronics STM32303C-EVAL RevC 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32303C-EVAL Set-up
    - Jumper JP5 and JP6 need to be set in I2C2 position.

@par How to use it ? 

   
In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 - Connect a headphone to the audio jack connector (CN11).
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 