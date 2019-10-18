/**
  @page SDADC_TempMeasurement temperature measurement using SDADC
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    SDADC/SDADC_TempMeasurement/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SDADC Temperature measurement using PT100 sensor.
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
Analog-to-Digital converter to perform accurate temperature measurement.

This is done using the PT100 sensor connected to PE7 in the STM32373C-EVAL evaluation board.

 - The SDADC is configured in single ended offset. The input range is from 0V to VREF/(2*gain).
   In this example, the SDADC internal gain is set to 8 so the range is from 0V to VREF/16.
   The external reference VREF (set to 3.3V on STM32373C-EVAL) is used as reference for SDADC
   using JP17 so the measurement ranges between 0V and VREF/16 = 0,20625V.
   The conversion is performed in continuous mode with interrupt enabled on end
   of regular conversion.
   A PT-100 sensor is connected to SDADC1 channel 3P (PE7) through a reference
   resistor 1K8 labeled R33 on STM32373C-EVAL evaluation board.
   The temperature is computed using the formula below:
   Rpt100 = 100 + 0.385 * T ==> T = (Rpt100 - 100) / 0.385
   Vpt100 = Rpt100 * Ipt100 = Rpt100 * VDD_ANA / 2 * Rref ==> Rpt100 = Vpt100 * 1800 * 2 / VDD_ANA 
   ==> T = ((Vpt100 * 1800 * 2 / VDD_ANA) - 100) / 0.385
         =  ((SDADC_Sample * 1800 * 2 / 65535) - 100) / 0.385  
   with:
      - Rpt100 is the resistance of the PT100 sensor
      - Vpt100 is the voltage measured on PT100 sensor
      - Ipt100 is the current crossing the PT100 sensor
      - VDD_ANA is the analog voltage
      - Rref is the reference resistor 1K8 labeled R33 on STM32373C-EVAL
      - SDADC_Sample is the digital value read from SDADC data register
        
 - The temperature measurement is performed in two steps:
   1. Temperature sensor calibration: this phase is performed with JP18
       fitted in 2-3(REF)position. A 100 Ohm resistor is connected to PE7 which is
       connected to VREF through the reference resistor.
       The SDADC converter measures the analog voltage applied on PE7 and then
       computes the correction coefficient.
       The PT100 is not connected in this phase.
   2. The temperature measurement: this phase is performed with JP18
       fitted in 1-2(PT100) position. The PT100 sensor is connected to PE7 which is
       connected to VREF through the reference resistor.
       The SDADC converter measures the analog voltage applied on PE7 and then
       computes the temperature which is given by the following formula:
      TemperaturePT100 = (((((CoeffCorrection * (AvrgRegularConvData/SDADC_GAIN) * REFERENCE_RESISTOR * 2) / SDADCRESOL) - RESISTANCE_ZERODEGRE) / RESISTANCE_COEFFICIENT);
      with:
       - AvrgRegularConvData is the average value of 256 samples
       - SDADC_GAIN is the internal SDADC gain. In this example it is set to 8
       - CoeffCorrection is the correction coefficient computed in phase 1.
       - REFERENCE_RESISTOR is the reference resistor 1K8 labeled R33 on STM32373C-EVAL
       - SDADCRESOL is the sigma delta converter resolution: 2e16-1
       - RESISTANCE_ZERODEGRE is the resistance of PT100 at 0 degree C
       - RESISTANCE_COEFFICIENT is the coefficient of PT100 sensor

 - The measured temperature is displayed on the LCD mounted on STM32373C-EVAL. 

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - SDADC/SDADC_TempMeasurement/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - SDADC/SDADC_TempMeasurement/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - SDADC/SDADC_TempMeasurement/Inc/main.h                  Header for main.c module  
  - SDADC/SDADC_TempMeasurement/Src/stm32f3xx_it.c          Interrupt handlers 
  - SDADC/SDADC_TempMeasurement/Src/stm32f3xx_hal_msp.c.c   HAL MSP module
  - SDADC/SDADC_TempMeasurement/Src/main.c                  Main program
  - SDADC/SDADC_TempMeasurement/Src/system_stm32f3xx.c      STM32F3xx system source file
  
@par Hardware and Software environment

  - This example runs on STM32F373xC devices.
  
  - This example has been tested with STMicroelectronics STM32373C-EVAL RevB
    boards and can be easily tailored to any other supported device 
    and development board.

  - STM32373xC-EVAL Set-up
    - Make sure that jumper JP16 and JP17 are fitted.
    - Make sure that the jumper JP12 and JP13 are fitted in 3V3 position.
    - Follow example instructions on jumper JP18 position 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
