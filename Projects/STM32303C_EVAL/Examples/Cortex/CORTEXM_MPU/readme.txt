    /**
  @page CortexM_MPU Cortex-M4 MPU example

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CORTEXM/CORTEXM_MPU/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CortexM MPU example.
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

Presentation of the MPU feature. This example configures a memory area as 
privileged read-only, and attempts to perform read and write operations in
different modes.

If the access is permitted LED1 is toggling. If the access is not permitted, 
a memory management fault is generated and LED2 is ON.

To generate an MPU memory fault exception due to an access right error, uncomment
the following line "PrivilegedReadOnlyArray[0] = 'e';" in the "stm32_mpu.c" file.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - CortexM/MPU/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - CortexM/MPU/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - CortexM/MPU/Inc/main.h                  Header for main.c module  
  - CortexM/MPU/Inc/stm32_mpu.h             MPU management
  - CortexM/MPU/Src/stm32f3xx_it.c          Interrupt handlers
  - CortexM/MPU/Src/main.c                  Main program
  - CortexM/MPU/Src/system_stm32f3xx.c      STM32F3xx system source file
  - CortexM/MPU/Src/stm32_mpu.c             MPU management

@par Hardware and Software environment

  - This example runs on STM32F303xC devices.
    
  - This example has been tested with STM32303C-EVAL RevC board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Add the required example files
   - stm32_mpu.c

 - Rebuild all files and load your image into target memory
 - Run the example


 */
