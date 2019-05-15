/**
  @page HRTIM HRTIM_LLC_HalfBridge

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    HRTIM/HRTIM_LLC_HalfBridge/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of HRTIM_LLC_HalfBridge example.
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
  This example shows how to configure the HRTIM to control a half-bridge LLC 
  converter with synchronous rectification, using timer units A and B and 
  TA1/TA2/TB1/TB2 outputs.
  
  IMPORTANT NOTE: the Vin input pin of the discovery kit MUST NOT BE CONNECTED.
  This demo only aims at showing signal generation, the discovery kit does not 
  include the hardware for demoing a LLC converter. For a functional demo, 
  however, the discovery kit can be coupled and control a LLC converter with the
  resources given here-after.
  
  The TA1 and TA2 outputs (resp. PA8, PA9)are complementary PWM outputs for
  controlling the primary side half-bridge switches.
  The TB1 and TB2 outputs (resp. PA10, PA11) are independent PWM signals for
  controlling the synchronous rectification FETs on the secondary side.
  
  The ADC is configured to do 8 conversions per switching cycle, at the 
  following time:
  - 250ns before SR1 turn-on, 250ns after SR1 turn-on, 250ns before SR1 turn-off
  and 250ns after SR1 turn-off
  - same applies for SR2 also
    
  The FAULT1 input is enabled on PA12 (active low) to demonstrate PWM shut down 
  (low level sensitive), for all outputs.
  When the fault is triggered (PA12 input connected to GND) TA1, TA2, TB1, TB2
  signals are shut-down. The system can be re-armed by pressing the user button.
  
  The circuit is also protected from over-current on the primary side with the 
  comparator 2, on PA7 input, in delayed idle protection mode.
  
  The demo starts in RUN mode, and the LLC switching frequency is slowly 
  adjusted in the Master IRQ handler to simulate a control loop adjusting the
  operating point in real-time and show that all events are synchronized with 
  the primary side (SR FETs and ADC sampling points).
  
  LEDs are indicating the following:
  Green LED5: blinks during RUN operation
  Red LED3: blinks when FAULT is triggered
  Orange LED4: indicates the occurrence and duration of the PWM update ISR.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - HRTIM/HRTIM_LLC_HalfBridge/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - HRTIM/HRTIM_LLC_HalfBridge/Inc/stm32f3xx_it.h          PPP interrupt handlers header file
  - HRTIM/HRTIM_LLC_HalfBridge/Inc/main.h                  Header for main.c module  
  - HRTIM/HRTIM_LLC_HalfBridge/Src/stm32f3xx_it.c          PPP interrupt handlers
  - HRTIM/HRTIM_LLC_HalfBridge/Src/main.c                  Main program
  - HRTIM/HRTIM_LLC_HalfBridge/Src/stm32f3xx_hal_msp.c     HAL MSP file

@par Hardware and Software environment 

  - This example runs on STM32F334x4/STM32F334x6/STM32F334x8 devices.
    
  - This example has been tested with STM32F3348-DISCO
    board and can be easily tailored to any other supported device and development board.
  
  - STM32F3348-DISCO Set-up: PA12 must be briefly tied to ground to simulate a fault event

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
