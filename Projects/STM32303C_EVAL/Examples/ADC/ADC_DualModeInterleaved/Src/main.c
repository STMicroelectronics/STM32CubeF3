/**
  ******************************************************************************
  * @file    ADC/ADC_DualModeInterleaved/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the ADC
  *          peripheral to convert a regular channel in Dual interleaved mode.
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

/** @addtogroup ADC_DualModeInterleaved
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle1;
ADC_HandleTypeDef    AdcHandle2;

/* Variable used to get converted value */
__IO uint16_t uhADCDualConvertedValue[256];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void ADC_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
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

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  
  /* Configure the system clock to 72 MHz */
  SystemClock_Config();


  /*##-1- Configure ADC1 and ADC2 peripherals ################################*/
  ADC_Config();

  /* Run the ADC calibration in single-ended mode */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle1, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

  if (HAL_ADCEx_Calibration_Start(&AdcHandle2, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

  /*##-2- Enable ADC2 ########################################################*/
  if (HAL_ADC_Start(&AdcHandle2) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /*##-3- Start ADC1 and ADC2 multimode conversion process and enable DMA ####*/
  if (HAL_ADCEx_MultiModeStart_DMA(&AdcHandle1, (uint32_t *)&uhADCDualConvertedValue[0], 256) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /* Infinite loop */
  while (1)
  {
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
  {
  }
}

/**
  * @brief  ADC configuration
  * @note   This function Configure the ADC peripheral
            1) Enable peripheral clocks
            2) Configure ADC Channel 12 pin as analog input
            3) DMA2_Stream0 channel2 configuration
            4) Configure ADC1 Channel 12
            5) Configure ADC2 Channel 12
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef sConfig;
  ADC_MultiModeTypeDef   mode;

  /*##-1- Configure the ADC2 peripheral ######################################*/
  AdcHandle2.Instance          = ADCy;

  AdcHandle2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  AdcHandle2.Init.Resolution = ADC_RESOLUTION_8B;
  AdcHandle2.Init.ScanConvMode = ENABLE;
  AdcHandle2.Init.ContinuousConvMode = ENABLE;
  AdcHandle2.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle2.Init.NbrOfDiscConversion = 0;
  AdcHandle2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
  AdcHandle2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle2.Init.NbrOfConversion = 1;
  AdcHandle2.Init.DMAContinuousRequests = ENABLE;
  AdcHandle2.Init.EOCSelection = ENABLE;

  if (HAL_ADC_Init(&AdcHandle2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure ADC2 regular channel #####################################*/
  sConfig.Channel = ADCxy_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_4CYCLES_5;
  sConfig.Offset = 0;

  if (HAL_ADC_ConfigChannel(&AdcHandle2, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /*##-3- Configure the ADC1 peripheral ######################################*/
  AdcHandle1.Instance          = ADCx;

  AdcHandle1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  AdcHandle1.Init.Resolution = ADC_RESOLUTION_8B;
  AdcHandle1.Init.ScanConvMode = DISABLE;
  AdcHandle1.Init.ContinuousConvMode = ENABLE;
  AdcHandle1.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle1.Init.NbrOfDiscConversion = 0;
  AdcHandle1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
  AdcHandle1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle1.Init.NbrOfConversion = 1;
  AdcHandle1.Init.DMAContinuousRequests = ENABLE;
  AdcHandle1.Init.EOCSelection = ENABLE;

  if (HAL_ADC_Init(&AdcHandle1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-4- Configure ADC1 regular channel #####################################*/
  if (HAL_ADC_ConfigChannel(&AdcHandle1, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /*##-5- Configure Multimode ################################################*/
  mode.Mode = ADC_DUALMODE_INTERL;
  mode.DMAAccessMode = ADC_DMAACCESSMODE_8_6_BITS;
  mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_6CYCLES;
  if (HAL_ADCEx_MultiModeConfigChannel(&AdcHandle1, &mode) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  hadc : hadc handle
  * @note   This example shows a simple way to report end of conversion, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Turn LED1 on: Transfer process is correct */
  BSP_LED_On(LED1);
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
