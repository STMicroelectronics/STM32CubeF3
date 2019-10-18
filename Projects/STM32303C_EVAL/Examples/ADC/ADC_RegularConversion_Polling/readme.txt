/**
  @page ADC_RegularConversion_Polling conversion using Polling

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_RegularConversion_Polling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC RegularConversion Polling example.
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

How to use the ADC in Polling mode to convert data through the HAL API.

When the end of conversion occurs, the converted data of ADC1 DR register is 
affected to the uhADCxConvertedValue variable.

Conversion time (reference manual, Reset and Clock control part):
  The function SystemClock_Config() configures the clock divider as follows:
  1) The system clock is 72MHz.
  2) AHB Prescaler = 1 => AHB clock is 72 MHz.
  3) ADC Prescaler = 2 => ADC clock is 36 MHz.

  Sampling time is set to ADC_SAMPLETIME_4CYCLES_5 (4.5 cycles).
  ConvTime = Sampling time + 12.5 ADC clock cycles.
           = 17 clock cycles
           = 0.47 us


User can vary the ADC_CHANNEL_7 voltage using the Eval Board potentiometer (RV2) connected to PC.01.

STM32 Eval board's LEDs can be used to monitor the transfer status:
  - LED3 is ON when there are an error in initialization.

@par Directory contents 

  - ADC/ADC_RegularConversion_Polling/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - ADC/ADC_RegularConversion_Polling/Inc/stm32f3xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_RegularConversion_Polling/Inc/main.h                  Header for main.c module  
  - ADC/ADC_RegularConversion_Polling/Src/stm32f3xx_it.c          DMA interrupt handlers
  - ADC/ADC_RegularConversion_Polling/Src/main.c                  Main program
  - ADC/ADC_RegularConversion_Polling/Src/stm32f3xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_RegularConversion_Polling/Src/system_stm32f3xx.c      STM32F3xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F303xC devices.
  
  - This example has been tested with STM32303C-EVAL RevC board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
