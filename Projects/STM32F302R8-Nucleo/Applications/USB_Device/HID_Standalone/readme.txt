/**
  @page HID_Standalone USB Device Human Interface (HID) example

  @verbatim
  ******************************************************************************
  * @file    USB_Device/HID_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB HID example.
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

Use of the USB device application based on the Human Interface (HID).

This is a typical example on how to use the USB peripheral in Device mode with HID class V1.11
following the "Device Class Definition for Human Interface Devices (HID) Version 1.11
Jun 27, 2001". The example is built around the USB device library in order to emulate the Mouse directions
using User button.

This example supports the remote wakeup feature (the ability to bring the USB suspended bus back
to the active condition), and the User button is used as the remote wakeup source.

By default, in Windows OS, the USB mouse Power Management feature is turned off. This setting
is different from classic PS/2 computer functionality. To enable the Wake up from  standby
option, user has to manually turn on the Power Management feature for the USB mouse.

To enable the wake from standby option for the USB mouse, the following steps have to be followed:
 - Start "Device Manager",
 - Select "Mice and other pointing devices",
 - Select the "HID-compliant mouse" device (make sure that PID &VID are equal to 0x5710 & 0x0483 respectively)
 - Right click and select "Properties",
 - Select "Power Management" tab,
 - Finally click to select "Allow this device to wake the computer" check box.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 72 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock, which is generated from an integrated PLL.

It is possible to remappe the USB interrupts (USB_LP and USB_WKUP) on interrupt lines 75 and 76.
User can select USB line Interrupt through macro defined in main.h.
(USE_USB_INTERRUPT_DEFAULT and USE_USB_INTERRUPT_REMAPPED)

@note The application needs to ensure that the SysTick time base is set to 1 millisecond
      to have correct HAL configuration.

@note To reduce the example footprint, the toolchain dynamic allocation is replaced by a static allocation
      by returning the address of a pre-defined static buffer with the HID class structure size.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The built-in USB peripheral of the STM32F3x does not provide a specific interrupt for USB cable plug/unplug
      detection. The correct way to detect the USB cable plug/unplug is to detect the availability of the VBUS line
	  using a normal GPIO pin (external interrupt line).

@par Directory contents

  - USB_Device/HID_Standalone/Src/main.c                  Main program
  - USB_Device/HID_Standalone/Src/system_stm32f3xx.c      STM32f3xx system clock configuration file
  - USB_Device/HID_Standalone/Src/stm32f3xx_it.c          Interrupt handlers
  - USB_Device/HID_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/HID_Standalone/Src/usbd_desc.c             USB device HID descriptor
  - USB_Device/HID_Standalone/Inc/main.h                  Main program header file
  - USB_Device/HID_Standalone/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - USB_Device/HID_Standalone/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - USB_Device/HID_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/HID_Standalone/Inc/usbd_desc.h             USB device MSC descriptor header file


@par Hardware and Software environment

  - This example runs on STM32F301 and STM32F302xx devices.

  - This example has been tested with STMicroelectronics STM32 NUCLEO Rev C
    board with a USB shield daughter board, and can be easily tailored to any other supported
	device and development board.

  - STM32 NUCLEO Rev C Set-up
      - Since there is no USB 2.0 Full speed connector (Type B) on the nucleo board, user has to make
      his own USB shield daughter board with the a USB connector and plug it on top of the  CN11 and CN12
      connectors of the STM32F302R8-Nucleo. The USB connector has to be connected to the USB device associated GPIOs
      as follows:
       - DP (D+ of the USB connector) <======> PA12( pin12 in CN10 ) (Nucleo board)
       - DM (D- of the USB connector) <======> PA11( pin14 in CN10 ) (Nucleo board)
      - External USB 1.5k  resistor pull-ups is required on the USB D+ Line and VDD (3V3).
      - To improve EMC performance (noise immunity and signal integrity), it is recommended to connect a 100nF
      ceramic capacitor to the USB VDD pin.

@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
