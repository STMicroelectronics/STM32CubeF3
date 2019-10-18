/**
  @page ADC_Sequencer ADC conversion example using related peripherals (GPIO, DMA), voltage input from DAC, user control by push button and LED

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_Sequencer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC conversion example
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

How to use the ADC peripheral with a sequencer to convert several channels. 
The channels converted are, in order, one external channel and two internal 
channels (VrefInt and temperature sensors).
Next, voltages and temperature are computed.


WAVEFORM_VOLTAGE_GENERATION_FOR_TEST compilation switch (located in main.h) is  
available to generate a waveform voltage for the test :
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" defined: waveform voltage for test is generated
   on DAC_CHANNEL_1 (output pin PA.04) by DAC peripheral,
   so user has just to connect a wire between DAC channel output and ADC input 
   (input pin PA.01) to run this example.
 - "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" not defined: no voltage is generated, user has
   to connect a voltage source to the selected ADC channel input to run this example


Other peripherals related to ADC are used:
Mandatory:
 - GPIO peripheral is used in analog mode to drive signal from device pin to
   ADC input.
Optionally:
 - DMA peripheral is used to transfer ADC converted data.

ADC settings:
  Sequencer is enabled and set to convert 3 ranks (3 channels) in discontinuous
  mode, one by one, each conversion being software-triggered.

ADC conversion results:
 - ADC conversions results are transferred automatically by DMA, into variable
   array "aADCxConvertedValues".
 - Each entry of this array contains one of the converted data of the ADC sequencer three ranks.
 - When DMA transfer half-buffer and buffer lengths are reached, callbacks
   HAL_ADC_ConvHalfCpltCallback() and HAL_ADC_ConvCpltCallback() are called.
 - When the ADC sequence is complete (the three ADC conversions are carried out), 
   the voltages and temperature are computed and stored in variables:
   uhADCChannelToDAC_mVolt, uhVrefInt_mVolt, wTemperature_DegreeCelsius.


Board settings:
 - ADC rank 1 is configured to convert ADC_CHANNEL_2 (pin PA.01).
  If compilation switch "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" is defined, the voltage input on ADC channel comes 
  from the DAC channel. This means that pins PA.01 and PA.04 must
  be connected by a wire. 
 - The voltage input on ADC channel is provided from DAC channel.
   DAC channel output is on pin PA.04.
   Note: ADC channel could have been chosen on pin PA.04 to be directly connected to DAC output,
   but this channel is on ADC2 only on this device. This example must use ADC1 because temperature sensor
   can be read from ADC1 only. Pin PA.01 is connected to a channel of ADC1.
   ==> Therefore, external connection is needed between pins PA.04 and PA.01 to run this example.
 - Voltage is increasing at each click on push button, from 0 to maximum range in 4 steps.
   Clicks on push button follow circular cycles: At clicks counter maximum value reached, counter is set back to 0.

  Channels on sequencer ranks 2 and 3 are ADC internal channels: no external connection is required.

Note: PA4 is connected to Arduino connector CN7, pin 17.
      PA1 is connected to Arduino connector CN9, pin 13.


STM32F303ZE-Nucleo Rev B board's LEDs are used to monitor the program execution status:
 - Normal operation: LED1 is turned-on/off with respect to ADC conversion results.
    - Turned-off if sequencer has not yet converted all ranks
    - Turned-on if sequencer has converted all ranks
 - Error: In case of error, LED2 is toggling at a frequency of 1Hz.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - ADC/ADC_Sequencer/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - ADC/ADC_Sequencer/Inc/stm32f3xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_Sequencer/Inc/main.h                  Header for main.c module  
  - ADC/ADC_Sequencer/Src/stm32f3xx_it.c          DMA interrupt handlers
  - ADC/ADC_Sequencer/Src/main.c                  Main program
  - ADC/ADC_Sequencer/Src/stm32f3xx_hal_msp.c     HAL MSP file
  - ADC/ADC_Sequencer/Src/system_stm32f3xx.c      STM32F3xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F303xE devices.

  - This example has been tested with STM32F303ZE-Nucleo Rev B board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
