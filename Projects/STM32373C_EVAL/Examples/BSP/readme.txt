/**
  @page BSP  Example on how to use the BSP drivers
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    BSP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the BSP example.
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

This example provides a description of how to use the different BSP drivers. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 72 MHz.

This example shows how to use the different functionalities of components 
available on the board by switching between all tests using Key push-button button. 

 ** Push the User button to start first Test.  
Blue Led (LD4) will blink between each test. Press Key push-button to start another test:

 ** JOYSTICK **
Use the joystick button to move a pointer inside a rectangle 
(up/down/right/left) and change the pointer color(select).

 ** LCD **
This example shows how to use the different LCD features to display string
with different fonts, to display different shapes and to draw a bitmap.

 ** SD **
This example shows how to erase, write and read the SD card and also 
how to detect the presence of the card.

 ** EEPROM **
This example show how to read and write data in EEPROM I2C M24LR64 connected on STM32373C-EVAL RevB
or EEPROM I2C M24M01-HR mounted on STM32373C-EVAL RevB: 
   * The I2C RF EEPROM memory (M24LR64) is available through the connector CN3.
   @note Make sure that JP4 and JP5 set to I2C2 position.
   * The I2C EEPROM memory (M24M01-HR) is directly mounted on STM32373C-EVAL RevB
   @note Make sure that JP4 and JP5 set to I2C2_F position

 ** TSENSOR **
This example show how to read a Temperature using the temperature sensor.
   @note Make sure that JP4 and JP5 set to I2C2 position.

 ** AUDIO PLAY **
This example show how to play/pause/resume or change the volume (min 50% / max 100 %)
of an audio playback using Key or Joystick button.
   @note Make sure that JP4 and JP5 set to I2C2 position.

      Plug a headphone to ear the sound  /!\ Take care of yours ears.
      Default volume is 60%.
      A press of JOY_SEL button will pause the audio file play (LED 1 green keep previous state
       and LED2 orange ON).
      Another press on JOY_SEL button will resumes audio file play (only LED 1 green blink at each replay)
      @Note: Copy file 'audio.bin' (available in Utilities\Media folder) directly in the flash 
      at @0x08015000 using ST-Link utility
      A press of JOY_UP button will increment the volume of the audio playback.
      A press of JOY_DOWN button will decrement the volume of the audio playback.


 ** LCD LOG **
This example show how to use the LCD log features. 

@par Directory contents 

  - BSP/Src/main.c                 Main program
  - BSP/Src/system_stm32f3xx.c     STM32F3xx system clock configuration file
  - BSP/Src/stm32f3xx_it.c         Interrupt handlers 
  - BSP/Src/joystick.c             joystick feature
  - BSP/Src/lcd.c                  LCD drawing features
  - BSP/Src/log.c                  LCD Log firmware functions
  - BSP/Src/sd.c                   SD features
  - BSP/Src/eeprom.c               EEPROM Read/Write features
  - BSP/Src/temperature_sensor.c   Temperature Sensor features
  - BSP/Src/audio_play.c           Audio Playback features
  - BSP/AudioFile/audio.bin        Audio wave extract.
  - BSP/Inc/main.h                 Main program header file  
  - BSP/Inc/stm32f3xx_conf.h       HAL Configuration file
  - BSP/Inc/stm32f3xx_it.h         Interrupt handlers header file
  - BSP/Inc/lcd_log_conf.h         lcd_log configuration template file
  - BSP/Inc/stlogo.h               Image used for BSP example
        
@par Hardware and Software environment  

  - This example runs on STM32F373C devices.
    
  - This example has been tested with STMicroelectronics STM32373C-EVAL RevB
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
