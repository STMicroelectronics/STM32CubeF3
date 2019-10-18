/**
  @page SMBUS_TSENSOR SMBUS Temperature Sensor example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    SMBUS/SMBUS_TSENSOR/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the SMBUS Temperature Sensor example.
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

SMBUS data buffer transmission/reception using an interrupt. The STM32 microcontroller communicates with an SMBUS temperature sensor.

   __________________________________________________________________________                        
  |           ______________                        ______________           |
  |          | I2C2         |                      |   TSENSOR    |          |
  |          |              |                      |              |          |
  |          |      SCL(PF6)|______________________|CLOCK         |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |     SDA(PA10)|______________________|DATA          |          |
  |          |              |                      |              |          |
  |          |SMB alert(PA8)|______________________|EVENT         |          |
  |          |______________|                      |______________|          |
  |      __                                                                  |
  |     |__|                                                                 |
  |     Key                                                                  |
  |                                                                          |
  |__________________________________________________________________________|
      STM32303C-EVAL RevC

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 72 MHz.

The temperature sensor configuration is ensured by TSENSOR_Init() which verify that
temperature sensor is ready to communicate by calling TSENSOR_IsReady(), then
set the configuration using HAL_SMBUS_Master_Transmit_IT() and HAL_SMBUS_EnableAlert_IT()

The SMBUS peripheral configuration is ensured by the HAL_SMBUS_Init()
and HAL_SMBUS_IsDeviceReady() called respectively in TSENSOR_Init() function.
This later is calling the HAL_SMBUS_MspInit()function which core is implementing
the configuration of the needed SMBUS resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change SMBUS configuration.

The SMBUS/Temperature Sensor communication is then initiated.

Then at each loop in main program a read and a display on LCD of the current temperature
is done each second.
An specific warning message is display on LCD in case of an alert from temperature sensor
is detected.

This management is perform using the TSENSOR_ReadTemp(), TSENSOR_AlerteResponseAddressRead()
and TSENSOR_ReadStatus() functions allow respectively the read of the current temperature,
get the address of temperature sensor in alert and get the temperature sensor status.

STM32303C-EVAL RevC's Messages and LEDs can be used to monitor the temperature sensor status:
 - LED3 is ON when there is an error during communication with the temperature sensor.
 - In case of the Temperature high limit exceeded, this blink warning message is display on the LCD
 until alert limit disappear :
                                    Temperature Limit High
                                       has been exceeded

 - In case of the Temperature is at or blow low limit,a blink warning message is display on the LCD
 until alert limit disappear :
                                      Temperature is at or
                                       below the Low Limit
                 

@note TSENSOR_SMBUS instance used and associated resources can be updated in "stm32f3xx_tsensor.c"
and "main.h" files depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - SMBUS/SMBUS_TSENSOR/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - SMBUS/SMBUS_TSENSOR/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - SMBUS/SMBUS_TSENSOR/Inc/main.h                  Header for main.c module  
  - SMBUS/SMBUS_TSENSOR/Inc/stm32f3xx_tsensor.h     Temperature Sensor driver header file
  - SMBUS/SMBUS_TSENSOR/Src/stm32f3xx_it.c          Interrupt handlers
  - SMBUS/SMBUS_TSENSOR/Src/main.c                  Main program
  - SMBUS/SMBUS_TSENSOR/Src/system_stm32f3xx.c      STM32F3xx system source file
  - SMBUS/SMBUS_TSENSOR/Src/stm32f3xx_hal_msp.c     HAL MSP file    
  - SMBUS/SMBUS_TSENSOR/Src/stm32f3xx_tsensor.c     Temperature Sensor driver file

@par Hardware and Software environment

  - This example runs on STM32F303xC devices.
    
  - This example has been tested with STM32303C-EVAL RevC board and can be
    easily tailored to any other supported device and development board.    

  - STM32303C-EVAL RevC Set-up 
    - Make sure the jumper JP5 and JP6 are set in I2C2 position.
    - Make sure the jumper JP1 (SMB) is fitted on the board.
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
