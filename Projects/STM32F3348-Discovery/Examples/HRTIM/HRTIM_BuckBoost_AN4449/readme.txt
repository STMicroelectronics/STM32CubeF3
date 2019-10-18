/**
  @page HRTIM HRTIM_BuckBoost_AN4449

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    HRTIM/HRTIM_BuckBoost_AN4449/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of HRTIM_BuckBoost_AN4449 example.
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

  This example is related to AN4449 Buck-boost converter.
   
  It demonstrated the buck-boost DC/DC converter application.
  HRTIM is fully configured to control the H-bridge power switching stage.
  Any Vin range [3;15]Vdc can be converted into Vout range [3;15]Vdc with different
  operating modes such as BUCK, BOOST or MIXED (buck-boost).
  User has to enter a VOUT_TARGET parameter in main.h and the application will regulate this value 
  whatever the conditions on Vin.
  External power supply is needed for Vin.
  See AN4449 for furhter details.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - HRTIM/HRTIM_BuckBoost_AN4449/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - HRTIM/HRTIM_BuckBoost_AN4449/Inc/stm32f3xx_it.h          PPP interrupt handlers header file
  - HRTIM/HRTIM_BuckBoost_AN4449/Inc/main.h                  Header for main.c module  
  - HRTIM/HRTIM_BuckBoost_AN4449/Src/stm32f3xx_it.c          PPP interrupt handlers
  - HRTIM/HRTIM_BuckBoost_AN4449/Src/main.c                  Main program
  - HRTIM/HRTIM_BuckBoost_AN4449/Src/stm32f3xx_hal_msp.c     HAL MSP file

@par Hardware and Software environment 

  - This example runs on STM32F334x4/STM32F334x6/STM32F334x8 devices.
    
  - This example has been tested with STM32F3348-DISCO
    board and can be easily tailored to any other supported device and development board.
  

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
