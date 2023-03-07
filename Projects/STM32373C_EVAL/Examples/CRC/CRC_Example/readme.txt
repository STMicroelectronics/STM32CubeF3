/**
  @page CRC_Example Cyclic Redundancy Check Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CRC/CRC_Example/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of Cyclic Redundancy Check Example.
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

How to configure the CRC using the HAL API. The CRC (cyclic
redundancy check) calculation unit computes the CRC code of a given buffer of
32-bit data words, using a fixed generator polynomial (0x4C11DB7).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 72 MHz.

The CRC peripheral configuration is ensured by the HAL_CRC_Init() function.
This later is calling the HAL_CRC_MspInit()function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function input parameter CrcHandle
to change CRC configuration.

The calculated CRC code is displayed on the debugger in uwCRCValue variable.
Once calculated, the CRC value is compared to the CRC expected value contained
in uwExpectedCRCValue variable..

STM32373-C Eval board LEDs are used to monitor the example status:
  - LED_GREEN is ON when the calculated CRC value is the expected CRC
  - LED_RED is ON when the calculated CRC value is not the expected CRC or when
    an error initialization error has occurred.


@par Directory contents 
  
  - CRC/CRC_Example/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - CRC/CRC_Example/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - CRC/CRC_Example/Inc/main.h                  Header for main.c module
  - CRC/CRC_Example/Src/stm32f3xx_it.c          Interrupt handlers
  - CRC/CRC_Example/Src/main.c                  Main program
  - CRC/CRC_Example/Src/stm32f3xx_hal_msp.c     HAL MSP module 
  - CRC/CRC_Example/Src/system_stm32f3xx.c      STM32F3xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32F373xx devices.
  - This example has been tested with STM32373C-EVAL board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 

 */
 