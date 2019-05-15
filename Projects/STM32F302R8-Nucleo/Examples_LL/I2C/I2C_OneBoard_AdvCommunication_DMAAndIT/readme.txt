/**
  @page I2C_OneBoard_AdvCommunication_DMAAndIT I2C (Master DMA Mode)
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_LL/I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_OneBoard_AdvCommunication_DMAAndIT I2C example (Master DMA Mode).
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example describes how to exchange some datas between an I2C Master device
using DMA mode and an I2C Slave device using IT mode. Peripheral initialization done
using LL unitary services functions for optimization purpose (performance and size).

This example guides you through the different configuration steps by mean of LL API
to configure GPIO, DMA and I2C peripherals using only one STM32F302R8-Nucleo Rev C.

I2C1 Peripheral is configured in Slave mode with EXTI (Clock 400Khz, Own address 7-bit enabled).
I2C2 Peripheral is configured in Master mode with DMA (Clock 400Khz).
GPIO associated to User push-button is linked with EXTI. 

LED2 blinks quickly to wait for user-button press. 

Example execution:
Press the User push-button to initiate a write request by Master through Handle_I2C_Master_Transmit() or 
through Handle_I2C_Master_TransmitReceive() routine depends on Command Code type.

Command code type is decomposed in two categories :
1- Action Command code
    a. Type of command which need an action from Slave Device without send any specific anwser)
    b. I2C sequence is composed like that :
     _____________________________________________________________________________________
    |_START_|_Slave_Address_|_Wr_|_A_|_Command_Code_BYTE_1_|_A_|_Command_Code_BYTE_2_|_A_|....
     ________________________________
    |_Command_Code_BYTE_M_|_A_|_STOP_|

First of all, through Handle_I2C_Master_Transmit() routine, Master device generate an I2C start condition
with the Slave address and a write bit condition.
When address Slave match code is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then checking Address Match Code and direction Write.
This will allow Slave to enter in receiver mode and then acknowledge Master to send the Command code bytes through DMA.
When acknowledge is received on I2C2, DMA transfer the Command code data from flash memory buffer to I2C2 TXDR register.
This will allow Master to transmit a byte to the Slave.
Each time a byte is received on I2C1 (Slave), an RXNE interrupt occurs and byte is stored into an internal buffer
until a STOP condition.
And so each time the Slave acknowledge the byte received,
DMA transfer the next data from flash memory buffer to I2C2 TXDR register until Transfer completed.
Master auto-generate a Stop condition when DMA transfer is achieved.

The STOP condition generate a STOP interrupt and initiate the end of reception on Slave side.
I2C1 IRQ handler and Handle_I2C_Master_Transmit() routine are then clearing the STOP flag in both side.

LED2 is On if data are well sent and the Command Code sent to slave is print in the Terminal I/O.

2- Request Command code :
    a. Type of command which need a specific data answer from Slave Device.
    b. I2C sequence is composed like that :
     _____________________________________________________________________________________
    |_START_|_Slave_Address_|_Wr_|_A_|_Command_Code_BYTE_1_|_A_|_Command_Code_BYTE_2_|_A_|....
     ______________________________________________________________________________
    |_Command_Code_BYTE_M_|_A_|_RESTART_|_Slave_Address_|_Rd_|_A_|_Data_BYTE_1_|_A_|...
     ___________________________________________
    |_Data_BYTE_2_|_A_|_Data_BYTE_N_|_NA_|_STOP_|

First of all, through Handle_I2C_Master_TransmitReceive() routine, Master device generate an I2C start condition
with the Slave address and a write bit condition.
When address Slave match code is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then checking Address Match Code and direction Write.
This will allow Slave to enter in receiver mode and then acknowledge Master to send the Command code bytes through DMA.
When acknowledge is received on I2C2, DMA transfer the Command code data from flash memory buffer to I2C2 TXDR register.
This will allow Master to transmit a byte to the Slave.
Each time a byte is received on I2C1 (Slave), an RXNE interrupt occurs and byte is stored into an internal buffer
until a RESTART condition.
And so each time the Slave acknowledge the byte received,
DMA transfer the next data from flash memory buffer to I2C2 TXDR register until Transfer completed.
Then Master device generate a RESTART condition with Slave address and a read bit condition.
When address Slave match code is received on I2C1, an ADDR interrupt occurs.
I2C1 IRQ Handler routine is then checking Address Match Code and direction Read.
This will allow Slave to enter in transmitter mode and then send a byte when TXIS interrupt occurs.
When byte is received on I2C2, an RXNE event occurs and DMA transfer data from RXDR register to an internal buffer
until end of transfer.
Master auto-generate a NACK and STOP condition to inform the Slave that the transfer is finished.

The STOP condition generate a STOP interrupt and initiate the end of transmission on Slave side.
I2C1 IRQ handler and Handle_I2C_Master_TransmitReceive() routine are then clearing the STOP flag in both side.

LED2 is On (500 ms) if data are well sent and the Command Code sent to slave is print as follow depending of IDE :
Note that
 - When resorting to EWARM IAR IDE:
 Command Code is displayed on debugger as follows: View --> Terminal I/O

 - When resorting to MDK-ARM KEIL IDE:
 Command Code is displayed on debugger as follows: View --> Serial Viewer --> Debug (printf) Viewer

- When resorting to AC6 SW4STM32 IDE:
 In Debug configuration window\ Startup, in addition to "monitor reset halt" add the command "monitor arm semihosting enable"
 Command Code is displayed on debugger as follows: Window--> Show View--> Console.

After each use cases, the LED2 blinks quickly to wait for a new user-button press to send a new Command code to the Slave device.

In all cases, if an error occurs, LED2 is blinking slowly.


@par Directory contents 

  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Inc/main.h                  Header for main.c module
  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Src/stm32f3xx_it.c          Interrupt handlers
  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Src/main.c                  Main program
  - I2C/I2C_OneBoard_AdvCommunication_DMAAndIT/Src/system_stm32f3xx.c      STM32F3xx system source file

@par Hardware and Software environment

  - This example runs on STM32F302x8 devices.
    
  - This example has been tested with STM32F302R8-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board.

  - STM32F302R8-Nucleo Rev C Set-up
    - Connect GPIOs connected to I2C1 SCL/SDA (PB.8 and PB.9)
    to respectively SCL and SDA pins of I2C2 (PA.9 and PA.10).
      - I2C1_SCL  PB.8 (CN10, pin 3) : connected to I2C2_SCL PA.9 (CN10, pin 21) 
      - I2C1_SDA  PB.9 (CN10, pin 5) : connected to I2C2_SDA PA.10 (CN10, pin 33)

  - Launch the program in debug mode to benefit of Terminal I/O information. Press User push-button to initiate a write request by Master 
      then Slave receive bytes.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
