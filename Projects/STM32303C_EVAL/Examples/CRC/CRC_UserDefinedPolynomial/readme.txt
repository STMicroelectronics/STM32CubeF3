/**
  @page CRC_UserDefinedPolynomial Cyclic Redundancy Check Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CRC/CRC_UserDefinedPolynomial/readme.txt 
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
redundancy check) calculation unit computes the 8-bit CRC code for a given
buffer of 32-bit data words, based on a user-defined generating polynomial.
In this example, the polynomial is set manually to 0x9B, that is,
X^8 + X^7 + X^4 + X^3 + X + 1.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK).

The CRC peripheral configuration is ensured by the HAL_CRC_Init() function.
This later is calling the HAL_CRC_MspInit()function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change CRC configuration.

The calculated CRC code is saved in uwCRCValue variable.
Once calculated, the CRC value (uwCRCValue) is compared to the CRC expected value (uwExpectedCRCValue).

STM32 Eval board's LEDs can be used to monitor the example status:
  - LED1 is ON when the correct CRC value is calculated
  - LED3 is ON when there is an error in initialization or if an incorrect CRC value is calculated.


@par Directory contents 
  
  - CRC/CRC_UserDefinedPolynomial/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - CRC/CRC_UserDefinedPolynomial/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - CRC/CRC_UserDefinedPolynomial/Inc/main.h                  Header for main.c module
  - CRC/CRC_UserDefinedPolynomial/Src/stm32f3xx_it.c          Interrupt handlers
  - CRC/CRC_UserDefinedPolynomial/Src/main.c                  Main program
  - CRC/CRC_UserDefinedPolynomial/Src/stm32f3xx_hal_msp.c     HAL MSP module 
  - CRC/CRC_UserDefinedPolynomial/Src/system_stm32f3xx.c      STM32F3xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32F303xC devices.
  - This example has been tested with STM32303C-EVAL board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 

 */
 