/**
  ******************************************************************************
  * @file    SDADC/SDADC_Voltmeter/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use SDADC through
  *          the STM32F3xx HAL API to realize a pressure measurement. The sensor
  *          used is MPX2102A.
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

/** @addtogroup STM32F3xx_HAL_Examples
  * @{
  */

/** @addtogroup SDADC_Voltmeter
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* InjectedConvData contains the result of the conversion, updated in HAL_SDADC_InjectedConvCpltCallback*/
__IO int16_t InjectedConvData = 0;

/* hsdadc is a handle to manage the SDADC*/
SDADC_HandleTypeDef hsdadc;

/* InjChannel contains the converted channel after each conversion. This is not used in this example.*/
__IO uint32_t InjChannel = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void ErrorDisplay(uint8_t *strLine4, uint8_t *strLine5, uint8_t *strLine6);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* inputVoltageMv contains the voltage at POT_GPIO_PIN */
  __IO float inputVoltageMv = 0;

  /* LCDstr is a buffer to store the string to display on screen */
  uint8_t LCDstr[40] = {0};

  /* confParam contains the channel configuration */
  SDADC_ConfParamTypeDef confParam;

  /* endOfExample is set to 1 when the user press the key button. This give an overview on how to disable the SDADC when it is no more required to use it.*/
  uint32_t endOfExample = 0;

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

  /* Configure the system clock to 72 Mhz */
  SystemClock_Config();

  /* Configure the User Button in GPIO Mode */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

  /* Initialise the LCD */
  BSP_LCD_Init();

  /* Clear the LCD */
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAtLine(0, (uint8_t *)MESSAGE1);

  /* Fill the SDADC handler with the parameters of initialisation */
  /* An external reference is used. The other options are not used (low power, fast conversion,slow clock mode)*/
  hsdadc.Instance = SDADC1;
  hsdadc.Init.ReferenceVoltage = POT_SDADC_VREF;
  hsdadc.Init.IdleLowPowerMode = SDADC_LOWPOWER_NONE;
  hsdadc.Init.FastConversionMode = SDADC_FAST_CONV_DISABLE;
  hsdadc.Init.SlowClockMode = SDADC_SLOW_CLOCK_DISABLE;

  /* -1- Initialise the SDADC */
  if (HAL_SDADC_Init(&hsdadc) != HAL_OK)
  {
    /* An error occurs during the initialisation */
    ErrorDisplay((uint8_t*)"Fail to initialise",(uint8_t*)"the SDADC",(uint8_t*)"");  
  }

  /* -2- Prepare the channel configuration */
  confParam.CommonMode = SDADC_COMMON_MODE_VSSA;
  confParam.Gain = POT_SDADC_GAIN;
  confParam.InputMode = SDADC_INPUT_MODE_SE_ZERO_REFERENCE;
  confParam.Offset = 0x00000000;
  if (HAL_SDADC_PrepareChannelConfig(&hsdadc, SDADC_CONF_INDEX_0, &confParam) != HAL_OK)
  {
    /* An error occurs during the preparation of the channel's configuration */
    ErrorDisplay((uint8_t*)"Fail to prepare",(uint8_t*)"the channel",(uint8_t*)"configuration");
  }

  /* associate POT_SDADC_CHANNEL to the configuration 0 */
  if (HAL_SDADC_AssociateChannelConfig(&hsdadc, POT_SDADC_CHANNEL, SDADC_CONF_INDEX_0) != HAL_OK)
  {
    /* An error occurs during the association of the channel to the prepared configuration */
    ErrorDisplay((uint8_t*)"Fail to associate",(uint8_t*)"the channel to the",(uint8_t*)"configuration");
  }

  /* select POT_SDADC_CHANNEL for injected conversion and continuous mode */
  if (HAL_SDADC_InjectedConfigChannel(&hsdadc, POT_SDADC_CHANNEL, SDADC_CONTINUOUS_CONV_ON) != HAL_OK)
  {
    /* An error occurs during the selection of the channel for the injected conversion */
    ErrorDisplay((uint8_t*)"Fail to select the",(uint8_t*)"channel for an",(uint8_t*)"injected conversion");
  }

  /* Select external trigger for injected conversion */
  if (HAL_SDADC_SelectInjectedTrigger(&hsdadc, SDADC_SOFTWARE_TRIGGER) != HAL_OK)
  {
    /* An error occurs during the selection of the trigger */
    ErrorDisplay((uint8_t*)"Fail to select the", (uint8_t*)"trigger for an", (uint8_t*)"injected conversion");
  }

  /* Start Calibration in polling mode */
  if (HAL_SDADC_CalibrationStart(&hsdadc, SDADC_CALIBRATION_SEQ_1) != HAL_OK)
  {
    /* An error occurs during the starting phase of the calibration */
    ErrorDisplay((uint8_t*)"Fail to start the",(uint8_t*)"calibration",(uint8_t*)"");
  }

  /* Pool for the end of calibration */
  if (HAL_SDADC_PollForCalibEvent(&hsdadc, HAL_MAX_DELAY) != HAL_OK)
  {
    /* An error occurs while waiting for the end of the calibration */
    ErrorDisplay((uint8_t*)"Fail to correctly",(uint8_t*)"calibrate SDADC",(uint8_t*)"");
  }

  /* Start injected conversion in interrupt mode */
  if (HAL_SDADC_InjectedStart_IT(&hsdadc) != HAL_OK)
  {
    /* An error occurs during the configuration of the injected conversion in interrupt mode */
    ErrorDisplay((uint8_t*)"Fail to start the",(uint8_t*)"injected convertion",(uint8_t*)"in Interrupt mode");
  }

  /* The initialisation of SDADC occurs correctly */

  /* Set the LCD Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_ClearStringLine(5);
  BSP_LCD_ClearStringLine(9);
  BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"KEY button to stop");

  while (endOfExample == 0)
  {
    /* Compute the input voltage */
    inputVoltageMv = (((InjectedConvData + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL));

    /* Write result to LCD */
    sprintf((char *)LCDstr, " RV3 = %2.0f mV  ", inputVoltageMv);
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *)LCDstr);

    /* If the KEY button is pressed, the end of the example is reached*/
    if (BSP_PB_GetState(BUTTON_KEY) == RESET)
    {
      while (BSP_PB_GetState(BUTTON_KEY) == RESET);
      endOfExample = 1;
    }
  }

  /* Stop the injected conversion in interrupt mode */
  if (HAL_SDADC_InjectedStop_IT(&hsdadc) != HAL_OK)
  {
    /* An error occurs while stopping the injected conversion */
    ErrorDisplay((uint8_t*)"Fail to stop the",(uint8_t*)"injected conversion",(uint8_t*)"");
  }

  /* DeInit the SDADC */
  HAL_SDADC_DeInit(&hsdadc);

  /* This is the end of the example*/
  BSP_LCD_ClearStringLine(9);
  BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"  End of example");

  return 1;
}

/**
  * @brief  Display an error message on screen and never return from the while(1) loop.
  * @param  strLine4 : the string displayed on line 4.
  * @param  strLine5 : the string displayed on line 5.
  * @param  strLine6 : the string displayed on line 6.
  * @retval None
  */
void ErrorDisplay(uint8_t *strLine4, uint8_t *strLine5, uint8_t *strLine6)
{
  /* Set the Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);

  /* Set the Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_RED);

  /* Remark: a line can contains only 18 characters */
  BSP_LCD_DisplayStringAtLine(4, strLine4);
  BSP_LCD_DisplayStringAtLine(5, strLine5);
  BSP_LCD_DisplayStringAtLine(6, strLine6);
  while (1);
}

/**
  * @brief  Injected conversion complete callback.
  * @param  hsdadc : SDADC handle.
  * @retval None
  */
void HAL_SDADC_InjectedConvCpltCallback(SDADC_HandleTypeDef *hsdadc)
{
  /* Get conversion value */
  InjectedConvData = HAL_SDADC_InjectedGetValue(hsdadc, (uint32_t *) &InjChannel);
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
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
void assert_failed(uint8_t *file, uint32_t line)
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
