/**
  @page COMP_AnalogWatchdog COMP Analog Watchdog example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    COMP/COMP_AnalogWatchdog/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the COMP Analog Watchdog Example
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

This example shows how to make an analog watchdog using the COMP peripherals in window mode.

  - The upper threshold is set to VREFINT = 1.22V
  - The lower threshold is set to VREFINT / 4 = 1.22V = 0.305V
  - The input voltage is configured to be connected to PA.01

STM32 Eval board's LEDs can be used to monitor the transfer status:
- LD3 and LD1 are turned On, if the input voltage is above the higher threshold.
- LD2 and LD4 are turned On, if the input voltage is under the lower threshold.
- If the input voltage is within the thresholds, the MCU remains in STOP mode.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - COMP/COMP_AnalogWatchdog/Inc/stm32f3xx_hal_conf.h   HAL configuration file
  - COMP/COMP_AnalogWatchdog/Inc/stm32f3xx_it.h         COMP interrupt handlers header file
  - COMP/COMP_AnalogWatchdog/Inc/main.h                 Header for main.c module
  - COMP/COMP_AnalogWatchdog/Src/stm32f3xx_it.c         COMP interrupt handlers
  - COMP/COMP_AnalogWatchdog/Src/main.c                 Main program
  - COMP/COMP_AnalogWatchdog/Src/system_stm32f3xx.c     STM32F3xx system source file
  - COMP/COMP_AnalogWatchdog/Src/stm32f3xx_hal_msp.c    HAL MSP file


@par Hardware and Software environment

  - This example runs on STM32F3xxx devices.
    
  - This example has been tested with STM32303C-EVAL RevB evaluation board and can be
    easily tailored to any other supported device and development board.    

  - STM32303C-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PE.08, PE.09 ,PE.10
      and PE.11 pins
      @note This example run in stand alone mode.
    - Use a wire to connect Potentiometer RV2 (PC.01 pin) to COMP2 non inverting input (PA.01)


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
