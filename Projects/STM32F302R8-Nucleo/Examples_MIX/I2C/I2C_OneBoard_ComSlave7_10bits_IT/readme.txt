/**
  @page I2C_OneBoard_ComSlave7_10bits_IT I2C One Board Communication Slave 10-Bit
  then Slave 7-Bit IT example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C One Board Master Communication with 
  *          a Slave 10-Bit address then a Slave 7-Bit address example.
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

How to perform I2C data buffer transmission/reception between
one master and two slaves with different address sizes (7-bit or 10-bit). This example
uses the STM32F3xx I2C HAL and LL API (LL API usage for performance improvement)
and an interrupt.

Board: STM32F302R8-Nucleo Rev C (embeds a STM32F302R8T6 device)
SCL MASTER Pin: PA.9 (CN10, pin 21)
SDA MASTER Pin: PA.10 (CN10, pin 33)

SCL SLAVE Pin: PB.8 (CN10, pin 3)
SDA SLAVE Pin: PB.9 (CN10, pin 5)
   ____________________________________________
  |     ____________          ____________     |
  |    |    I2C2    |        |    I2C1    |    |
  |    |            |        |            |    |
  |    |  SCL_MASTER|________|SCL_SLAVE   |    |
  |    |            |        |            |    |
  |    |            |        |            |    |
  |    |  SDA_MASTER|________|SDA_SLAVE   |    |
  |    |____________|        |____________|    |
  |               __                           |
  |              |__|                          |
  |              USER                          |
  |                                            |
  |___________STM32F302R8-Nucleo Rev C_________|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 64 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change I2C configuration.

For this example two buffers are used 
- aTxBuffer buffer contains the data to be transmitted 
- aRxBuffer buffer is used to save the received data
Note that both buffers have same size
                       
Example execution:
First step, press the User push-button, this action initiates a reception process (aRxBuffer) for
I2C Slave1 or Slave2 address through HAL_I2C_Slave_Receive_IT().
Then I2C Master starts the communication by sending aTxBuffer through HAL_I2C_Master_Transmit_IT()
for Slave1 (10-Bit address). 
The end of this step is monitored through the HAL_I2C_GetState() function
result.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.
Toggle LED2 when data is received correctly, otherwise LED2 is slowly blinking (1 sec. period)
and communication is stopped (using infinite loop)

Second step, press the User push-button, this action initiates a transmission process (aTxBuffer) for
I2C Slave1 or Slave2 address through HAL_I2C_Slave_Transmit_IT().
Then I2C Master starts the communication by receiving aRxBuffer through HAL_I2C_Master_Transmit_IT()
from Slave2 (7-Bit address).
The end of this two steps are monitored through the HAL_I2C_GetState() function
result.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.
Toggle LED2 when data is received correctly, otherwise LED2 is slowly blinking (1 sec. period)
and communication is stopped (using infinite loop)

@note In Master side, only Acknowledge failure error is handled. When this error
       occurs Master restart the current operation until Slave acknowledges it's
       address.
        
@note I2C2 and I2C1 instance used and associated resources can be updated in "main.h"
       file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Inc/stm32f3xx_it.h          I2C interrupt handlers header file
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Inc/main.h                  Header for main.c module  
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Src/stm32f3xx_it.c          I2C interrupt handlers
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Src/main.c                  Main program
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Src/system_stm32f3xx.c      STM32F3xx system source file
  - Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Src/stm32f3xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F302x8 devices.
    
  - This example has been tested with STM32F302R8-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board.    

  -STM32F302R8-Nucleo Rev C Set-up
    - Connect GPIOs connected to I2C1 SCL/SDA (PB.8 and PB.9)
    to respectively SCL and SDA pins of I2C2 (PA.9 and PA.10).
      - I2C1_SCL  PB.8 (CN10, pin 3) : connected to I2C2_SCL PA.9 (CN10, pin 21)
      - I2C1_SDA  PB.9 (CN10, pin 5) : connected to I2C2_SDA PA.10 (CN10, pin 33)

  - Launch the program.
  - Press User push-button to initiate a write request by Master
    then Slave1 (10-Bit address) receive a Buffer. LED2 Toggle when data is received correctly.
  - Press again User push-button to initiate a read request by Master
    then Slave2 (7-Bit address) Transmit a Buffer. LED2 Toggle when data is received correctly.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
