/**
  ******************************************************************************
  * @file    BSP/Src/temperature_sensor.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to manage an I2C Temperature Sensor
  *          =================================================================== 
  *          Notes:
  *           - This driver is intended for STM32F3xx families devices only. 
  *           - The I2C Temperature Sensor is available on the STM32303E-EVAL RevC board.
  *             To use this driver, you have to set the
  *             Jumper JP5 and JP6 needs to I2C2 position.
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
typedef enum {NOT_READY = 0, READY = !NOT_READY} Status; 
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TSENSOR_SetHint(void);
static void TSENSOR_Display_Temperature(uint16_t temperature);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  EEPROM Demo
  * @param  None
  * @retval None
  */
void TSENSOR_demo (void)
{ 
  uint16_t TempValue = 0;
  uint8_t  TsensorReady = 0;
  uint8_t  RequestSample = 0;
  uint32_t Tick = 0;

  TSENSOR_SetHint();

  /* Initialize the I2C TSENSOR driver ----------------------------------------*/
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  if (BSP_TSENSOR_Init() != TSENSOR_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Temperature Sensor not detected", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"Verify that jumper JP5 and JP6", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 160, (uint8_t*)"are well set to I2C2 position", CENTER_MODE);
  }
  else
  {
    TsensorReady = READY;
    RequestSample = SET;
  }

  while (1)
  {
    if((TsensorReady == READY) && (RequestSample == SET))
    {
      TempValue = BSP_TSENSOR_ReadTemp();
      TSENSOR_Display_Temperature(TempValue);
      Tick = HAL_GetTick();
      RequestSample = RESET;
    }
    else
    {
      /* Request a Temperature sampling each 1s <-> 1000 ms */
      if(HAL_GetTick() >= Tick + 1000)
      {
        RequestSample = SET;
      }
    }
    
    
    if(CheckForUserInput() > 0)
    {
      return;
    }
    HAL_Delay(5);
  }
}

/**
  * @brief  Display TSENSOR Demo Hint
  * @param  None
  * @retval None
  */
static void TSENSOR_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"TEMPERATURE SENSOR", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"read a Temperature", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
 }

  /**
  * @brief  Display temperature
  * @param  temperature : temperature value
  * @retval None
  */
static void TSENSOR_Display_Temperature(uint16_t temperature)
{
  uint8_t TempCelsiusDisplay[] = "+abc.dddd C";
  uint16_t TempValue = temperature;
  uint16_t TempValueCelsius = 0;
  uint32_t TempCelsius = 0;

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Measured Temperature : ", CENTER_MODE);

  /* Change Font size to display Temperature Value */
  BSP_LCD_SetFont(&Font16);
  
  if (TempValue <= 2048)
  {
    /* Positive temperature measured */
    TempCelsiusDisplay[0] = '+';
    /* Initialize the temperature sensor value*/
    TempValueCelsius = TempValue;
    /* Set Text color to Green */
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);    
  }
  else
  {
    /* Negative temperature measured */
    TempCelsiusDisplay[0] = '-';
    /* Remove temperature value sign */
    TempValueCelsius = 0x1000 - TempValue;
    /* Set Text color to Red */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
  }

  TempCelsius = 0;
  /* Calculate temperature digits in �C */
  if (TempValueCelsius & 0x01)
  {
    TempCelsius += 625;     
  }
  if (TempValueCelsius & 0x02)
  {
    TempCelsius += 1250;
  }
  if (TempValueCelsius & 0x04)
  {
    TempCelsius += 2500;
  }
  if (TempValueCelsius & 0x08)
  {
    TempCelsius += 5000;
  }
  
  TempCelsiusDisplay[5] = (TempCelsius / 1000) + 0x30;
  TempCelsiusDisplay[6] = ((TempCelsius % 1000) / 100) + 0x30;
  TempCelsiusDisplay[7] = (((TempCelsius % 1000) % 100) / 10)+ 0x30;
  TempCelsiusDisplay[8] = (((TempCelsius % 1000) % 100) % 10) + 0x30;
    
  TempValueCelsius >>= 4;

  TempCelsiusDisplay[1] = (TempValueCelsius / 100) + 0x30;
  TempCelsiusDisplay[2] = ((TempValueCelsius % 100) / 10) + 0x30;
  TempCelsiusDisplay[3] = ((TempValueCelsius % 100) % 10) + 0x30;

  /* Display Temperature value on LCD */
  BSP_LCD_DisplayStringAt(0, 145, TempCelsiusDisplay, CENTER_MODE); 
}

/**
  * @}
  */ 
