/**
  @page SPI_FullDuplex_AdvComIT SPI Full Duplex Advanced Interrupt example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    SPI/SPI_FullDuplex_AdvComIt/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SPI Full Duplex Advanced Interrupt example.
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
 
Configuration of the HAL SPI API to transmit/receive a data buffer using Interrupt mode 
in an advanced communication mode:
The master board always sends the command to the slave before performing any transmission; 
the slave board sends back an acknowledgement before proceeding.

To use this example, you need to load it on two STM32 boards (let's call them 
BoardA and BoardB) then connect these two boards through SPI lines and GND.
In the firmware example two project are provided to use the SPI peripheral as STM32
Master device or as STM32 Slave .

   _________________________                       __________________________
  |    _________CN7_________|                      |________CN7__________    |
  |   |SPI3                 |                      |                SPI3|    |
  |   |                     |                      |                    |    |
  |   |(pin 1)     CLK(PC10)|______________________|(PC10)CLK    (pin 1)|    |
  |   |                     |                      |                    |    |
  |   |(pin 2)    MISO(PC11)|______________________|(PC11)MISO   (pin 2)|    |
  |   |                     |                      |                    |    |
  |   |(pin 3)    MOSI(PC12)|______________________|(PC12)MOSI   (pin 3)|    |
  |   |                     |                      |                    |    |
  |   |_____________________|                      |____________________|    |
  |      __                 |                      |                         |
  |     |__|                |                      |                         |
  |     USER                |                      |                         |
  |                      GND|______________________|GND                      |
  |                         |                      |                         |
  |_STM32F3 Master _________|                      |_STM32F3 Slave __________|

@note The connection between the pins should use a short wires and a common Ground.


At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 64 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK & 
GPIO). You may update this function to change SPI configuration.

The SPI peripheral is configured in full duplex mode with baudrate = FCPU/16. 

If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.
                       
In Master board, when the user button is pressed (used for only synchronization at startup)
these steps are executed: 
 - Synchro with the Slave
 - Requests read operation by sending specific command to the slave that contain
   the master read command and the number of data to be read
 - Synchro with the Slave
 - Receive the ACK from the Slave
 - Synchro with the Slave  
 - Receive the requested data "aRxBuffer" from the slave
 - Synchro with the Slave
 - Send the ACK to the Slave
 - Checks the correctness of data and in case of mismatch between sent and received data,
   LED2 is On and communication is stopped (using infinite loop). Otherwise LED2 toggles,
   indicating that the data is received correctly.
 - Synchro with the Slave
 - Requests write operation by sending specific command to the slave that contain
   the master write command and the number of data to be written
 - Synchro with the Slave 
 - Receive the ACK from the Slave
 - Synchro with the Slave 
 - Send the requested data "aTxMasterBuffer" to the slave
 - Synchro with the Slave 
 - Receive the ACK from the Slave
These previously steps are repeated in an infinitely loop and a delay 100ms is inserted at
the end of data transfer to permit the user to see LED2 toggling.

In Slave board, these steps are executed: 
 - Synchro with the Master
 - Receive requests from the master
 - Synchro with the Master
 - Send the ACK to the Master
 - Synchro with the Master
 - If the "ADDRCMD_MASTER_READ" command is received: 
   - Send the requested data "aTxSlaveBuffer" to the Master
   - Synchro with the Master
   - Receive the ACK from the Master
 - If the "ADDRCMD_MASTER_WRITE" command is received: 
   - Receive the requested data "aRxBuffer" from the Master
   - Synchro with the Master
   - Send the ACK to the Master
   - checks the correctness of data is performed and in case of mismatch between sent and
     received data, LED2 is On and communication is stopped (using infinite loop).
     Otherwise LED2 toggles indicating that the data is received correctly.

These previously steps are repeated in an infinite loop.

STM32 board's LEDs can be used to monitor the transfer status:
 - LED2 toggles quickly on master board waiting User push-button to be pressed.
 - LED2 toggles on Master board at end of each loop.
 - LED2 toggles on both boards when the read comparison is OK.
 - LED2 turns ON when there is an error in transmission/reception process. 

@note SPIx instance used and associated resources can be updated in "main.h"
      file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - SPI/SPI_FullDuplex_AdvComIT/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - SPI/SPI_FullDuplex_AdvComIT/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - SPI/SPI_FullDuplex_AdvComIT/Inc/main.h                  Header for main.c module  
  - SPI/SPI_FullDuplex_AdvComIT/Src/stm32f3xx_it.c          Interrupt handlers
  - SPI/SPI_FullDuplex_AdvComIT/Src/main.c                  Main program
  - SPI/SPI_FullDuplex_AdvComIT/Src/system_stm32f3xx.c      stm32f3xx system source file
  - SPI/SPI_FullDuplex_AdvComIT/Src/stm32f3xx_hal_msp.c     HAL MSP file

@par Hardware and Software environment

  - This example runs on STM32F302x8 devices.

  - This example has been tested with STM32F302R8-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board.

  - STM32F302R8-Nucleo Rev C Set-up
    - Connect Master SPI3 SCK pin  (PC10) to Slave SPI3 SCK pin  (PC10) (pin 1 on CN7 connector)
    - Connect Master SPI3 MISO pin (PC11) to Slave SPI3 MOSI pin (PC11) (pin 2 on CN7 connector)
    - Connect Master SPI3 MOSI pin (PC12) to Slave SPI3 MOSI pin (PC12) (pin 3 on CN7 connector)
    - Connect Master board GND  to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 