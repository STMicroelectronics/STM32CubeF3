/**
  @page SDADC_Voltmeter Voltage measurement using SDADC
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    SDADC/SDADC_Voltmeter/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SDADC voltage measurement example.
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
Analog-to-Digital converter to perform precise voltage measurement when input 
voltage range is between 0V and VREF/gain.

 - The SDADC is configured in Single Ended Zero Reference mode: the input range
   is 0V to VREF/gain.
   In this example, the gain is set to 1 so all the range [0, VREF] is used.
 - The SDADC is configured in injected continuous mode. At each end of conversion
   an interrupt occurs and the SDADC data register is read in SDADC ISR.
 - In Single Ended Zero Reference mode, the input voltage is computed using the
   following formula: (InjectedConvData + 32768) * SDADCVREF / 65535
   With:
      - SDADCVREF is the SDADC reference voltage which is set to 3.3V on the 
      STM32373C-EVAL evaluation board.
      - InjectedConvData is the digital value read from SDADC data register
 - Connect a variable power supply 0-3.3V to SDADC1 channel 5P mapped on pin PB1.
   The potentiometer RV3 available on STM32373C-EVAL can be used. 
   The measured voltage on PB1 is displayed on the LCD mounted on the STM32373C-EVAL
   evaluation board.
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - SDADC/SDADC_Voltmeter/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - SDADC/SDADC_Voltmeter/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - SDADC/SDADC_Voltmeter/Inc/main.h                  Header for main.c module  
  - SDADC/SDADC_Voltmeter/Src/stm32f3xx_it.c          Interrupt handlers 
  - SDADC/SDADC_Voltmeter/Src/stm32f3xx_hal_msp.c.c   HAL MSP module
  - SDADC/SDADC_Voltmeter/Src/main.c                  Main program
  - SDADC/SDADC_Voltmeter/Src/system_stm32f3xx.c      STM32F3xx system source file
  
@par Hardware and Software environment

  - This example runs on STM32F373xx/378xx Devices.
  
  - This example has been tested with STMicroelectronics STM32373C-EVAL (STM32F373xC)
    evaluation board and can be easily tailored to any other supported device 
    and development board.

  - STM32373C-EVAL Set-up
    - Make sure that jumper JP16 and JP17 are fitted.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
