/**
  @page CEC CEC_MultiAddress example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CEC/CEC_MultiAddress/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CEC Multiple Address communication example.
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

How to configure and use the CEC peripheral to receive and transmit messages 
in the case where one device supports two distinct logical addresses at the 
same time.

- Hardware Description

To use this example, two STM32373C-EVAL RevB boards (called Device_1 and 
Device_2) are loaded with the same software then connected through CEC lines
(PB8 or HDMI connectors) and GND.
 /|\  In the firmware file main.h, uncomment the dedicated line to use
/_!_\ the CEC peripheral as STM32 device_1 or STM32 device_2.


@verbatim
*------------------------------------------------------------------------------*
|           STM32373C-EVAL RevB                               STM32373C-EVAL RevB              |
|         Device Address :0x01                    Device Address :0x03         |
|                                                 Device Address :0x05)        |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |   CEC    |____|____CECLine______|____|   CEC    |    |         |
|        |    | Device_1 |    |                 |    | Device_2 |    |         |
|        |    |__________|    |                 |    |__________|    |         |
|        |                    |                 |                    |         |
|        |  O LD1             |                 |  O LD1             |         |
|        |  O LD2    Joystick |                 |  O LD2    Joystick |         |
|        |  O LD3        _    |                 |  O LD3        _    |         |
|        |  O LD4       |_|   |                 |  O LD4       |_|   |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim


- Software Description

The test unrolls as follows.

On Device_1 TX side, three possible messages can be transmitted and are 
indicated as below on the transmitting board:
 - when Joystick Left push-button is pressed, a ping message (no payload) is addressed
  to Device_2 first logical address; LED1 toggles
 - when Joystick Down push-button is pressed, a ping message (no payload) is addressed
  to Device_2 second logical address; LED4 toggles  
 - when Joystick Sel push-button is pressed, a broadcast message (with empty
 payload) is sent by Device_1; LED2 toggles
 
When Joystick Up push-button is pressed, LED3 toggles (no message sent)  


Accordingly, the following happens on Device_2 RX side in case of successful
reception:
 - when Joystick Left push-button is pressed on TX side, 
     * LED1 is turned on
     *  LED3 and LED4 are turned off 
 - when Joystick Down push-button is pressed on TX side, 
     * LED4 is turned on
     * LED1 and LED3 are turned off      
 - when Joystick Sel push-button is pressed on TX side, all LEDs are turned off 


Conversely, on Device_2 TX side, only ping messages addressed to the 
single Device_1 logical address or broadcast messages are sent:
 - when Joystick Left push-button is pressed, a ping message (no payload) is addressed
  to Device_2; LED1 toggles
 - when Joystick Down push-button is pressed, a ping message (no payload) is addressed
  to Device_2; LED4 toggles  
 - when Joystick Sel push-button is pressed, a broadcast message (with empty
 payload) is sent; LED2 toggles


Accordingly, the following happens on Device_1 RX side in case of successful
transmission:
 - when Joystick Left push-button or Joystick Down push-button are pressed on TX side, 
      * LED1 and LED4 are turned on
      * LED 3 is turned off     
 - when Joystick Sel push-button is pressed on TX side, all LEDs are turned off 


For Device_1 and _2, in case of transmission or reception error, 
LED3 is turned on.


Practically, 4 EXTI lines (EXTI4, EXTI9_5 and EXTI_Line2_TSC) are configured to 
generate an interrupt on each rising edge. 
A specific message is then transmitted by the CEC IP
and a LED connected to a specific GPIO pin is toggled.
    - EXTI4 is mapped to PF.04
    - EXTI9_5 is mapped to PE.06  
    - EXTI_Line2_TSC is mapped to PF.02 

In this example, HCLK is configured at 72 MHz.


@par Directory contents 

  - CEC/CEC_MultiAddress/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - CEC/CEC_MultiAddress/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - CEC/CEC_MultiAddress/Inc/main.h                  Header for main.c module  
  - CEC/CEC_MultiAddress/Src/stm32f3xx_it.c          Interrupt handlers
  - CEC/CEC_MultiAddress/Src/system_stm32f3xx.c      STM32F3xx system source file
  - CEC/CEC_MultiAddress/Src/main.c                  Main program
  - CEC/CEC_MultiAddress/Src/stm32f3xx_hal_msp.c     IP hardware resources initialization
  
@par Hardware and Software environment

  - This example runs on STM32F373C Devices.
    
  - This example has been tested with STM32373C-EVAL RevB board and can be
    easily tailored to any other supported device and development board.      


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - in main.h, uncomment DEVICE_1 for first board, uncomment DEVICE_2 for second board
 - Rebuild all files and load your image into target memory
 - With a wire, connect HDMI_CEC_LINE_GPIO_PORT-HDMI_CEC_LINE_PIN between the 2 boards
 - Connect the ground of the 2 boards
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
