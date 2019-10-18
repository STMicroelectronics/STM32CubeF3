/**
  @page DFU_Standalone USB Device Firmware Upgrade (DFU) example
  
  @verbatim
  ******************************************************************************
  * @file    USB_Device/DFU_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB DFU example.
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

@par Example Description 

Compliant implementation of the Device Firmware Upgrade (DFU) 
capability to program the embedded Flash memory through the USB peripheral.

This is a typical example on how to use the STM32F3xx USB Device peripheral to implement the DFU
capability for programming the embedded flash memory of the STM32F3xx devices.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 72 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock, which is generated from an integrated PLL. 

The DFU transactions are based on Endpoint 0 (control endpoint) transfer. All requests and status 
control are sent/received through this endpoint.

The Internal flash memory memory is split as follows:
 - DFU area located in [0x08000000 : USBD_DFU_APP_DEFAULT_ADD-1]: Only read access
 - Application area located in [USBD_DFU_APP_DEFAULT_ADD : Device's end address]: Read, Write, and Erase
   access

In this application, two operating modes are available:
 1. DFU operating mode: 
    This mode is entered after an MCU reset in case:
     - The DFU mode is forced by the user: the user presses the key button.
     - No valid code found in the application area: a code is considered valid if the MSB of the initial
       Main Stack Pointer (MSP) value located in the first address of the application area is equal to 
       0x2000
      
 2. Run-time application mode: 
    This is the normal run-time activities. A binary which toggles LEDs on the evaluation board is 
    provided in Binary directory.

@note After each device reset, hold down the Key button on the evaluation board to enter the DFU mode.     

Traditionally, firmware is stored in Hex, S19 or Binary files, but these formats do not contain the 
necessary information to perform the upgrade operation, they contain only the actual data of the program
to be downloaded. However, the DFU operation requires more information, such as the product identifier, 
vendor identifier, Firmware version and the Alternate setting number (Target ID) of the target to be 
used, this information makes the upgrade targeted and more secure. To add this information, DFU file 
format is used. For more details refer to the "DfuSe File Format Specification" document (UM0391).

To generate a DFU image, download "DFUse Demonstration" tool and use DFU File Manager to convert a 
binary image into a DFU image. This tool is for download from www.st.com
To download a *.dfu image, use "DfuSe Demo" available within "DFUse Demonstration" install directory. 

Please refer to UM0412, DFuSe USB device firmware upgrade STMicroelectronics extension for more details
on the driver installation and PC host user interface.
    
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
	  
@par USB Library Configuration

It is possible to fine tune needed USB Device features by modifying defines values in USBD configuration
file “usbd_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:      
 - USBD_DFU_APP_DEFAULT_ADD, specifing the address from where user's application will be downloaded.

Device's end address is the end address of the flash memory and it is dependent on the device in use.


@par Directory contents 

  - USB_Device/DFU_Standalone/Src/main.c                  Main program
  - USB_Device/DFU_Standalone/Src/system_stm32f3xx.c      STM32F3xx system clock configuration file
  - USB_Device/DFU_Standalone/Src/stm32f3xx_it.c          Interrupt handlers
  - USB_Device/DFU_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/DFU_Standalone/Src/usbd_desc.c             USB device descriptor
  - USB_Device/DFU_Standalone/Src/usbd_dfu_flash.c        Internal flash memory management
  - USB_Device/DFU_Standalone/Inc/main.h                  Main program header file
  - USB_Device/DFU_Standalone/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - USB_Device/DFU_Standalone/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - USB_Device/DFU_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/DFU_Standalone/Inc/usbd_desc.h             USB device descriptor header file
  - USB_Device/DFU_Standalone/Inc/usbd_dfu_flash.h        Internal flash memory management header file


@par Hardware and Software environment

  - This example runs on STM32F303xx devices.
    
  - This example has been tested with STMicroelectronics STM32303C-EVAL
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32303C-EVAL Set-up:
    - Install the DFU driver available in "DfuSe Demonstrator" installation directory
    - For Windows 8.1 and later : Update STM32 DFU device driver manually from Windows Device Manager.
      The install of required device driver is available under "Program Files\STMicroelectronics\Software\DfuSe v3.0.5\Bin\Driver\Win8.1" directory.
    - Connect the STM32303C-EVAL board to the PC through 'USB Type A-Male 
      to A-Male'


@par How to use it ?

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
