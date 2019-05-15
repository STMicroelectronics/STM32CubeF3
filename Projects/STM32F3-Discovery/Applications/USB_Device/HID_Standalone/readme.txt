/**
  @page HID_Standalone USB Device Human Interface (HID) example
  
  @verbatim
  ******************************************************************************
  * @file    USB_Device/HID_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB HID example.
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

This application shows how to use the USB device application based on the Human Interface (HID).

This is a typical example on how to use the USB peripheral in Device mode with HID class V1.11 
following the "Device Class Definition for Human Interface Devices (HID) Version 1.11 
Jun 27, 2001". The example is built around the USB device library and emulate the joystick by moving 
the host mouse pointer horizontally.

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

The function GetPointerData() is responsible to move the mouse cursor automatically,
which can be easily customized (square, circle...).
In this application the cursor is moving forward and backward in an infinite loop.

Four LEDs are toggling (LED3,LED4,LED9 and LED10) each 100ms to show user input state. 

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
	  
@note The build-in USB peripheral of the STM32F3x does not provide a specific interrupt for USB cable plug/unplug 
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

  - This example runs on STM32F303xx devices.
    
  - This example has been tested with STMicroelectronics STM32F3-Discovery
    board and can be easily tailored to any other supported device 
    and development board.

  - STM32F3-Discovery Set-up
    - Connect the STM32F3-Discovery board to the PC through 'USB Mini B-Receptacle USB User connector
              
@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
  