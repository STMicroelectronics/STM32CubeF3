/**
  ******************************************************************************
  * @file    SMBUS/SMBUS_TSENSOR/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F3xx SMBUS HAL API.
  *          The communication is done using an Temperature Sensor STTS751
  *          on STM32303C-EVAL RevC Eval board.
  *          =================================================================== 
  *          Notes:
  *           - This example is intended for STM32F303xC families devices only. 
  *          =================================================================== 
  *          Notes:
  *           - The Temperature Sensor (STTS751) is compatible
  *             with the SMBUS protocol.
  *           - Jumper JP5 and JP6 needs to be set in I2C2 position.
  *           - Jumper JP1 (SMB) needs to be set.
  *          ===================================================================
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
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stlogo.h"

/** @addtogroup STM32F3xx_HAL_Examples
  * @{
  */

/** @addtogroup SMBUS_TSENSOR
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TSENSOR_ADDR            0x90    /* STTS751 Address  */
#define TEMPERATURE_LOW         23      /* 23°C             */
#define TEMPERATURE_HIGH        26      /* 26°C             */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TSENSOR_InitTypeDef TSENSOR_InitStructure;

/* Useful variables */
uint16_t temperaturevalue = 0;
uint8_t  alertoccurs = 0;
uint8_t  requestsample = 0;
uint32_t tick = 0;
uint8_t  addressalert = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Display_ExampleDescription(void);
static void TSENSOR_SetHint(void);
static void TSENSOR_Display_Temperature(uint16_t Temperature);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F3xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 72 MHz */
  SystemClock_Config();
  
  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* Configure Key Button*/
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

  /*##-1- Initialize the LCD #################################################*/
  BSP_LCD_Init();
   
  /*##-2- Display welcome messages on LCD ####################################*/  
  Display_ExampleDescription();

  /* Wait for Key Button press before starting the Example */
  while (BSP_PB_GetState(BUTTON_KEY) != GPIO_PIN_RESET)
  {
  }
  
  /* Wait for Key Button release before starting the Example */
  while (BSP_PB_GetState(BUTTON_KEY) != GPIO_PIN_SET)
  {
  }

  /*##-3- Display Example Template ###########################################*/  
  TSENSOR_SetHint();

  /*##-4- Configure the Temperature Sensor ###################################*/
  /* Conversion 12 bits in continuous mode at one conversion per second */
  /* Alert outside range Limit Temperature */
  TSENSOR_InitStructure.AlertMode             = TSENSOR_ALERT_ENABLE;
  TSENSOR_InitStructure.ConversionMode        = TSENSOR_CONTINUOUS_MODE;
  TSENSOR_InitStructure.ConversionResolution  = TSENSOR_CONV_12BITS;
  TSENSOR_InitStructure.ConversionRate        = TSENSOR_ONE_PER_SECOND;           
  TSENSOR_InitStructure.TemperatureLimitHigh  = TEMPERATURE_HIGH;
  TSENSOR_InitStructure.TemperatureLimitLow   = TEMPERATURE_LOW;

  if (TSENSOR_Init(TSENSOR_ADDR, &TSENSOR_InitStructure) != TSENSOR_OK)
  {
    /* Initialization Error */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Verify that jumper JP5 and JP6", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"are well set to I2C2 position", CENTER_MODE);
    Error_Handler();
  }

  /* Initialize tick counter */
  tick = HAL_GetTick();
  
  /*##-5- Main loop to manage Alert and display Temperature Measured #########*/
  while (1)
  {
    /* Check if a new temperature read is requested to display                */
    if (requestsample == 1)
    {
      /* Read and Display the current temperature                             */
      temperaturevalue = TSENSOR_ReadTemp(TSENSOR_ADDR);
      TSENSOR_Display_Temperature(temperaturevalue);
      
      /* Reset the sampling request                                           */
      tick = HAL_GetTick();
      requestsample = 0;
    }
    else
    {
      /* Request a Temperature sampling each 1s <-> 1000 ms                   */
      if (HAL_GetTick() >= tick + 1000)
      {
        /* Set the sampling request                                           */
        requestsample = 1;
      }
    }
    
    /* Check if an alert occurs                                               */
    if (alertoccurs == 1)
    {
      /* Get the Address of temperature sensor in Alert Mode                  */
      addressalert = TSENSOR_AlerteResponseAddressRead();
      
      /* Display warning message depends on Limit value                       */
      if ((TSENSOR_ReadStatus(addressalert) & TSENSOR_TEMP_EXCEED_HIGH_LIMIT) == TSENSOR_TEMP_EXCEED_HIGH_LIMIT)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        /* Display warning message Temperature high limit exceeded            */
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"Temperature Limit High", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"has been exceeded", CENTER_MODE);
      }
      else
      {
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        /* Display warning message Temperature is at or blow low limit        */
        BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"Temperature is at or", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"below the Low Limit", CENTER_MODE);
      }
      alertoccurs = 0;
    }
        
    HAL_Delay(5);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV                     = 1
  *            PLLMUL                         = RCC_PLL_MUL9 (9)
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler(); 
  }
    	
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    Error_Handler(); 
  }
}

/**
  * @brief  Display main example message
  * @param  None
  * @retval None
  */
static void Display_ExampleDescription(void)
{
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */ 
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);  

  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32F303xC", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"Example", CENTER_MODE);
  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 65, (uint8_t *)stlogo);
  
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 20, (uint8_t *)"Copyright (c) STMicroelectronics 2016", CENTER_MODE);
  
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 15, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_DARKBLUE); 
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 30, (uint8_t *)"Press Key Button to start :", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 45, (uint8_t *)" TEMPERATURE SENSOR Example", CENTER_MODE);
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
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"and manage temperature limit alert", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);

  /* Prepare LCD to display */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

  /**
  * @brief  Display temperature
  * @param  temperature : temperature value
  * @retval None
  */
static void TSENSOR_Display_Temperature(uint16_t Temperature)
{
  uint8_t tempcelsiusdisplay[] = "+abc.dddd C";
  uint16_t temperaturevalue = Temperature;
  uint16_t temperaturevaluecelsius = 0;
  uint32_t tempcelsius = 0;

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Measured Temperature : ", CENTER_MODE);

  /* Change Font size to display Temperature Value */
  BSP_LCD_SetFont(&Font16);

  /* Clear previous warning message */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"                       ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"                       ", CENTER_MODE);

  /* Verify the sign of the temperature */
  if (temperaturevalue <= 2048)
  {
    /* Positive temperature measured */
    tempcelsiusdisplay[0] = '+';
    /* Initialize the temperature sensor value*/
    temperaturevaluecelsius = temperaturevalue;
    /* Set Text color to Green */
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);    
  }
  else
  {
    /* Negative temperature measured */
    tempcelsiusdisplay[0] = '-';
    /* Remove temperature value sign */
    temperaturevaluecelsius = 0x1000 - temperaturevalue;
    /* Set Text color to Red */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  }

  tempcelsius = 0;
  /* Calculate temperature digits in °C */
  if (temperaturevaluecelsius & 0x01)
  {
    tempcelsius += 625;     
  }
  if (temperaturevaluecelsius & 0x02)
  {
    tempcelsius += 1250;
  }
  if (temperaturevaluecelsius & 0x04)
  {
    tempcelsius += 2500;
  }
  if (temperaturevaluecelsius & 0x08)
  {
    tempcelsius += 5000;
  }
  
  tempcelsiusdisplay[5] = (tempcelsius / 1000) + 0x30;
  tempcelsiusdisplay[6] = ((tempcelsius % 1000) / 100) + 0x30;
  tempcelsiusdisplay[7] = (((tempcelsius % 1000) % 100) / 10)+ 0x30;
  tempcelsiusdisplay[8] = (((tempcelsius % 1000) % 100) % 10) + 0x30;
    
  temperaturevaluecelsius >>= 4;

  tempcelsiusdisplay[1] = (temperaturevaluecelsius / 100) + 0x30;
  tempcelsiusdisplay[2] = ((temperaturevaluecelsius % 100) / 10) + 0x30;
  tempcelsiusdisplay[3] = ((temperaturevaluecelsius % 100) % 10) + 0x30;

  /* Display Temperature value on LCD */
  BSP_LCD_DisplayStringAt(0, 145, tempcelsiusdisplay, CENTER_MODE); 
}

/**
  * @brief  TSENSOR error callbacks.
  * @param  Error : Temperature Sensor Error status.
  * @retval None
  */
void TSENSOR_ErrorCallback(uint16_t Error)
{
  if (Error == TSENSOR_ALERT)
  {
    alertoccurs = 1;
  }
  else
  {
    Error_Handler();
  }
  
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
    /* Clear Previous message on the LCD */
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
    BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);

    /* Display Communication error message */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"An error occurs during", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"communication with", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"the temperature sensor", CENTER_MODE);

    /* Turn LED3 on */
    BSP_LED_On(LED3);
    while(1)
    {
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
