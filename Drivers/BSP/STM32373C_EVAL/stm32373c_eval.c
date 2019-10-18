/**
  ******************************************************************************
  * @file    stm32373c_eval.c
  * @author  MCD Application Team
  * @brief   This file provides a set of firmware functions to manage LEDs, 
  *          push-button and COM port available on STM32373C-EVAL evaluation
  *          board (MB988) RevB from STMicroelectronics.
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
  
/* Includes ------------------------------------------------------------------*/
#include "stm32373c_eval.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32373C_EVAL
 * @brief This file provides firmware functions to manage Leds, push-buttons,  
 *        COM ports, SD card and temperature sensor available on 
 *        STM32373C-EVAL evaluation board from STMicroelectronics.
  * @{
  */   
    
/** @addtogroup STM32373C_EVAL_Common
  * @{
  */ 

/** @addtogroup STM32373C_EVAL_Private_Constants
  * @{
  */ 
/* LINK LCD */
#define START_BYTE         0x70
#define SET_INDEX          0x00
#define READ_STATUS        0x01
#define LCD_WRITE_REG      0x02
#define LCD_READ_REG       0x03

/* LINK SD Card */
#define SD_DUMMY_BYTE           0xFF
#define SD_NO_RESPONSE_EXPECTED 0x80

/**
 * @brief STM32373C EVAL BSP Driver version number V2.2.2
   */
#define __STM32373C_EVAL_BSP_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __STM32373C_EVAL_BSP_VERSION_SUB1   (0x02) /*!< [23:16] sub1 version */
#define __STM32373C_EVAL_BSP_VERSION_SUB2   (0x02) /*!< [15:8]  sub2 version */
#define __STM32373C_EVAL_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32373C_EVAL_BSP_VERSION         ((__STM32373C_EVAL_BSP_VERSION_MAIN << 24)\
                                             |(__STM32373C_EVAL_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32373C_EVAL_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32373C_EVAL_BSP_VERSION_RC))
/**
  * @}
  */ 

/** @addtogroup STM32373C_EVAL_Private_Variables
  * @{
  */ 
/**
 * @brief LED variables
 */
GPIO_TypeDef* LED_PORT[LEDn] = {LED1_GPIO_PORT,
                                LED2_GPIO_PORT,
                                LED3_GPIO_PORT,
                                LED4_GPIO_PORT};

const uint16_t LED_PIN[LEDn] = {LED1_PIN,
                                LED2_PIN,
                                LED3_PIN,
                                LED4_PIN};

/**
 * @brief BUTTON variables
 */
GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {TAMPER_BUTTON_GPIO_PORT,
                                      KEY_BUTTON_GPIO_PORT,
                                      SEL_JOY_GPIO_PORT,
                                      LEFT_JOY_GPIO_PORT,
                                      RIGHT_JOY_GPIO_PORT,
                                      DOWN_JOY_GPIO_PORT,
                                      UP_JOY_GPIO_PORT}; 

const uint16_t BUTTON_PIN[BUTTONn] = {TAMPER_BUTTON_PIN,
                                      KEY_BUTTON_PIN,
                                      SEL_JOY_PIN,
                                      LEFT_JOY_PIN,
                                      RIGHT_JOY_PIN,
                                      DOWN_JOY_PIN,
                                      UP_JOY_PIN};

const uint16_t BUTTON_IRQn[BUTTONn] = {TAMPER_BUTTON_EXTI_IRQn,
                                       KEY_BUTTON_EXTI_IRQn,
                                       SEL_JOY_EXTI_IRQn,
                                       LEFT_JOY_EXTI_IRQn,
                                       RIGHT_JOY_EXTI_IRQn,
                                       DOWN_JOY_EXTI_IRQn,
                                       UP_JOY_EXTI_IRQn};

/**
 * @brief JOYSTICK variables
 */
GPIO_TypeDef* JOY_PORT[JOYn] = {SEL_JOY_GPIO_PORT,
                                LEFT_JOY_GPIO_PORT, 
                                RIGHT_JOY_GPIO_PORT, 
                                DOWN_JOY_GPIO_PORT, 
                                UP_JOY_GPIO_PORT}; 

const uint16_t JOY_PIN[JOYn] = {SEL_JOY_PIN, 
                                LEFT_JOY_PIN, 
                                RIGHT_JOY_PIN, 
                                DOWN_JOY_PIN, 
                                UP_JOY_PIN}; 

const uint8_t JOY_IRQn[JOYn] = {SEL_JOY_EXTI_IRQn,
                                LEFT_JOY_EXTI_IRQn, 
                                RIGHT_JOY_EXTI_IRQn, 
                                DOWN_JOY_EXTI_IRQn, 
                                UP_JOY_EXTI_IRQn};

/**
 * @brief COM variables
 */
#if defined(HAL_UART_MODULE_ENABLED)
USART_TypeDef* COM_USART[COMn]   = {EVAL_COM1}; 

GPIO_TypeDef*  COM_TX_PORT[COMn] = {EVAL_COM1_TX_GPIO_PORT};
 
GPIO_TypeDef*  COM_RX_PORT[COMn] = {EVAL_COM1_RX_GPIO_PORT};

const uint16_t COM_TX_PIN[COMn]  = {EVAL_COM1_TX_PIN};

const uint16_t COM_RX_PIN[COMn]  = {EVAL_COM1_RX_PIN};
 
const uint16_t COM_TX_AF[COMn]   = {EVAL_COM1_TX_AF};

const uint16_t COM_RX_AF[COMn]   = {EVAL_COM1_RX_AF};
#endif /* HAL_UART_MODULE_ENABLED) */

/**
 * @brief BUS variables
 */
#if defined(HAL_I2C_MODULE_ENABLED)
uint32_t I2c1Timeout = EVAL_I2C1_TIMEOUT_MAX;        /*<! Value of Timeout when I2C1 communication fails */
uint32_t I2c2Timeout = EVAL_I2C2_TIMEOUT_MAX;        /*<! Value of Timeout when I2C2 communication fails */
I2C_HandleTypeDef heval_I2c1;
I2C_HandleTypeDef heval_I2c2;
#endif /* HAL_I2C_MODULE_ENABLED */

#if defined(HAL_SPI_MODULE_ENABLED)
uint32_t SpixTimeout = EVAL_SPIx_TIMEOUT_MAX;        /*<! Value of Timeout when SPI communication fails */
static SPI_HandleTypeDef heval_Spi;
#endif /* HAL_SPI_MODULE_ENABLED */

/**
  * @}
  */ 

/** @defgroup STM32373C_EVAL_BUS Bus Operation functions
  * @{
  */
#if defined(HAL_I2C_MODULE_ENABLED)
/* I2Cx bus function */
static void              I2C1_Init(void);
static HAL_StatusTypeDef I2C1_TransmitData(uint8_t *pBuffer, uint16_t Length);
static void              I2C1_Error(void);
static void              I2C1_MspInit(I2C_HandleTypeDef *hi2c);

static void              I2C2_Init(void);
static void              I2C2_WriteData(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t Value);
static HAL_StatusTypeDef I2C2_WriteBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize,  uint8_t *pBuffer, uint16_t Length);
static uint8_t           I2C2_ReadData(uint16_t Addr, uint8_t Reg, uint16_t RegSize);
static HAL_StatusTypeDef I2C2_ReadBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length);
static HAL_StatusTypeDef I2C2_ReceiveData(uint16_t Addr, uint8_t * pBuffer, uint16_t Length);
static HAL_StatusTypeDef I2C2_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);
static void              I2C2_Error(void);
static void              I2C2_MspInit(I2C_HandleTypeDef *hi2c);

/* Link functions for EEPROM peripheral */
void                     EEPROM_IO_Init(void);
HAL_StatusTypeDef        EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef        EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef        EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

/* Link functions for Temperature Sensor peripheral */
void                     TSENSOR_IO_Init(void);
void                     TSENSOR_IO_Write(uint16_t DevAddress, uint8_t* pBuffer, uint8_t WriteAddr, uint16_t Length);
void                     TSENSOR_IO_Read(uint16_t DevAddress, uint8_t* pBuffer, uint8_t ReadAddr, uint16_t Length);
uint16_t                 TSENSOR_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

/* Link functions for Audio Codec peripheral */
void                     AUDIO_IO_Init(void);
void                     AUDIO_IO_DeInit(void);
void                     AUDIO_IO_Write(uint16_t DevAddress, uint8_t Reg, uint8_t Value);
uint8_t                  AUDIO_IO_Read(uint16_t DevAddress, uint8_t Reg);

/* Link functions for CEC peripheral */
void                     HDMI_CEC_IO_Init(void);
HAL_StatusTypeDef        HDMI_CEC_IO_WriteData(uint8_t * pBuffer, uint16_t BufferSize);
HAL_StatusTypeDef        HDMI_CEC_IO_ReadData(uint16_t DevAddress, uint8_t * pBuffer, uint16_t BufferSize);
#endif /* HAL_I2C_MODULE_ENABLED */

#if defined(HAL_SPI_MODULE_ENABLED)
/* SPIx bus function */
static void              SPIx_Init(void);
static void              SPIx_Write(uint8_t Value);
static uint32_t          SPIx_Read(void);
static void              SPIx_Error (void);
static void              SPIx_MspInit(SPI_HandleTypeDef *hspi);

/* Link functions for LCD peripheral over SPI */
void                     LCD_IO_Init(void);
void                     LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);
void                     LCD_IO_WriteReg(uint8_t Reg);
uint16_t                 LCD_IO_ReadData(uint16_t Reg);
void                     LCD_Delay(uint32_t delay);

/* Link functions for SD Card peripheral over SPI */
void                     SD_IO_Init(void);
HAL_StatusTypeDef        SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response);
HAL_StatusTypeDef        SD_IO_WaitResponse(uint8_t Response);
void                     SD_IO_WriteDummy(void);
void                     SD_IO_WriteByte(uint8_t Data);
uint8_t                  SD_IO_ReadByte(void);
#endif /* HAL_SPI_MODULE_ENABLED */

/**
  * @}
  */ 

/** @addtogroup STM32373C_EVAL_Exported_Functions
  * @{
  */ 

/**
  * @brief  This method returns the STM32373C EVAL BSP Driver revision
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32373C_EVAL_BSP_VERSION;
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval None
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED clock */
  LEDx_GPIO_CLK_ENABLE(Led);

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = LED_PIN[Led];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init(LED_PORT[Led], &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4  
  * @retval None
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4 
  * @retval None
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4  
  * @retval None
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}

/**
  * @brief  Configures push button GPIO and EXTI Line.
  * @param  Button Button to be configured
  *   This parameter can be one of the following values:
  *     @arg BUTTON_TAMPER: Tamper Push Button 
  *     @arg BUTTON_KEY   : Key Push Button 
  * @param  Mode Button mode
  *   This parameter can be one of the following values:
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line
  *                            with interrupt generation capability
  * @retval None
  */
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Mode)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable the corresponding Push Button clock */
  BUTTONx_GPIO_CLK_ENABLE(Button);

  /* Configure Button pin as input */
  GPIO_InitStruct.Pin = BUTTON_PIN[Button];
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  if (Mode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
  }

  if (Mode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    if(Button != BUTTON_KEY) 
    {
      GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    }
    else
    {
      GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    }
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
}

/**
  * @brief  Returns the selected button state.
  * @param  Button Button to be checked.
  *   This parameter can be one of the following values:
  *     @arg BUTTON_TAMPER: Tamper Push Button
  *     @arg BUTTON_KEY: Key Push Button 
  * @retval The Button GPIO pin value
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  Configures all button of the joystick in GPIO or EXTI modes.
  * @param  Joy_Mode Joystick mode.
  *    This parameter can be one of the following values:
  *      @arg  JOY_MODE_GPIO: Joystick pins will be used as simple IOs
  *      @arg  JOY_MODE_EXTI: Joystick pins will be connected to EXTI line 
  *                                 with interrupt generation capability  
  * @retval HAL_OK: if all initializations are OK. Other value if error.
  */
uint8_t BSP_JOY_Init(JOYMode_TypeDef Joy_Mode)
{
  JOYState_TypeDef joykey;
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Initialized the Joystick. */
  for(joykey = JOY_SEL; joykey < (JOY_SEL+JOYn) ; joykey++)
  {
    /* Enable the JOY clock */
    JOYx_GPIO_CLK_ENABLE(joykey);

    GPIO_InitStruct.Pin = JOY_PIN[joykey];
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    if (Joy_Mode == JOY_MODE_GPIO)
    {
      /* Configure Joy pin as input */
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      HAL_GPIO_Init(JOY_PORT[joykey], &GPIO_InitStruct);
    }

    if (Joy_Mode == JOY_MODE_EXTI)
    {
      /* Configure Joy pin as input with External interrupt */
      GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
      HAL_GPIO_Init(JOY_PORT[joykey], &GPIO_InitStruct);

      /* Enable and set Joy EXTI Interrupt to the lowest priority */
      HAL_NVIC_SetPriority((IRQn_Type)(JOY_IRQn[joykey]), 0x0F, 0x00);
      HAL_NVIC_EnableIRQ((IRQn_Type)(JOY_IRQn[joykey]));
    }
  }
  
  return HAL_OK;
}

/**
  * @brief  Returns the current joystick status.
  * @retval Code of the joystick key pressed
  *          This code can be one of the following values:
  *            @arg  JOY_NONE
  *            @arg  JOY_SEL
  *            @arg  JOY_DOWN
  *            @arg  JOY_LEFT
  *            @arg  JOY_RIGHT
  *            @arg  JOY_UP
  *            @arg  JOY_NONE
  */
JOYState_TypeDef BSP_JOY_GetState(void)
{
  JOYState_TypeDef joykey;
	
  for(joykey = JOY_SEL; joykey < (JOY_SEL+JOYn) ; joykey++)
  {
    if(HAL_GPIO_ReadPin(JOY_PORT[joykey], JOY_PIN[joykey]) == GPIO_PIN_SET)
    {
      /* Return Code Joystick key pressed */
      return joykey;
    }
  }
  
  /* No Joystick key pressed */
  return JOY_NONE;
}

#if defined(HAL_UART_MODULE_ENABLED)
/**
  * @brief  Configures COM port.
  * @param  COM Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1  
  * @param  huart pointer to a UART_HandleTypeDef structure that
  *   contains the configuration information for the specified UART peripheral.
  * @retval None
  */
void BSP_COM_Init(COM_TypeDef COM, UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIO clock */
  COMx_TX_GPIO_CLK_ENABLE(COM);
  COMx_RX_GPIO_CLK_ENABLE(COM);

  /* Enable USART clock */
  COMx_CLK_ENABLE(COM);

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStruct.Pin = COM_TX_PIN[COM];
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = COM_TX_AF[COM];
  HAL_GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStruct);
    
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStruct.Pin = COM_RX_PIN[COM];
  GPIO_InitStruct.Alternate = COM_RX_AF[COM];
  HAL_GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStruct);

  /* USART configuration */
  huart->Instance = COM_USART[COM];
  HAL_UART_Init(huart);
}
#endif /* HAL_UART_MODULE_ENABLED) */
/**
  * @}
  */ 

/** @addtogroup STM32373C_EVAL_BUS
  * @{
  */
/*******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/
#if defined(HAL_I2C_MODULE_ENABLED)
/******************************* I2C Routines**********************************/
/**
  * @brief Initializes I2C1 MSP
  * @param hi2c I2C handle
  * @retval None
  */
static void I2C1_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  if (hi2c->Instance == EVAL_I2C1)
  {
    /*##-1- Set source clock to SYSCLK for I2C1 ################################################*/  
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    RCC_PeriphCLKInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

    /*##-2- Configure the GPIOs ################################################*/  

    /* Enable GPIO clock */
    EVAL_I2C1_GPIO_CLK_ENABLE();
      
    /* Configure I2C SCL & SDA as alternate function  */
    GPIO_InitStruct.Pin = (EVAL_I2C1_SCL_PIN | EVAL_I2C1_SDA_PIN);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = EVAL_I2C1_AF;
    HAL_GPIO_Init(EVAL_I2C1_GPIO_PORT, &GPIO_InitStruct);

    /*##-3- Configure the Eval I2C peripheral #######################################*/ 
    /* Enable I2C clock */
    EVAL_I2C1_CLK_ENABLE();

    /* Force the I2C peripheral clock reset */
    EVAL_I2C1_FORCE_RESET();

    /* Release the I2C peripheral clock reset */
    EVAL_I2C1_RELEASE_RESET();
  }
}

/**
  * @brief I2C Bus initialization
  * @retval None
  */
static void I2C1_Init(void)
{
  if(HAL_I2C_GetState(&heval_I2c1) == HAL_I2C_STATE_RESET)
  {
    heval_I2c1.Instance              = EVAL_I2C1;
    heval_I2c1.Init.Timing           = I2C1_TIMING;
    heval_I2c1.Init.OwnAddress1      = HDMI_CEC_I2C_ADDRESS;
    heval_I2c1.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    heval_I2c1.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    heval_I2c1.Init.OwnAddress2      = 0;
    heval_I2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    heval_I2c1.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    heval_I2c1.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;  
      
    /* Init the I2C */
    I2C1_MspInit(&heval_I2c1);
    HAL_I2C_Init(&heval_I2c1);
  }
}

/**
  * @brief  Write buffer through I2C.
  * @param  pBuffer The address of the data to be written 
  * @param  Length buffer size to be written
  * @retval None
  */
static HAL_StatusTypeDef I2C1_TransmitData(uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Slave_Transmit(&heval_I2c1, pBuffer, Length, I2c1Timeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2C1_Error();
    return HAL_ERROR;
  }        
  return HAL_OK;
}

/**
  * @brief  Manages error callback by re-initializing I2C.
  * @retval None
  */
static void I2C1_Error(void)
{
  /* De-initialize the I2C communication BUS */
  HAL_I2C_DeInit(&heval_I2c1);
  
  /* Re-Initiaize the I2C communication BUS */
  I2C1_Init();
}

/**
  * @brief Initializes I2C2 MSP.
  * @param hi2c I2C handle
  * @retval None
  */
static void I2C2_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
  RCC_PeriphCLKInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_SYSCLK;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

  /* Enable GPIO clock */
  EVAL_I2C2_GPIO_CLK_ENABLE();
      
  /* Configure I2C SCL, SDA and SMBUS as alternate function  */
  GPIO_InitStruct.Pin = (EVAL_I2C2_SCL_PIN | EVAL_I2C2_SDA_PIN | EVAL_I2C2_SMBUS_PIN);
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = EVAL_I2C2_AF;
  HAL_GPIO_Init(EVAL_I2C2_GPIO_PORT, &GPIO_InitStruct);

  /* Enable I2C clock */
  EVAL_I2C2_CLK_ENABLE();

  /* Force the I2C peripheral clock reset */
  EVAL_I2C2_FORCE_RESET();

  /* Release the I2C peripheral clock reset */
  EVAL_I2C2_RELEASE_RESET();
}

/**
  * @brief I2C Bus initialization
  * @retval None
  */
static void I2C2_Init(void)
{
  if(HAL_I2C_GetState(&heval_I2c2) == HAL_I2C_STATE_RESET)
  {
    heval_I2c2.Instance              = EVAL_I2C2;
    heval_I2c2.Init.Timing           = I2C2_TIMING;
    heval_I2c2.Init.OwnAddress1      = 0;
    heval_I2c2.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    heval_I2c2.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    heval_I2c2.Init.OwnAddress2      = 0;
    heval_I2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    heval_I2c2.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    heval_I2c2.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;  
      
    /* Init the I2C */
    I2C2_MspInit(&heval_I2c2);
    HAL_I2C_Init(&heval_I2c2);
  }
}

/**
  * @brief  Write a value in a register of the device through I2C.
  * @param  Addr Device address on I2C Bus.  
  * @param  Reg The target register address to write
  * @param  RegSize The target register size (can be 8BIT or 16BIT)
  * @param  Value The target register value to be written 
  * @retval None
  */
static void I2C2_WriteData(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t Value)
  {
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&heval_I2c2, Addr, (uint16_t)Reg, RegSize, &Value, 1, I2c2Timeout); 

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2C2_Error();
  }        
}

/**
  * @brief  Write buffer through I2C.
  * @param  Addr Device address on I2C Bus.  
  * @param  Reg The target register address to write
  * @param  RegSize The target register size (can be 8BIT or 16BIT)
  * @param  pBuffer The address of the data to be written 
  * @param  Length buffer size to be written
  * @retval HAL Status
  */
static HAL_StatusTypeDef I2C2_WriteBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize,  uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&heval_I2c2, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2c2Timeout); 

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2C2_Error();
  }        
  return status;
}

/**
  * @brief  Read a register of the device through I2C.
  * @param  Addr Device address on I2C Bus.  
  * @param  pBuffer The address to store the read data 
  * @param  Length buffer size to be read
  * @retval None
  */
static HAL_StatusTypeDef I2C2_ReceiveData(uint16_t Addr, uint8_t * pBuffer, uint16_t Length)
  {
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Master_Receive(&heval_I2c2, Addr, pBuffer, Length, I2c2Timeout); 

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2C2_Error();
  }        
  return status;
}

/**
  * @brief  Read a register of the device through I2C
  * @param  Addr Device address on I2C.  
  * @param  Reg The target register address to read
  * @param  RegSize The target register size (can be 8BIT or 16BIT)
  * @retval Read register value
  */
static uint8_t I2C2_ReadData(uint16_t Addr, uint8_t Reg, uint16_t RegSize)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t value = 0;
  
  status = HAL_I2C_Mem_Read(&heval_I2c2, Addr, Reg, RegSize, &value, 1, I2c2Timeout);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2C2_Error();
  }
  return value;
}

/**
  * @brief  Reads buffer through I2C.
  * @param  Addr Device address on I2C
  * @param  Reg The target address to read
  * @param  RegSize The target register size (can be 8BIT or 16BIT)
  * @param  pBuffer The address to store the read data
  * @param  Length buffer size to be read
  * @retval None   
  */
static HAL_StatusTypeDef I2C2_ReadBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Read(&heval_I2c2, Addr, (uint16_t)Reg, RegSize, pBuffer, Length, I2c2Timeout);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2C2_Error();
  }
  return status;
}

/**
* @brief  Checks if target device is ready for communication. 
* @note   This function is used with Memory devices
* @param  DevAddress Target device address
* @param  Trials Number of trials
* @retval HAL status
*/
static HAL_StatusTypeDef I2C2_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{ 
  return (HAL_I2C_IsDeviceReady(&heval_I2c2, DevAddress, Trials, I2c2Timeout));
}

/**
  * @brief  Manages error callback by re-initializing I2C.
 * @retval None
  */
static void I2C2_Error(void)
{
  /* De-initialize the I2C communication BUS */
  HAL_I2C_DeInit(&heval_I2c2);
  
  /* Re-Initiaize the I2C communication BUS */
  I2C2_Init();
}

#endif /*HAL_I2C_MODULE_ENABLED*/

#if defined(HAL_SPI_MODULE_ENABLED)
/******************************* SPI Routines *********************************/
/**
  * @brief Initializes SPI MSP.
  * @param hspi SPI handle
  * @retval None
  */
static void SPIx_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable SPI clock  */
  EVAL_SPIx_CLK_ENABLE();

  /* enable SPI gpio clock */
  EVAL_SPIx_GPIO_CLK_ENABLE();

  /* configure SPI SCK, MOSI and MISO */
  GPIO_InitStruct.Pin = (EVAL_SPIx_SCK_PIN | EVAL_SPIx_MOSI_PIN | EVAL_SPIx_MISO_PIN);
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = EVAL_SPIx_AF;
  HAL_GPIO_Init(EVAL_SPIx_GPIO_PORT, &GPIO_InitStruct);

  /* Force the SPI peripheral clock reset */
  EVAL_SPIx_FORCE_RESET();

  /* Release the SPI peripheral clock reset */
  EVAL_SPIx_RELEASE_RESET();
}

/**
  * @brief  Initializes SPI HAL.
  * @retval None
  */
static void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&heval_Spi) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    heval_Spi.Instance = EVAL_SPIx;
    /* SPI baudrate is set to 18 MHz (PCLK2/SPI_BaudRatePrescaler = 72/4 = 18 MHz) */
    heval_Spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    heval_Spi.Init.Direction = SPI_DIRECTION_2LINES;
    heval_Spi.Init.CLKPhase = SPI_PHASE_2EDGE;
    heval_Spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
    heval_Spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    heval_Spi.Init.CRCPolynomial = 7;
    heval_Spi.Init.DataSize = SPI_DATASIZE_8BIT;
    heval_Spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    heval_Spi.Init.NSS = SPI_NSS_SOFT;
    heval_Spi.Init.TIMode = SPI_TIMODE_DISABLE;
    heval_Spi.Init.Mode = SPI_MODE_MASTER;
    
    SPIx_MspInit(&heval_Spi);
    HAL_SPI_Init(&heval_Spi);
  }
}

/**
  * @brief SPI Read 4 bytes from device
  * @retval Value read on the SPI
  */
static uint32_t SPIx_Read(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t readvalue = 0x0;
  uint32_t writevalue = 0xFFFFFFFF;
  
  status = HAL_SPI_TransmitReceive(&heval_Spi, (uint8_t*) &writevalue, (uint8_t*) &readvalue, 1, SpixTimeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }

  return readvalue;
}

/**
  * @brief SPI Write a byte to device
  * @param Value value to be written
  * @retval None
  */
static void SPIx_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&heval_Spi, (uint8_t*) &Value, 1, SpixTimeout);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/**
  * @brief SPI error treatment function
  * @retval None
  */
static void SPIx_Error (void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&heval_Spi);
  
  /* Re- Initiaize the SPI communication BUS */
  SPIx_Init();
}

#endif /*HAL_SPI_MODULE_ENABLED*/

/******************************************************************************
LINK OPERATIONS
*******************************************************************************/
/**
  * @}
  */ 

/** @defgroup STM32373C_EVAL_LINK_OPERATIONS Link Operation functions
  * @{
  */

#if defined(HAL_SPI_MODULE_ENABLED)
/********************************* LINK LCD ***********************************/

/**
  * @brief  Configures the LCD_SPI interface.
  * @retval None
  */
void LCD_IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Configure the LCD Control pins ------------------------------------------*/
  LCD_NCS_GPIO_CLK_ENABLE();
    
  /* Configure NCS in Output Push-Pull mode */
  GPIO_InitStruct.Pin     = LCD_NCS_PIN;
  GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull    = GPIO_NOPULL;
  GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_NCS_GPIO_PORT, &GPIO_InitStruct);
    
  /* Set or Reset the control line */
  LCD_CS_LOW();
  LCD_CS_HIGH();

  SPIx_Init();
}

/**
  * @brief  Write register value.
  * @param  pData Pointer on the register value
  * @param  Size Size of byte to transmit to the register
  * @retval None
  */
void LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size)
{
  uint32_t counter = 0;

  /* Reset LCD control line(/CS) and Send data */  
  LCD_CS_LOW();

  /* Send Start Byte */
  SPIx_Write(START_BYTE | LCD_WRITE_REG);
  
  for (counter = Size; counter != 0; counter--)
  {
    while(((heval_Spi.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
    {
    }  
    /* Need to invert bytes for LCD*/
    *((__IO uint8_t*)&heval_Spi.Instance->DR) = *(pData+1);
  
    while(((heval_Spi.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
    {
    }  
    *((__IO uint8_t*)&heval_Spi.Instance->DR) = *pData;
    counter--;
    pData += 2;
  }
  
  /* Wait until the bus is ready before releasing Chip select */ 
  while(((heval_Spi.Instance->SR) & SPI_FLAG_BSY) != RESET)
  {
  }  

  /* Reset LCD control line(/CS) and Send data */  
  LCD_CS_HIGH();
}

/**
  * @brief  register address.
  * @param  Reg
  * @retval None
  */
void LCD_IO_WriteReg(uint8_t Reg) 
{
  /* Reset LCD control line(/CS) and Send command */
  LCD_CS_LOW();
  
  /* Send Start Byte */
  SPIx_Write(START_BYTE | SET_INDEX);
  
  /* Write 16-bit Reg Index (High Byte is 0) */
  SPIx_Write(0x00);
  SPIx_Write(Reg);
  
  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
}

/**
  * @brief  Read register value.
  * @retval None
  */
uint16_t LCD_IO_ReadData(uint16_t Reg) 
{
  uint32_t readvalue = 0;

  /* Send Reg value to Read */
  LCD_IO_WriteReg(Reg);

  /* Reset LCD control line(/CS) and Send command */
  LCD_CS_LOW();
  
  /* Send Start Byte */
  SPIx_Write(START_BYTE | LCD_READ_REG);

  /* Read Upper Byte */
  SPIx_Write(0xFF);
  readvalue = SPIx_Read();
  readvalue = readvalue << 8;
  readvalue |= SPIx_Read();
  
  HAL_Delay(10);

  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
  return readvalue;
}

/**
  * @brief  Wait for loop in ms.
  * @param  Delay in ms.
  * @retval None
  */
void LCD_Delay (uint32_t Delay)
{
  HAL_Delay(Delay);
}

/******************************** LINK SD Card ********************************/

/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for 
  *         data transfer).
  * @retval None
  */
void SD_IO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  uint8_t counter;

  /* SD_CS_GPIO and SD_DETECT_GPIO Periph clock enable */
  SD_CS_GPIO_CLK_ENABLE();
  SD_DETECT_GPIO_CLK_ENABLE();

  /* Configure SD_CS_PIN pin: SD Card CS pin */
  GPIO_InitStruct.Pin = SD_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SD_CS_GPIO_PORT, &GPIO_InitStruct);

  /* Configure SD_DETECT_PIN pin: SD Card detect pin */
  GPIO_InitStruct.Pin = SD_DETECT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_InitStruct);

  /* Enable and set SD EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(SD_DETECT_EXTI_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(SD_DETECT_EXTI_IRQn);

  /*------------Put SD in SPI mode--------------*/
  /* SD SPI Config */
  SPIx_Init();

  /* SD chip select high */
  SD_CS_HIGH();
  
  /* Send dummy byte 0xFF, 10 times with CS high */
  /* Rise CS and MOSI for 80 clocks cycles */
  for (counter = 0; counter <= 9; counter++)
  {
    /* Send dummy byte 0xFF */
    SD_IO_WriteByte(SD_DUMMY_BYTE);
  }
}

/**
  * @brief  Write a byte on the SD.
  * @param  Data byte to send.
  * @retval None
  */
void SD_IO_WriteByte(uint8_t Data)
{
  /* Send the byte */
  SPIx_Write(Data);
}

/**
  * @brief  Read a byte from the SD.
  * @retval The received byte.
  */
uint8_t SD_IO_ReadByte(void)
{
  uint8_t data = 0;
  
  /* Get the received data */
  data = SPIx_Read();

  /* Return the shifted data */
  return data;
}

/**
  * @brief  Send 5 bytes command to the SD card and get response
  * @param  Cmd The user expected command to send to SD card.
  * @param  Arg The command argument.
  * @param  Crc The CRC.
  * @param  Response Expected response from the SD card
  * @retval  HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef SD_IO_WriteCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc, uint8_t Response)
{
  uint32_t counter = 0x00;
  uint8_t frame[6];

  /* Prepare Frame to send */
  frame[0] = (Cmd | 0x40); /* Construct byte 1 */
  frame[1] = (uint8_t)(Arg >> 24); /* Construct byte 2 */
  frame[2] = (uint8_t)(Arg >> 16); /* Construct byte 3 */
  frame[3] = (uint8_t)(Arg >> 8); /* Construct byte 4 */
  frame[4] = (uint8_t)(Arg); /* Construct byte 5 */
  frame[5] = (Crc); /* Construct CRC: byte 6 */
  
  /* SD chip select low */
  SD_CS_LOW();
    
  /* Send Frame */
  for (counter = 0; counter < 6; counter++)
  {
    SD_IO_WriteByte(frame[counter]); /* Send the Cmd bytes */
  }

  if(Response != SD_NO_RESPONSE_EXPECTED)
  {
    return SD_IO_WaitResponse(Response);
  }

  return HAL_OK;
}

/**
  * @brief  Wait response from the SD card
  * @param  Response Expected response from the SD card
  * @retval  HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef SD_IO_WaitResponse(uint8_t Response)
{
  uint32_t timeout = 0xFFF;

  /* Check if response is got or a timeout is happen */
  while ((SD_IO_ReadByte() != Response) && timeout)
  {
    timeout--;
  }

  if (timeout == 0)
  {
    /* After time out */
    return HAL_TIMEOUT;
  }
  else
  {
    /* Right response got */
    return HAL_OK;
  }
}

/**
  * @brief  Send dummy byte with CS High
  * @retval None
  */
void SD_IO_WriteDummy(void)
{
  /* SD chip select high */
  SD_CS_HIGH();
    
  /* Send Dummy byte 0xFF */
  SD_IO_WriteByte(SD_DUMMY_BYTE);
}

#endif /* HAL_SPI_MODULE_ENABLED */

#if defined(HAL_I2C_MODULE_ENABLED)
/********************************* LINK I2C EEPROM *****************************/
/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @retval None
  */
void EEPROM_IO_Init(void)
{
  I2C2_Init();
}

/**
  * @brief  Write data to I2C EEPROM driver
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  pBuffer Pointer to data buffer
  * @param  BufferSize Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
  return (I2C2_WriteBuffer(DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pBuffer, BufferSize));
}

/**
  * @brief  Read data from I2C EEPROM driver
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  pBuffer Pointer to data buffer
  * @param  BufferSize Amount of data to be read
  * @retval HAL status
  */
HAL_StatusTypeDef EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
  return (I2C2_ReadBuffer(DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pBuffer, BufferSize));
}

/**
* @brief  Checks if target device is ready for communication. 
* @note   This function is used with Memory devices
* @param  DevAddress Target device address
* @param  Trials Number of trials
* @retval HAL status
*/
HAL_StatusTypeDef EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{ 
  return (I2C2_IsDeviceReady(DevAddress, Trials));
}

/********************************* LINK I2C TEMPERATURE SENSOR *****************************/
/**
  * @brief  Initializes peripherals used by the I2C Temperature Sensor driver.
  * @retval None
  */
void TSENSOR_IO_Init(void)
{
  I2C2_Init();
}

/**
  * @brief  Writes one byte to the TSENSOR.
  * @param  DevAddress Target device address
  * @param  pBuffer Pointer to data buffer
  * @param  WriteAddr TSENSOR's internal address to write to.
  * @param  Length Number of data to write
  * @retval None
  */
void TSENSOR_IO_Write(uint16_t DevAddress, uint8_t* pBuffer, uint8_t WriteAddr, uint16_t Length)
{
  I2C2_WriteBuffer(DevAddress, WriteAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, Length);
}

/**
  * @brief  Reads one byte from the TSENSOR.
  * @param  DevAddress Target device address
  * @param  pBuffer pointer to the buffer that receives the data read from the TSENSOR.
  * @param  ReadAddr TSENSOR's internal address to read from.
  * @param  Length Number of data to read
  * @retval None
  */
void TSENSOR_IO_Read(uint16_t DevAddress, uint8_t* pBuffer, uint8_t ReadAddr, uint16_t Length)
{
  I2C2_ReadBuffer(DevAddress, ReadAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, Length);
}

/**
* @brief  Checks if Temperature Sensor is ready for communication. 
  * @param  DevAddress Target device address
  * @param  Trials Number of trials
* @retval HAL status
*/
uint16_t TSENSOR_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{ 
  return (I2C2_IsDeviceReady(DevAddress, Trials));
}


/********************************* LINK AUDIO CODEC ***********************************/
/**
  * @brief  Initializes peripherals used by the Audio Codec driver.
  * @retval None
  */
void AUDIO_IO_Init(void) 
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable Reset GPIO Clock */
  AUDIO_RESET_GPIO_CLK_ENABLE();

  /* Audio reset pin configuration -------------------------------------------------*/
  GPIO_InitStruct.Pin = AUDIO_RESET_PIN; 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(AUDIO_RESET_GPIO_PORT, &GPIO_InitStruct);    

  I2C2_Init();

  /* Power Down the codec */
  CODEC_AUDIO_POWER_OFF();

  /* wait for a delay to insure registers erasing */
  HAL_Delay(5); 

  /* Power on the codec */
  CODEC_AUDIO_POWER_ON();
   
  /* wait for a delay to insure registers erasing */
  HAL_Delay(5); 
}

/**
  * @brief  DeInitializes Audio low level.
  * @note   This function is intentionally kept empty, user should define it.
  */
void AUDIO_IO_DeInit(void)
{

}

/**
  * @brief  Writes a single data on the Audio Codec.
  * @param  DevAddress Target device address
  * @param  Reg Target Register address 
  * @param  Value Data to be written
  * @retval None
  */
void AUDIO_IO_Write(uint16_t DevAddress, uint8_t Reg, uint8_t Value)
{
  I2C2_WriteData(DevAddress, Reg, I2C_MEMADD_SIZE_8BIT, Value);
}

/**
  * @brief  Reads a single data from the Audio Codec.
  * @param  DevAddress Target device address
  * @param  Reg Target Register address 
  * @retval Data to be read
  */
uint8_t AUDIO_IO_Read(uint16_t DevAddress, uint8_t Reg)
{
  uint8_t value;
  
  value = I2C2_ReadData(DevAddress, Reg, I2C_MEMADD_SIZE_8BIT);
  
  return value;
}

/****************************** LINK HDMI CEC *********************************/
/**
  * @brief  Initializes CEC low level.
  * @retval None
  */
void HDMI_CEC_IO_Init (void) 
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Enable CEC clock */
  __HAL_RCC_CEC_CLK_ENABLE();
  
  /* Enable CEC LINE GPIO clock */
  HDMI_CEC_LINE_CLK_ENABLE();

  /* Configure CEC LINE GPIO as alternate function open drain */
  GPIO_InitStruct.Pin = HDMI_CEC_LINE_PIN; 
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = HDMI_CEC_LINE_AF;
  HAL_GPIO_Init(HDMI_CEC_LINE_GPIO_PORT, &GPIO_InitStruct);    

  /* CEC IRQ Channel configuration */
  HAL_NVIC_SetPriority((IRQn_Type)HDMI_CEC_IRQn, 0xF, 0x0);
  HAL_NVIC_EnableIRQ((IRQn_Type)HDMI_CEC_IRQn); 

  /* Enable CEC HPD SINK GPIO clock */
  HDMI_CEC_HPD_SINK_CLK_ENABLE();
  
  /* Configure CEC HPD SINK GPIO as output push pull */
  GPIO_InitStruct.Pin = HDMI_CEC_HPD_SINK_PIN; 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
  HAL_GPIO_Init(HDMI_CEC_HPD_SINK_GPIO_PORT, &GPIO_InitStruct); 
  
  I2C1_Init();

  /* Enable CEC HPD SOURCE GPIO clock */
  HDMI_CEC_HPD_SOURCE_CLK_ENABLE();

  /* Configure CEC HPD SOURCE GPIO as output push pull */
  GPIO_InitStruct.Pin = HDMI_CEC_HPD_SOURCE_PIN; 
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
  HAL_GPIO_Init(HDMI_CEC_HPD_SOURCE_GPIO_PORT, &GPIO_InitStruct); 
  
  I2C2_Init();
}

/**
  * @brief  Write data to I2C HDMI CEC driver
  * @param  pBuffer Pointer to data buffer
  * @param  BufferSize Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HDMI_CEC_IO_WriteData(uint8_t * pBuffer, uint16_t BufferSize)
{
  return (I2C1_TransmitData(pBuffer, BufferSize));
}

/**
  * @brief  Read data to I2C HDMI CEC driver
  * @param  DevAddress Target device address
  * @param  pBuffer Pointer to data buffer
  * @param  BufferSize Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HDMI_CEC_IO_ReadData(uint16_t DevAddress, uint8_t * pBuffer, uint16_t BufferSize)
{
  return (I2C2_ReceiveData(DevAddress, pBuffer, BufferSize));
}

#endif /* HAL_I2C_MODULE_ENABLED */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
