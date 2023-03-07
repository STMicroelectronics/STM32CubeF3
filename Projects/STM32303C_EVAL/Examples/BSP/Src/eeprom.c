/**
  ******************************************************************************
  * @file    BSP/Src/eeprom.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to manage I2C EEPROM memory
  *          and a SPI EEPROM memory.
  *          =================================================================== 
  *          Notes:
  *           - This driver is intended for STM32F303xB/303xC families devices only. 
  *           - The I2C EEPROM memory (M24LR64) is available on separate daughter 
  *             board ANT7-M24LR-A, which is provided with the STM32303C-EVAL RevC board.
  *             To use this driver with M24LR64, you have to connect
  *             the ANT7-M24LR-A to CN1 connector of STM32303C-EVAL RevC board.
  *             Jumper JP5 and JP6 needs to be set in I2C2 position.
  *             Jumper JP8 (E2P WC) needs to be set.
  *
  *           - The SPI EEPROM memory (M95M01) is available directly
  *             on STM32303C-EVAL RevC board.
  *          ===================================================================
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F3xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus; 
/* Private define ------------------------------------------------------------*/
#define EEPROM_FEATURES_NUM     2
#define EEPROM_TEST_NB          2
#define BUFFER_SIZE1            (countof(Tx1Buffer)-1 + 14)
#define EEPROM_WRITE_ADDRESS1   0x50
#define EEPROM_READ_ADDRESS1    0x50
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))
/* Private variables ---------------------------------------------------------*/
static uint8_t EepromFeature = 0;
static uint8_t EepromSelected[EEPROM_TEST_NB] = {BSP_EEPROM_M24LR64, BSP_EEPROM_M95M01};
static uint8_t EepromTestNb = 0;
uint8_t EepromDetected = 1;
/* Private function prototypes -----------------------------------------------*/
static void EEPROM_SetHint(void);
static void EEPROM_Show_Feature(uint8_t feature);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

extern __IO uint8_t NbLoop;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  EEPROM Demo
  * @param  None
  * @retval None
  */
void EEPROM_demo (void)
{ 
  EEPROM_SetHint();
  EepromFeature = 0;
  EepromTestNb = 0;

  /* Select EEPROM Component */
  BSP_EEPROM_SelectDevice(EepromSelected[EepromTestNb++]);
  EEPROM_Show_Feature(EepromFeature++);

  while (1)
  {
    if(CheckForUserInput() > 0)
    {
      if(EepromFeature < EEPROM_FEATURES_NUM)
      {
       EEPROM_Show_Feature(EepromFeature++); 
      }
      else
      {
        return;
      }
      
      if(EepromTestNb < EEPROM_TEST_NB)
      {
        /* Select another EEPROM */
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_DisplayStringAt(0, 205, (uint8_t *)"Press KEY button to select other EEPROM", CENTER_MODE);
        BSP_EEPROM_SelectDevice(EepromSelected[EepromTestNb++]);
        
        /* Restart example feature */
        EepromFeature = 0;
      }
    }
    HAL_Delay(100);
  }
}

/**
  * @brief  Display EEPROM Demo Hint
  * @param  None
  * @retval None
  */
static void EEPROM_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"EEPROM", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows the different", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"EEPROM Features, use KEY button", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"to start EEPROM data transfer", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
 }

/**
  * @brief  Show EEPROM Features
  * @param  feature : feature index
  * @retval None
  */
static void EEPROM_Show_Feature(uint8_t feature)
{
  uint8_t Tx1Buffer[] = "STM32F303C EEPROM";
  
  uint8_t Rx1Buffer[BUFFER_SIZE1] = {0}; 
  uint8_t Tx2Buffer[BUFFER_SIZE1] = {0}; 
  __IO TestStatus TransferStatus1 = FAILED;
  __IO uint32_t NumDataRead = 0; 

  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize()- 24, BSP_LCD_GetYSize() - 104);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  /* Initialize the EEPROM driver --------------------------------------------*/
  if (BSP_EEPROM_Init() != EEPROM_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Check if HW connected or", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"HW version not supported", CENTER_MODE); 
    return;
  }
  EepromDetected = 1;
  
  switch (feature)
  {
  case 0:
    /* Read old parameter in EEPROM */
    if (EepromDetected == 1)
    {
      /* Set the Number of data to be read */
      NumDataRead = (uint32_t)BUFFER_SIZE1;
      
      /* Read from I2C EEPROM from EEPROM_READ_ADDRESS1 */
      if (BSP_EEPROM_ReadBuffer(Rx1Buffer, EEPROM_READ_ADDRESS1, (uint32_t *)(&NumDataRead)) != EEPROM_OK)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);    
        BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Init issue at read old data", CENTER_MODE); 
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"Press again KEY button", CENTER_MODE); 
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t*)"To write new data", CENTER_MODE); 
        return;
      }
      BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"String read", CENTER_MODE); 
      BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"in the current EEPROM selected:", CENTER_MODE); 
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);    
      BSP_LCD_DisplayStringAt(0, 160, Rx1Buffer, CENTER_MODE); 
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DisplayStringAt(0, 190, (uint8_t*)"Press KEY button", CENTER_MODE); 
      BSP_LCD_DisplayStringAt(0, 205, (uint8_t*)"To write new data", CENTER_MODE); 
    }
    else
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);    
      BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Problem to communicate", CENTER_MODE); 
      BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"with EEPROM", CENTER_MODE); 
      BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"Press again KEY button", CENTER_MODE); 
    }
    
    break;
    
  case 1:
     /* Write new parameter in EEPROM */
    if (EepromDetected == 1)
    {
      snprintf((char*)Tx2Buffer, BUFFER_SIZE1, "%s_%d Ex. test %d", Tx1Buffer, EepromTestNb, NbLoop);
      /* First write in the memory followed by a read of the written data ----*/
      /* Write on I2C EEPROM to EEPROM_WRITE_ADDRESS1 */
      if (BSP_EEPROM_WriteBuffer(Tx2Buffer, EEPROM_WRITE_ADDRESS1, BUFFER_SIZE1) != EEPROM_OK)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);    
        BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Init issue at write", CENTER_MODE); 
        return;
      }
      
      /* Set the Number of data to be read */
      NumDataRead = (uint32_t)BUFFER_SIZE1;
      
      /* Read from I2C EEPROM from EEPROM_READ_ADDRESS1 */
      if (BSP_EEPROM_ReadBuffer(Rx1Buffer, EEPROM_READ_ADDRESS1, (uint32_t *)(&NumDataRead)) != EEPROM_OK)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);    
        BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Init issue at read", CENTER_MODE); 
        return;
      }
      
      /* Check if the data written to the memory is read correctly */
      TransferStatus1 = Buffercmp(Tx2Buffer, Rx1Buffer, BUFFER_SIZE1);
      if(TransferStatus1 != FAILED)
      {
        BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"String writes", CENTER_MODE); 
        BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"in the current EEPROM selected:", CENTER_MODE); 
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);    
        BSP_LCD_DisplayStringAt(0, 160, Tx2Buffer, CENTER_MODE); 
      }
      else 
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);    
        BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"FAILED to write!", CENTER_MODE); 
        BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Press KEY button to end test", CENTER_MODE); 
      }
    }
    else
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);    
      BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Problem to communicate", CENTER_MODE); 
      BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"again with EEPROM", CENTER_MODE); 
      BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"Press KEY button to end test", CENTER_MODE); 
    }
    break;
  }
}

/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval 0.
  */
void BSP_EEPROM_TIMEOUT_UserCallback(void)
{
  EepromDetected = 0;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}
/**
  * @}
  */ 
/**
  * @}
  */ 
