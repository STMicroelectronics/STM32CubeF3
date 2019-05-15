/**
  ******************************************************************************
  * @file    stm32f3_discovery.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32F3-Discovery's Leds, push-
  *          buttons hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F3_DISCOVERY_H
#define __STM32F3_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32F3_DISCOVERY STM32F3-DISCOVERY
  * @{
  */ 
 
/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
   
/** @defgroup STM32F3_DISCOVERY_Common STM32F3-DISCOVERY Common
  * @{
  */
  
/** @defgroup STM32F3_DISCOVERY_Private_Constants Private Constants
  * @{
  */
/**
* @}
*/        
      
/** @defgroup STM32F3_DISCOVERY_Private_Variables Private Variables
  * @{
  */ 
/**
* @}
  */ 

/** @defgroup STM32F3_DISCOVERY_Exported_Types Exported Types
  * @{
  */

/**
 * @brief LED Types Definition
 */
typedef enum
{
  LED3 = 0,
  LED4 = 1,
  LED5 = 2,
  LED6 = 3,
  LED7 = 4,
  LED8 = 5,
  LED9 = 6,
  LED10 = 7,
  
  LED_GREEN  = LED6, 
  LED_ORANGE = LED5, 
  LED_RED    = LED3, 
  LED_BLUE   = LED4,
  LED_GREEN_2  = LED7,
  LED_ORANGE_2 = LED8,
  LED_RED_2    = LED10,
  LED_BLUE_2   = LED9
}Led_TypeDef;

/**
 * @brief BUTTON Types Definition
 */
typedef enum 
{
  BUTTON_USER = 0

}Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1

}ButtonMode_TypeDef;


/**
  * @}
  */ 

/** @defgroup STM32F3_DISCOVERY_Exported_Constants Exported Constants
  * @{
  */ 

/** 
  * @brief  Define for STM32F3_DISCOVERY board  
  */ 

#if !defined (USE_STM32F3_DISCO)
 #define USE_STM32F3_DISCO
#endif


/** @defgroup STM32F3_DISCOVERY_LED STM32F3-DISCOVERY LED
  * @{
  */
#define LEDn                             8

#define LED6_PIN                         GPIO_PIN_15
#define LED6_GPIO_PORT                   GPIOE
#define LED6_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE() 
#define LED6_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()

#define LED8_PIN                         GPIO_PIN_14
#define LED8_GPIO_PORT                   GPIOE
#define LED8_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE() 
#define LED8_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()
  
#define LED10_PIN                        GPIO_PIN_13
#define LED10_GPIO_PORT                  GPIOE
#define LED10_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED10_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()
  
#define LED9_PIN                         GPIO_PIN_12
#define LED9_GPIO_PORT                   GPIOE
#define LED9_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED9_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()
  
#define LED7_PIN                         GPIO_PIN_11
#define LED7_GPIO_PORT                   GPIOE
#define LED7_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED7_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()

#define LED5_PIN                         GPIO_PIN_10
#define LED5_GPIO_PORT                   GPIOE
#define LED5_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()

#define LED3_PIN                         GPIO_PIN_9
#define LED3_GPIO_PORT                   GPIOE
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()

#define LED4_PIN                         GPIO_PIN_8
#define LED4_GPIO_PORT                   GPIOE
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__LED__)   do { if ((__LED__) == LED3) LED3_GPIO_CLK_ENABLE(); else\
                                             if ((__LED__) == LED4) LED4_GPIO_CLK_ENABLE(); else\
                                             if ((__LED__) == LED5) LED5_GPIO_CLK_ENABLE(); else\
                                             if ((__LED__) == LED6) LED6_GPIO_CLK_ENABLE(); else\
                                             if ((__LED__) == LED7) LED7_GPIO_CLK_ENABLE(); else\
                                             if ((__LED__) == LED8) LED8_GPIO_CLK_ENABLE(); else\
                                             if ((__LED__) == LED9) LED9_GPIO_CLK_ENABLE(); else\
                                             if ((__LED__) == LED10) LED10_GPIO_CLK_ENABLE();} while(0)

#define LEDx_GPIO_CLK_DISABLE(__LED__)  (((__LED__) == LED3) ? LED3_GPIO_CLK_DISABLE() :\
                                         ((__LED__) == LED4) ? LED4_GPIO_CLK_DISABLE() :\
                                         ((__LED__) == LED5) ? LED5_GPIO_CLK_DISABLE() :\
                                         ((__LED__) == LED6) ? LED6_GPIO_CLK_DISABLE() :\
                                         ((__LED__) == LED7) ? LED7_GPIO_CLK_DISABLE() :\
                                         ((__LED__) == LED8) ? LED8_GPIO_CLK_DISABLE() :\
                                         ((__LED__) == LED9) ? LED9_GPIO_CLK_DISABLE() :\
                                         ((__LED__) == LED10) ? LED10_GPIO_CLK_DISABLE() : 0 )

/**
  * @}
  */ 
  
/** @defgroup STM32F3_DISCOVERY_BUTTON STM32F3-DISCOVERY BUTTON
  * @{
  */  
#define BUTTONn                          1  

/**
 * @brief push-button
 */
#define USER_BUTTON_PIN                  GPIO_PIN_0
#define USER_BUTTON_GPIO_PORT            GPIOA
#define USER_BUTTON_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOA_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQn            EXTI0_IRQn 

#define BUTTONx_GPIO_CLK_ENABLE(__BUTTON__)  do { if ((__BUTTON__) == BUTTON_USER) USER_BUTTON_GPIO_CLK_ENABLE();} while(0)

#define BUTTONx_GPIO_CLK_DISABLE(__BUTTON__) (((__BUTTON__) == BUTTON_USER) ? USER_BUTTON_GPIO_CLK_DISABLE() : 0 )

/**
  * @}
  */ 

/** @defgroup STM32F3_DISCOVERY_COM STM32F3-DISCOVERY COM
  * @{
  */  
  
/*##################### SPIx ###################################*/
/**
  * @brief  Definition for SPI Interface pins (SPI1 used)
  */
#define DISCOVERY_SPIx                        SPI1
#define DISCOVERY_SPIx_CLK_ENABLE()           __HAL_RCC_SPI1_CLK_ENABLE()
#define DISCOVERY_SPIx_GPIO_PORT              GPIOA                      /* GPIOA */
#define DISCOVERY_SPIx_AF                     GPIO_AF5_SPI1
#define DISCOVERY_SPIx_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define DISCOVERY_SPIx_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOA_CLK_DISABLE()
#define DISCOVERY_SPIx_SCK_PIN                GPIO_PIN_5                 /* PA.05 */
#define DISCOVERY_SPIx_MISO_PIN               GPIO_PIN_6                 /* PA.06 */
#define DISCOVERY_SPIx_MOSI_PIN               GPIO_PIN_7                 /* PA.07 */
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SPIx_TIMEOUT_MAX                      ((uint32_t)0x1000)

/*##################### I2Cx ###################################*/
/**
  * @brief  Definition for I2C Interface pins (I2C1 used)
  */
#define DISCOVERY_I2Cx                        I2C1
#define DISCOVERY_I2Cx_CLK_ENABLE()           __HAL_RCC_I2C1_CLK_ENABLE()
#define DISCOVERY_I2Cx_CLK_DISABLE()          __HAL_RCC_I2C1_CLK_DISABLE()
#define DISCOVERY_I2Cx_FORCE_RESET()          __HAL_RCC_I2C1_FORCE_RESET()
#define DISCOVERY_I2Cx_RELEASE_RESET()        __HAL_RCC_I2C1_RELEASE_RESET() 

#define DISCOVERY_I2Cx_SCL_PIN                GPIO_PIN_6                  /* PB.06 */
#define DISCOVERY_I2Cx_SDA_PIN                GPIO_PIN_7                  /* PB.07 */

#define DISCOVERY_I2Cx_GPIO_PORT              GPIOB                       /* GPIOB */
#define DISCOVERY_I2Cx_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 
#define DISCOVERY_I2Cx_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOB_CLK_DISABLE()
#define DISCOVERY_I2Cx_AF                     GPIO_AF4_I2C1

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define I2Cx_TIMEOUT_MAX                      0x10000     

/**
  * @}
  */ 


/** @defgroup STM32F3_DISCOVERY_COMPONENT STM32F3-DISCOVERY COMPONENT
  * @{
  */
/*##################### GYRO ##########################*/
/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80) 
/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

/* Chip Select macro definition */
#define GYRO_CS_LOW()       HAL_GPIO_WritePin(GYRO_CS_GPIO_PORT, GYRO_CS_PIN, GPIO_PIN_RESET)
#define GYRO_CS_HIGH()      HAL_GPIO_WritePin(GYRO_CS_GPIO_PORT, GYRO_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  GYRO SPI Interface pins
  */
#define GYRO_CS_GPIO_PORT            GPIOE                       /* GPIOE */
#define GYRO_CS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOE_CLK_ENABLE()
#define GYRO_CS_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOE_CLK_DISABLE()
#define GYRO_CS_PIN                  GPIO_PIN_3                  /* PE.03 */

#define GYRO_INT_GPIO_PORT           GPIOE                       /* GPIOE */
#define GYRO_INT_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()
#define GYRO_INT_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOE_CLK_DISABLE()
#define GYRO_INT1_PIN                GPIO_PIN_0                  /* PE.00 */
#define GYRO_INT1_EXTI_IRQn          EXTI0_IRQn 
#define GYRO_INT2_PIN                GPIO_PIN_1                  /* PE.01 */
#define GYRO_INT2_EXTI_IRQn          EXTI1_IRQn 

/*##################### ACCELEROMETER ##########################*/
/**
  * @brief  ACCELEROMETER I2C1 Interface pins
  */
#define ACCELERO_I2C_ADDRESS             0x32

#define ACCELERO_DRDY_PIN                GPIO_PIN_2                  /* PE.02 */
#define ACCELERO_DRDY_GPIO_PORT          GPIOE                       /* GPIOE */
#define ACCELERO_DRDY_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE() 
#define ACCELERO_DRDY_GPIO_CLK_DISABLE() __HAL_RCC_GPIOE_CLK_DISABLE() 
#define ACCELERO_DRDY_EXTI_IRQn          EXTI2_TSC_IRQn              /*TAMP_STAMP_IRQn*/

#define ACCELERO_INT_GPIO_PORT           GPIOE                       /* GPIOE */
#define ACCELERO_INT_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()
#define ACCELERO_INT_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOE_CLK_DISABLE()
#define ACCELERO_INT1_PIN                GPIO_PIN_4                  /* PE.04 */
#define ACCELERO_INT1_EXTI_IRQn          EXTI4_IRQn 
#define ACCELERO_INT2_PIN                GPIO_PIN_5                  /* PE.05 */
#define ACCELERO_INT2_EXTI_IRQn          EXTI9_5_IRQn 

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32F3_DISCOVERY_Exported_Functions Exported Functions
  * @{
  */
uint32_t  BSP_GetVersion(void);
void      BSP_LED_Init(Led_TypeDef Led);
void      BSP_LED_On(Led_TypeDef Led);
void      BSP_LED_Off(Led_TypeDef Led);
void      BSP_LED_Toggle(Led_TypeDef Led);
void      BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
uint32_t  BSP_PB_GetState(Button_TypeDef Button);

/**
  * @}
  */
  
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F3_DISCOVERY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
