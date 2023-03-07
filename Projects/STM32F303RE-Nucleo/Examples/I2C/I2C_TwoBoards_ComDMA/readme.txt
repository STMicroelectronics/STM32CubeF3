/**
  @page I2C_TwoBoards_ComDMA I2C Two Boards Communication DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    I2C/I2C_TwoBoards_ComDMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C Two Boards Communication DMA example.
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

How to handle I2C data buffer transmission/reception between two boards, 
via DMA.

   ______________________                        ______________________
  |     _______CN10______|                      |_______CN10______     |
  |    | I2C3            |                      |             I2C3|    |
  |    |                 |                      |                 |    |
  |    |(pin 23) SCL(PA8)|______________________|(PA8)SCL (pin 23)|    |
  |    |                 |                      |                 |    |
  |    |                 |                      |                 |    |
  |    |(pin 1)  SDA(PC9)|______________________|(PC9)SDA (pin 1) |    |
  |    |_________________|                      |_________________|    |
  |         __           |                      |             __       |
  |        |__|          |                      |            |__|      |
  |        USER       GND|______________________|GND         USER      |
  |                      |                      |                      |
  |__STM32F303RE_NUCLEO__|                      |__STM32F303RE_NUCLEO__|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 64 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.
The project is split in two parts: the Master Board and the Slave Board
- Master Board
  The HAL_I2C_Master_Receive_DMA() and the HAL_I2C_Master_Transmit_DMA() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Master mode.
- Slave Board
  The HAL_I2C_Slave_Receive_DMA() and the HAL_I2C_Slave_Transmit_DMA() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode.
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button on the Master Board,
I2C Master starts the communication by sending aTxBuffer through HAL_I2C_Master_Transmit_DMA()
to I2C Slave which receives aRxBuffer through HAL_I2C_Slave_Receive_DMA(). 
The second step starts when the user press the User push-button on the Master Board,
the I2C Slave sends aTxBuffer through HAL_I2C_Slave_Transmit_DMA()
to the I2C Master which receives aRxBuffer through HAL_I2C_Master_Receive_DMA().
The end of this two steps are monitored through the HAL_I2C_GetState() function
result.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32F303RE-Nucleo Rev C's LED can be used to monitor the transfer status:
 - LED2 is ON when the transmission process is complete.
 - LED2 is OFF when the reception process is complete.
 - LED2 is slowly blinking (1 sec. period) in case of error in initialization or 
transmission/reception process

@note I2Cx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - I2C/I2C_TwoBoards_ComDMA/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_ComDMA/Inc/stm32f3xx_it.h          DMA interrupt handlers header file
  - I2C/I2C_TwoBoards_ComDMA/Inc/main.h                  Header for main.c module  
  - I2C/I2C_TwoBoards_ComDMA/Src/stm32f3xx_it.c          DMA interrupt handlers
  - I2C/I2C_TwoBoards_ComDMA/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_ComDMA/Src/system_stm32f3xx.c      STM32F3xx system source file
  - I2C/I2C_TwoBoards_ComDMA/Src/stm32f3xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F303RE devices.
    
  - This example has been tested with STM32F303RE-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board.    

  -STM32F303RE-Nucleo Rev C Set-up
    - Connect Master board PA8 to Slave Board PA8 (CN10, pin 23)
    - Connect Master board PC9 to Slave Board PC9 (CN10, pin 1)
    - Connect Master board GND to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
