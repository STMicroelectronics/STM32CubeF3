/**
  @page SDADC_PressureMeasurement Pressure Measurement using SDADC
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    SDADC/SDADC_PressureMeasurement/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SDADC Pressure Measurement example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
