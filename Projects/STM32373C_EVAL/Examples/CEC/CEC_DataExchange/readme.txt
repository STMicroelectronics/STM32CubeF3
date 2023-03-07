/**
  @page CEC CEC_DataExchange example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CEC/CEC_DataExchange/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CEC Data Exchange example.
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

How to configure and use the CEC peripheral to receive and transmit messages.

- Hardware Description

To use this example, two STM32373C-EVAL RevB boards (called Device_1 and 
Device_2) are loaded with the same software then connected through CEC lines
(PB8 or HDMI connectors) and GND.

 /|\  In the firmware file main.h, uncomment the dedicated line to use
/_!_\ the CEC peripheral as STM32 device_1 or STM32 device_2.

@verbatim
*------------------------------------------------------------------------------*
|           STM32373C-EVAL RevB                         STM32373C-EVAL RevB                    |
|         Device Address :0x01                    Device Address :0x03         |
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

On TX side, four possible messages can be transmitted and are indicated as
below on the transmitting board:
 - when Joystick Left push-button is pressed, LED1 toggles
 - when Joystick Sel push-button is pressed, LED2 toggles
 - when Joystick Up push-button is pressed, LED3 toggles
 - when Joystick Down push-button is pressed, LED4 toggles

Accordingly, the following happens on the RX side in case of successful
reception:
 - when Joystick Left push-button is pressed on TX side, 
     * all RX side LEDs are turned off 
 - when Joystick Sel push-button is pressed on TX side, on RX side
     *  LED1 and LED2 are turned on
     *  LED3 and LED4 are turned off 
 - when Joystick Up push-button is pressed on TX side, 
     *  all RX side LEDs are turned on
 - when Joystick Down push-button is pressed on TX side, on RX side 
     * LED1 and LED2 are turned off
     * LED3 and LED4 are turned on    
In case of unsuccessful reception, LED3 is turned on.

Practically, 4 EXTI lines (EXTI4, EXTI9_5, EXTI15_10 and EXTI_Line2_TSC) are configured to 
generate an interrupt on each rising edge. 
A specific message is then transmitted by the CEC IP
and a LED connected to a specific GPIO pin is toggled.
    - EXTI4 is mapped to PF.04
    - EXTI9_5 is mapped to PE.06
    - EXTI15_10 is mapped to PF.10     
    - EXTI_Line2_TSC is mapped to PF.02 
    
Then, on TX side,
  - when rising edge is detected on EXTI4, LED1 toggles
  - when rising edge is detected on EXTI9_5, LED2 toggles
  - when rising edge is detected on EXTI15_10, LED3 toggles
  - when rising edge is detected on EXTI_Line2_TSC, LED4 toggles
   

In this example, HCLK is configured at 72 MHz.

@par Directory contents 

  - CEC/CEC_DataExchange/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - CEC/CEC_DataExchange/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - CEC/CEC_DataExchange/Inc/main.h                  Header for main.c module  
  - CEC/CEC_DataExchange/Src/stm32f3xx_it.c          Interrupt handlers
  - CEC/CEC_DataExchange/Src/system_stm32f3xx.c      STM32F3xx system source file
  - CEC/CEC_DataExchange/Src/main.c                  Main program
  - CEC/CEC_DataExchange/Src/stm32f3xx_hal_msp.c     IP hardware resources initialization
  
@par Hardware and Software environment

  - This example runs on STM32F373xC devices.
    
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


 */
