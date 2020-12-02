/**
  ******************************************************************************
  * @file    stm32f3_discovery.c
  * @author  MCD Application Team
  * @brief   This file provides set of firmware functions to manage Leds and
  *          push-button available on STM32F3-DISCOVERY Kit from STMicroelectronics.
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
#include "stm32f3_discovery.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32F3_DISCOVERY
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on STM32F3-Discovery Kit from STMicroelectronics.
  * @{
  */ 
/** @addtogroup STM32F3_DISCOVERY_Common
  * @{
  */ 

/** @addtogroup STM32F3_DISCOVERY_Private_Constants
  * @{
  */ 

/**
 * @brief STM32F3 DISCOVERY BSP Driver version number V2.1.6
   */
#define __STM32F3_DISCO_BSP_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __STM32F3_DISCO_BSP_VERSION_SUB1   (0x01) /*!< [23:16] sub1 version */
#define __STM32F3_DISCO_BSP_VERSION_SUB2   (0x06) /*!< [15:8]  sub2 version */
#define __STM32F3_DISCO_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32F3_DISCO_BSP_VERSION        ((__STM32F3_DISCO_BSP_VERSION_MAIN << 24)\
                                            |(__STM32F3_DISCO_BSP_VERSION_SUB1 << 16)\
                                            |(__STM32F3_DISCO_BSP_VERSION_SUB2 << 8 )\
                                            |(__STM32F3_DISCO_BSP_VERSION_RC))
/**
  * @}
  */ 

  /** @addtogroup STM32F3_DISCOVERY_Private_Variables
  * @{
  */ 
/**
 * @brief LED variables
  */ 
GPIO_TypeDef* LED_PORT[LEDn] = {LED3_GPIO_PORT, LED4_GPIO_PORT, LED5_GPIO_PORT, LED6_GPIO_PORT,
                                 LED7_GPIO_PORT, LED8_GPIO_PORT, LED9_GPIO_PORT, LED10_GPIO_PORT};

const uint16_t LED_PIN[LEDn] = {LED3_PIN, LED4_PIN, LED5_PIN, LED6_PIN,
                                 LED7_PIN, LED8_PIN, LED9_PIN, LED10_PIN};

/**
 * @brief BUTTON variables
 */
GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {USER_BUTTON_GPIO_PORT}; 
const uint16_t BUTTON_PIN[BUTTONn] = {USER_BUTTON_PIN}; 
const uint8_t BUTTON_IRQn[BUTTONn] = {USER_BUTTON_EXTI_IRQn};

/**
 * @brief BUS variables
 */
#ifdef HAL_SPI_MODULE_ENABLED
uint32_t SpixTimeout = SPIx_TIMEOUT_MAX;    /*<! Value of Timeout when SPI communication fails */
static SPI_HandleTypeDef SpiHandle;
#endif

#ifdef HAL_I2C_MODULE_ENABLED
static I2C_HandleTypeDef I2cHandle;
uint32_t I2cxTimeout = I2Cx_TIMEOUT_MAX;    /*<! Value of Timeout when I2C communication fails */
#endif

/**
  * @}
  */ 

/** @defgroup STM32F3_DISCOVERY_BUS Bus Operation functions
  * @{
  */ 
#ifdef HAL_I2C_MODULE_ENABLED
/* I2Cx bus function */
static void     I2Cx_Init(void);
static void     I2Cx_WriteData(uint16_t Addr, uint8_t Reg, uint8_t Value);
static uint8_t  I2Cx_ReadData(uint16_t Addr, uint8_t Reg);
static void     I2Cx_Error (void);
static void     I2Cx_MspInit(I2C_HandleTypeDef *hi2c);
#endif

#ifdef HAL_SPI_MODULE_ENABLED
/* SPIx bus function */
static void     SPIx_Init(void);
static uint8_t  SPIx_WriteRead(uint8_t byte);
static void     SPIx_Error (void);
static void     SPIx_MspInit(SPI_HandleTypeDef *hspi);
#endif

#ifdef HAL_SPI_MODULE_ENABLED
/* Link function for GYRO peripheral */
void            GYRO_IO_Init(void);
void            GYRO_IO_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void            GYRO_IO_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
#endif

#ifdef HAL_I2C_MODULE_ENABLED
/* Link function for COMPASS / ACCELEROMETER peripheral */
void      COMPASSACCELERO_IO_Init(void);
void      COMPASSACCELERO_IO_ITConfig(void);
void      COMPASSACCELERO_IO_Write(uint16_t DeviceAddr, uint8_t RegisterAddr, uint8_t Value);
uint8_t   COMPASSACCELERO_IO_Read(uint16_t DeviceAddr, uint8_t RegisterAddr);
#endif

/**
  * @}
  */ 

/** @addtogroup STM32F3_DISCOVERY_Exported_Functions
  * @{
  */ 

/**
  * @brief  This method returns the STM32F3-DISCOVERY BSP Driver revision
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32F3_DISCO_BSP_VERSION;
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED_RED
  *     @arg LED_BLUE
  *     @arg LED_ORANGE
  *     @arg LED_GREEN
  *     @arg LED_GREEN2
  *     @arg LED_ORANGE2
  *     @arg LED_BLUE2
  *     @arg LED_RED2
  * @retval None
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
  LEDx_GPIO_CLK_ENABLE(Led);

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = LED_PIN[Led];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init(LED_PORT[Led], &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED_RED
  *     @arg LED4
  *     @arg LED5
  *     @arg LED6
  *     @arg LED7
  *     @arg LED8
  *     @arg LED9
  *     @arg LED10
  * @retval None
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET); 
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED_RED
  *     @arg LED_BLUE
  *     @arg LED_ORANGE
  *     @arg LED_GREEN
  *     @arg LED_GREEN2
  *     @arg LED_ORANGE2
  *     @arg LED_BLUE2
  *     @arg LED_RED2
  * @retval None
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET); 
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED_RED
  *     @arg LED_BLUE
  *     @arg LED_ORANGE
  *     @arg LED_GREEN
  *     @arg LED_GREEN2
  *     @arg LED_ORANGE2
  *     @arg LED_BLUE2
  *     @arg LED_RED2
  * @retval None
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}


/**
  * @brief  Configures Push Button GPIO and EXTI Line.
  * @param  Button Specifies the Button to be configured.
  *   This parameter should be: BUTTON_USER
  * @param  ButtonMode Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability  
  * @retval None
  */
void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable the BUTTON Clock */
  BUTTONx_GPIO_CLK_ENABLE(Button);
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  if (ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
  }

  if (ButtonMode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; 
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0x00);
    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  }
}

/**
  * @brief  Returns the selected Push Button state.
  * @param  Button Specifies the Button to be checked.
  *   This parameter should be: BUTTON_USER  
  * @retval The Button GPIO pin value.
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @}
  */ 

/** @addtogroup STM32F3_DISCOVERY_BUS
  * @{
  */ 
/******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/
#ifdef HAL_I2C_MODULE_ENABLED
/******************************* I2C Routines**********************************/

/**
  * @brief Discovery I2Cx MSP Initialization
  * @param hi2c I2C handle
  * @retval None
  */
static void I2Cx_MspInit(I2C_HandleTypeDef *hi2c)
{

  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable SCK and SDA GPIO clocks */
  DISCOVERY_I2Cx_GPIO_CLK_ENABLE();

  /* I2Cx SD1 & SCK pin configuration */
  GPIO_InitStructure.Pin = (DISCOVERY_I2Cx_SDA_PIN | DISCOVERY_I2Cx_SCL_PIN);
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Alternate = DISCOVERY_I2Cx_AF;
  HAL_GPIO_Init(DISCOVERY_I2Cx_GPIO_PORT, &GPIO_InitStructure);

  /* Enable the I2C clock */
  DISCOVERY_I2Cx_CLK_ENABLE();
}

/**
  * @brief Discovery I2Cx Bus initialization
  * @retval None
  */
static void I2Cx_Init(void)
{
  if(HAL_I2C_GetState(&I2cHandle) == HAL_I2C_STATE_RESET)
  {
    I2cHandle.Instance = DISCOVERY_I2Cx;
    I2cHandle.Init.OwnAddress1 =  ACCELERO_I2C_ADDRESS;
    I2cHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2cHandle.Init.OwnAddress2 = 0;
    I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;	

    /* Init the I2C */
    I2Cx_MspInit(&I2cHandle);
    HAL_I2C_Init(&I2cHandle);
  }
}

/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  Addr Device address on BUS Bus.  
  * @param  Reg The target register address to write
  * @param  Value The target register value to be written 
  * @retval  None
  */
static void I2Cx_WriteData(uint16_t Addr, uint8_t Reg, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&I2cHandle, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, I2cxTimeout);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2Cx_Error();
  }
}

/**
  * @brief  Read a value in a register of the device through BUS.
  * @param  Addr Device address on BUS Bus.  
  * @param  Reg The target register address to write
  * @retval Data read at register @
  */
static uint8_t I2Cx_ReadData(uint16_t Addr, uint8_t Reg)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t value = 0;
  
  status = HAL_I2C_Mem_Read(&I2cHandle, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &value, 1, I2cxTimeout);
 
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2Cx_Error();
  
  }
  return value;
}

/**
  * @brief I2C3 error treatment function
  * @retval None
  */
static void I2Cx_Error (void)
{
  /* De-initialize the I2C comunication BUS */
  HAL_I2C_DeInit(&I2cHandle);
  
  /* Re- Initiaize the I2C comunication BUS */
  I2Cx_Init();
}
#endif


#ifdef HAL_SPI_MODULE_ENABLED
/******************************* SPI Routines**********************************/
/**
  * @brief SPIx Bus initialization
  * @retval None
  */
static void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&SpiHandle) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    SpiHandle.Instance = DISCOVERY_SPIx;
    /* SPI baudrate is set to 5.6 MHz (PCLK2/SPI_BaudRatePrescaler = 90/16 = 5.625 MHz) 
      to verify these constraints:
      ILI9341 LCD SPI interface max baudrate is 10MHz for write and 6.66MHz for read
      l3gd20 SPI interface max baudrate is 10MHz for write/read
      PCLK2 frequency is set to 90 MHz 
  */
    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    SpiHandle.Init.Direction = SPI_DIRECTION_2LINES; 
    SpiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
    SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    SpiHandle.Init.CRCPolynomial = 7;
    SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
    SpiHandle.Init.Mode = SPI_MODE_MASTER;

    SPIx_MspInit(&SpiHandle);
    HAL_SPI_Init(&SpiHandle);
  }
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received 
  *         from the SPI bus.
  * @param  Byte Byte send.
  * @retval The received byte value
  */
static uint8_t SPIx_WriteRead(uint8_t Byte)
{

  uint8_t receivedbyte = 0;
  
  /* Send a Byte through the SPI peripheral */
  /* Read byte from the SPI bus */
  if(HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*) &Byte, (uint8_t*) &receivedbyte, 1, SpixTimeout) != HAL_OK)
  {
    SPIx_Error();
  }
  
  return receivedbyte;
}


/**
  * @brief SPIx error treatment function
  * @retval None
  */
static void SPIx_Error (void)
{
  /* De-initialize the SPI comunication BUS */
  HAL_SPI_DeInit(&SpiHandle);
  
  /* Re- Initiaize the SPI comunication BUS */
  SPIx_Init();
}


/**
  * @brief SPI MSP Init
  * @param hspi SPI handle
  * @retval None
  */
static void SPIx_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable SPI1 clock  */
  DISCOVERY_SPIx_CLK_ENABLE();

  /* enable SPI1 gpio clock */
  DISCOVERY_SPIx_GPIO_CLK_ENABLE();

  /* configure SPI1 SCK, MOSI and MISO */
  GPIO_InitStructure.Pin = (DISCOVERY_SPIx_SCK_PIN | DISCOVERY_SPIx_MOSI_PIN | DISCOVERY_SPIx_MISO_PIN);
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL; /* or GPIO_PULLDOWN */
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Alternate = DISCOVERY_SPIx_AF;
  HAL_GPIO_Init(DISCOVERY_SPIx_GPIO_PORT, &GPIO_InitStructure);      
}
/**
  * @}
  */ 

/** @defgroup STM32F3_DISCOVERY_LINK_OPERATIONS Link Operation functions
  * @{
  */

/******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/

/********************************* LINK GYROSCOPE *****************************/
/**
  * @brief  Configures the GYROSCOPE SPI interface.
  * @retval None
  */
void GYRO_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure the Gyroscope Control pins ------------------------------------------*/
  /* Enable CS GPIO clock and  Configure GPIO PIN for Gyroscope Chip select */  
  GYRO_CS_GPIO_CLK_ENABLE();  
  GPIO_InitStructure.Pin = GYRO_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GYRO_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  GYRO_CS_HIGH();

  /* Enable INT1, INT2 GPIO clock and Configure GPIO PINs to detect Interrupts */
  GYRO_INT_GPIO_CLK_ENABLE();
  GPIO_InitStructure.Pin = GYRO_INT1_PIN | GYRO_INT2_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pull= GPIO_NOPULL;
  HAL_GPIO_Init(GYRO_INT_GPIO_PORT, &GPIO_InitStructure);
  
  SPIx_Init();
}

/**
  * @brief  Writes one byte to the GYROSCOPE.
  * @param  pBuffer pointer to the buffer  containing the data to be written to the GYROSCOPE.
  * @param  WriteAddr GYROSCOPE's internal address to write to.
  * @param  NumByteToWrite Number of bytes to write.
  * @retval None
  */
void GYRO_IO_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  GYRO_CS_LOW();
  
  /* Send the Address of the indexed register */
  SPIx_WriteRead(WriteAddr);
  
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    SPIx_WriteRead(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  GYRO_CS_HIGH();
}

/**
  * @brief  Reads a block of data from the GYROSCOPE.
  * @param  pBuffer pointer to the buffer that receives the data read from the GYROSCOPE.
  * @param  ReadAddr GYROSCOPE's internal address to read from.
  * @param  NumByteToRead number of bytes to read from the GYROSCOPE.
  * @retval None
  */
void GYRO_IO_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  GYRO_CS_LOW();
  
  /* Send the Address of the indexed register */
  SPIx_WriteRead(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to GYROSCOPE (Slave device) */
    *pBuffer = SPIx_WriteRead(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  GYRO_CS_HIGH();
}  
#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef HAL_I2C_MODULE_ENABLED
/********************************* LINK ACCELEROMETER *****************************/
/**
  * @brief  Configures COMPASS / ACCELEROMETER I2C interface.
  * @retval None
  */
void COMPASSACCELERO_IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable DRDY clock */
  ACCELERO_DRDY_GPIO_CLK_ENABLE();
  
  /* Enable INT1 & INT2 GPIO clock */
  ACCELERO_INT_GPIO_CLK_ENABLE();
  
  /* Mems DRDY pin configuration */
  GPIO_InitStructure.Pin = ACCELERO_DRDY_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(ACCELERO_DRDY_GPIO_PORT, &GPIO_InitStructure);
  
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(ACCELERO_DRDY_EXTI_IRQn, 0x0F, 0x00);
  HAL_NVIC_EnableIRQ(ACCELERO_DRDY_EXTI_IRQn);
  
  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.Pin = ACCELERO_INT1_PIN | ACCELERO_INT2_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(ACCELERO_INT_GPIO_PORT, &GPIO_InitStructure);
  
  I2Cx_Init();
}

/**
  * @brief  Configures COMPASS / ACCELERO click IT
  * @retval None
  */
void COMPASSACCELERO_IO_ITConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable INT1 & INT2 GPIO clock */
  ACCELERO_INT_GPIO_CLK_ENABLE();
  
  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.Pin = ACCELERO_INT1_PIN | ACCELERO_INT2_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(ACCELERO_INT_GPIO_PORT, &GPIO_InitStructure);
  
  /* Enable and set Button EXTI Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(ACCELERO_INT1_EXTI_IRQn, 0x0F, 0x00);
  HAL_NVIC_EnableIRQ(ACCELERO_INT1_EXTI_IRQn);
  
}

/**
  * @brief  Writes one byte to the COMPASS / ACCELEROMETER.
  * @param  DeviceAddr specifies the slave address to be programmed.
  * @param  RegisterAddr specifies the COMPASS / ACCELEROMETER register to be written.
  * @param  Value Data to be written
  * @retval   None
 */
void COMPASSACCELERO_IO_Write(uint16_t DeviceAddr, uint8_t RegisterAddr, uint8_t Value)
{
  /* call I2Cx Read data bus function */
  I2Cx_WriteData(DeviceAddr, RegisterAddr, Value);
}

/**
  * @brief  Reads a block of data from the COMPASS / ACCELEROMETER.
  * @param  DeviceAddr specifies the slave address to be programmed(ACC_I2C_ADDRESS or MAG_I2C_ADDRESS).
  * @param  RegisterAddr specifies the COMPASS / ACCELEROMETER internal address register to read from
  * @retval ACCELEROMETER register value
  */ 
uint8_t COMPASSACCELERO_IO_Read(uint16_t DeviceAddr, uint8_t RegisterAddr)
{
  /* call I2Cx Read data bus function */   
  return I2Cx_ReadData(DeviceAddr, RegisterAddr);
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

/******************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
