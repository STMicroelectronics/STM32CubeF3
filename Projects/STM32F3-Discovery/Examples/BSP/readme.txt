/**
  @page BSP  Example on how to use the F3-DK BSP
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

This example provides a description of how to use the different BSP drivers. 

- Connect the STM32F3-DISCOVERY board to a PC with a 'USB type A to Mini-B' cable
through USB connector CN2 or CN1 to power the board. Then red led LD1 (PWR) and
LD2 (COM) light up.
- All 8 leds between B1 and B2 are blinking as a chenilard.
- Press User Button B1 (Button left corner of the board) then accelerometer MEMS sensor is
enabled, move the board horizontaly and vertically and observe the four LEDs blinking 
according to the acceleration.
- Press User Button B1 (Button left corner of the board) then gyroscope MEMS sensor is
enabled, move the board and observe the four LEDs blinking according to the motion
direction.
- Press User Button B1 (Button left corner of the board) then Compass MEMS sensor is
enabled, move the board horizontaly and observe the north direction. If you take the
board and lean it then all leds are blinking.

@par Directory contents 

  - Examples/BSP/stm32f3xx_hal_conf.h HAL Configuration file
  - Examples/BSP/stm32f3xx_it.c       Interrupt handlers
  - Examples/BSP/stm32f3xx_it.h       Interrupt handlers header file
  - Examples/BSP/main.c               Main program
  - Examples/BSP/main.h               Main program header file
  - Examples/BSP/mems.c               Mems feature
  - Examples/BSP/mems.h               Header for mems.c
  - Examples/BSP/system_stm32f3xx.c   STM32F3xx system source file

@note The "system_stm32f3xx.c" file contains the system clock configuration for
      STM32F3xx devices, and is customized for use with STM32F3-Discovery Kit. 
      The STM32F3xx is configured to run at 72 MHz.          

@par Hardware and Software environment

  - This Demo runs on STM32F303xC devices.
  - This example has been tested with STMicroelectronics STM32F3-Discovery (MB1035) 
    and can be easily tailored to any other supported device and development board.

  - STM32F3-Discovery Set-up
    - None.
      
@par How to use it ? 

In order to make the program work, you must do the following :
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
