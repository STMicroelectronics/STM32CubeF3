/** 
  ******************************************************************************
  * @file    stm32f3348_discovery.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32F3348-DISCO Kit's Leds and
  *          push-button hardware resources.
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
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F3348_DISCOVERY_H
#define __STM32F3348_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32F3348_DISCOVERY STM32F3348-DISCOVERY
  * @{
  */ 
 
/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
   
/** @defgroup STM32F3348_DISCOVERY_Common STM32F3348-DISCOVERY Common
  * @{
  */
  
/** @defgroup STM32F3348_DISCOVERY_Private_Constants Private Constants
  * @{
  */
/**
* @}
*/        
      
/** @defgroup STM32F3348_DISCOVERY_Private_Variables Private Variables
  * @{
  */ 
/**
* @}
  */ 

/** @defgroup STM32F3348_DISCOVERY_Exported_Types Exported Types
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

  LED_GREEN  = LED5,
  LED_ORANGE = LED4, 
  LED_RED    = LED3, 
  LED_BLUE   = LED6
}Led_TypeDef;

/**
 * @brief BUTTON Types Definition
 */
typedef enum 
{
  BUTTON_USER = 0,
}Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1

}ButtonMode_TypeDef;


/**
  * @}
  */ 

/** @defgroup STM32F3348_DISCOVERY_Exported_Constants Exported Constants
  * @{ 
  */ 

/** 
* @brief  Define for STM32F3348_DISCOVERY board  
*/ 
#if !defined (USE_STM32F3348_DISCO)
 #define USE_STM32F3348_DISCO
#endif

/** @defgroup STM32F3348_DISCOVERY_LED STM32F3348-DISCOVERY LED
  * @{
  */

#define LEDn                             4

#define LED3_PIN                         GPIO_PIN_6
#define LED3_GPIO_PORT                   GPIOB
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()  

#define LED4_PIN                         GPIO_PIN_8
#define LED4_GPIO_PORT                   GPIOB
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()  

#define LED5_PIN                         GPIO_PIN_9
#define LED5_GPIO_PORT                   GPIOB
#define LED5_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED5_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()  

#define LED6_PIN                         GPIO_PIN_7
#define LED6_GPIO_PORT                   GPIOB
#define LED6_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED6_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()  



#define LEDx_GPIO_CLK_ENABLE(__LED__)   do { if ((__LED__) == LED3) LED3_GPIO_CLK_ENABLE(); else\
                                             if ((__LED__) == LED4) LED4_GPIO_CLK_ENABLE(); else\
                                             if ((__LED__) == LED5) LED5_GPIO_CLK_ENABLE(); else\
                                             if ((__LED__) == LED6) LED6_GPIO_CLK_ENABLE();} while(0)

#define LEDx_GPIO_CLK_DISABLE(__LED__)  (((__LED__) == LED3) ? LED3_GPIO_CLK_DISABLE() :\
                                         ((__LED__) == LED4) ? LED4_GPIO_CLK_DISABLE() :\
                                         ((__LED__) == LED5) ? LED5_GPIO_CLK_DISABLE() :\
                                         ((__LED__) == LED6) ? LED6_GPIO_CLK_DISABLE() : 0 )
/**
  * @}
  */ 
  
/** @defgroup STM32F3348_DISCOVERY_BUTTON STM32F3348-DISCOVERY BUTTON
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

/**
  * @}
  */ 

/** @defgroup STM32F3348_DISCOVERY_Exported_Functions Exported Functions
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

#endif /* __STM32F3348_DISCOVERY_H */

