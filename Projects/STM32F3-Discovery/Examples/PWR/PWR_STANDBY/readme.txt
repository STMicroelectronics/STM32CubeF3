/**
  @page PWR_STANDBY PWR standby example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    PWR/PWR_STANDBY/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the PWR STANDBY example.
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
 
This example shows how to enter the system in STANDBY mode and wake-up from this
mode using external RESET or WKUP pin.

In the associated software, the system clock is set to 72 MHz, an EXTI line
is configured to generate an interrupt on falling edge and the SysTick is programmed
to generate an interrupt each 1 ms. In the SysTick interrupt handler, the LED3 is
toggled, this is used to indicate whether the MCU is in STANDBY or RUN mode.

When a falling edge is detected on the EXTI line an interrupt is generated. In the 
EXTI handler routine the RTC is configured to generate an Alarm event in 5 second
then the system enters STANDBY mode causing the LED3 to stop toggling. 
A rising edge on WKUP pin or an external RESET will wake-up the system from
STANDBY. If within 5 second neither rising edge on WKUP pin nor external RESET
are generated, the RTC Alarm A will wake-up the system. 

After wake-up from STANDBY mode, program execution restarts in the same way as after
a RESET, the RTC configuration (clock source, prescaler,...) is kept and LED3 restarts
toggling. As result there is no need to configure the RTC.

Two leds LED3 and LED4 are used to monitor the system state as following:
 - LED4 ON: configuration failed (system will go to an infinite loop)
 - LED3 toggling: system in RUN mode
 - LED3 off : system in STANDBY mode

These Steps are repeated in an infinite loop.

@note To measure the current consumption in STANDBY mode, please refer to 
      @subpage PWR_CurrentConsumption example.

@note This example can not be used in DEBUG mode, this is due to the fact 
      that the Cortex-M4 core is no longer clocked during low power mode 
      so debugging features are disbaled

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select the RTC clock source; in this 
       case the Backup domain will be reset in order to modify the RTC Clock source, as consequence RTC  
       registers (including the backup registers) and RCC_BDCR register are set to their reset values.

@par Directory contents 

  - PWR/PWR_STANDBY/Inc/stm32f3xx_conf.h         Library Configuration file
  - PWR/PWR_STANDBY/Inc/stm32f3xx_it.h           Header for stm32f3xx_it.c
  - PWR/PWR_STANDBY/Inc/main.h                   Header file for main.c
  - PWR/PWR_STANDBY/Src/system_stm32f3xx.c       STM32F3xx system clock configuration file
  - PWR/PWR_STANDBY/Src/stm32f3xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY/Src/main.c                   Main program
  - PWR/PWR_STANDBY/Src/stm32f3xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32F303xC devices
    
      
  - This example has been tested with STMicroelectronics STM32F3-Discovery RevB
    evaluation board and can be easily tailored to any other supported device 
    and development board.

  - STM32F3-Discovery RevB Set-up
    - Use LED3 and LED4 connected respectively to PE.09 and PE.08 pins
    - Use the User push-button connected to pin PA.00 (EXTI_Line0)
    - Wake-Up pin is PA.00

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
