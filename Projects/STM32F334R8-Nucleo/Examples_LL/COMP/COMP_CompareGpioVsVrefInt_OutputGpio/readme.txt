/**
  @page COMP_CompareGpioVsVrefInt_OutputGpio COMP example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/COMP/COMP_CompareGpioVsVrefInt_OutputGpio/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the COMP_CompareGpioVsVrefInt_OutputGpio example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to use a comparator peripheral to compare a voltage level applied on a GPIO
pin to the internal voltage reference (VREFINT). The comparator output is connected
to a GPIO. This example is based on the STM32F3xx COMP LL API.
The peripheral initialization uses LL unitary service functions for optimization
purposes (performance and size).

Example configuration:
Comparator instance COMP4 is configured to compare:
 - input plus set to a GPIO pin (cf pin below) in analog mode.
 - input minus set to internal voltage reference VrefInt (voltage level 1.2V,
   refer to device datasheet for min/typ/max values)
A GPIO pin a configured in alternate function to be connected directly
to comparator output (cf pin below).
Comparator is set in power mode ultra low power.

Example execution:
From the main program execution, comparator is enabled.
Each time the voltage level applied on GPIO pin (comparator input plus) 
is crossing VrefInt voltage threshold (comparator input minus),
the comparator output is setting the logical level on GPIO.
LED2 is turned on or off depending on comparator output state (monitoring
of state of GPIO connected to comparator output):
turned on if comparator voltage level on input plus is higher than input minus,
turned-off otherwise.

Connection needed:
A voltage must be supplied to the analog input pin (cf pin below),
between Vdda=3.3V and 0V. Otherwise this pin can be let floating (but in this
case COMP comparison level will be undetermined).

To check comparator threshold, you can:
 - Either use a wire to connect the GPIO pin of comparator input (cf pin below)
   to other connectors on board:
    - below comparator threshold: to ground
    - above comparator threshold: to voltage of power supply Vdda
      (on board Nucleo: Vdda=3.3V, available on CN6 pin 4 or CN7 pin 5)
 - Either use an external power supply

Other peripheral used:
  1 GPIO for LED2
  1 GPIO for analog input of comparator: PB.00 (Arduino connector CN8 pin A3, Morpho connector CN7 pin 34)
  1 GPIO for digital output of comparator: PB.01 (Morpho connector CN10 pin 24)


@par Directory contents 

  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio/Inc/main.h                  Header for main.c module
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio/Inc/stm32_assert.h          Template file to include assert_failed function
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio/Src/stm32f3xx_it.c          Interrupt handlers
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio/Src/main.c                  Main program
  - COMP/COMP_CompareGpioVsVrefInt_OutputGpio/Src/system_stm32f3xx.c      STM32F3xx system source file


@par Hardware and Software environment

  - This example runs on STM32F334R8 devices.
    
  - This example has been tested with STM32F334R8-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
