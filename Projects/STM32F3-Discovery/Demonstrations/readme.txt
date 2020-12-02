/**
  @page Description of the STM32F3DISCOVERY Demo

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of the STM32F3DISCOVERY Demo.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Demo Description

This demonstration firmware is based on STM32Cube. It helps you discover
STM32 Cortex-M devices that are plugged on your STM32 Discovery board.

- Connect the STM32F3DISCOVERY board to a PC with a 'USB type A to Mini-B' cable
  through USB connector CN2 or CN1 to power the board. Then red led LD1 (PWR) and
  LD2 (COM) light up.
- All 8 leds between B1 and B2 are blinking clockwise.
- At reset when the Button B1 is Pressed more than 1 second the USB Test is executed.
  Othersiwse the demo is launched.
- The USB test consists in moving the cursor in square path.
  The LED corresponding to the current cursor direction is ON.

- The following sequence describes the demo application:
  - Press the User Button B1 (blue button at the left corner of the board),
    then gyroscope MEMS sensor is enabled.
    Tilt the board relative to the horizontal plane and observe the four LEDs
    blinking according to the motion direction.
    As soon as the move is stopped, all LEDs are OFF.
  - Press the User Button B1 again, and all 8 leds between B1 and B2 restart
    blinking clockwise.
  - Press the User Button B1, then accelerometer MEMS sensor is enabled.
    Tilt the board relative to the horizontal plane and observe the four LEDs
    blinking according to the acceleration.
    As long as the board is tilted, the LED on the side down is ON.
  - Press the User Button B1 again, and all 8 leds between B1 and B2 restart
    blinking clockwise.
  - Press the User Button B1 to enable the USB USER and convert the STM32F3-Discovery
    board to a standard mouse (LD3, LD6, LED7 and LED10 are ON).
    Connect a second USB type A/mini-B cable through the USB User connector and
    the PC, then move the board (horizontally and vertically) and see the mouse
    cursor moving according to the motion direction.
  - Press the User Button B1 again to restart the demo application.

@par Known limitation

  Please note that the demo execution may freeze. In case this happens, simply
  restart it by pressing the Reset Button B2.
  Action is being taken to fix this known limitation in a future release.

@par Directory contents

  - Demonstration/stm32f3xx_hal_conf.h HAL Configuration file
  - Demonstration/stm32f3xx_it.c           Interrupt handlers
  - Demonstration/stm32f3xx_it.h           Header for stm32f3xx_it.c
  - Demonstration/usbd_conf.c              General low level driver configuration
  - Demonstration/usbd_conf.h              USB device driver Configuration file
  - Demonstration/usbd_desc.c              USB device HID descriptor
  - Demonstration/usbd_desc.h              USB device HID descriptor header file
  - Demonstration/main.c                   Main program
  - Demonstration/main.h                   Header for main.c
  - Demonstration/selftest.c               MEMS feature program
  - Demonstration/selftest.c .h            Header for selftest.c
  - Demonstration/system_stm32f3xx.c       STM32F3xx system source file
  - Demonstration/stm32f3xx_hal_conf.h     HAL configuration file

@note The "system_stm32f3xx.c" file contains the system clock configuration after reset.

@par Hardware and Software environment

  - This Demo runs on STM32F303xC devices.

  - This example has been tested with STMicroelectronics STM32F3DISCOVERY (MB1035)
    and can be easily tailored to any other supported device and development board.

  - STM32F3DISCOVERY board Set-up
    - None.

@par How to use

In order to make the program work, you must do the following :
- Open your preferred toolchain
- Rebuild all files and load your image into target memory
- Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
