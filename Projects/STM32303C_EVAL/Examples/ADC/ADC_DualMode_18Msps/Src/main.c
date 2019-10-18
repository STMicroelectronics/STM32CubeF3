/**
  ******************************************************************************
  * @file    ADC/ADC_DualModeInterleaved_18MSamplesPerSecond/Src/main.c
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

/* Variable containing ADC conversions results */
__IO  uint16_t aADCDualConvertedValue[256];
  
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
  /* Configuration of ADC peripheral */
  ADC_Config();

  /* Run the ADC calibration in single-ended mode */  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle1, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }
  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle2, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }
  
  /*##-2- Enable ADC2 ########################################################*/
  if (HAL_ADC_Start(&AdcHandle2) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /*##-3- Start ADC1 and ADC2 multimode conversion process and enable DMA ####*/
  if (HAL_ADCEx_MultiModeStart_DMA(&AdcHandle1, (uint32_t *)aADCDualConvertedValue, 256) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /* Array "aADCDualConvertedValue" contains both ADC results on 16 bits:     */
  /*  - ADC master results in the 8 LSB [7:0]                                 */
  /*  - ADC slave results in the 8 MSB [15:8]                                 */
  
  /* Infinite loop */
  while (1)
  {
    __NOP();
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
            1) Configuration of ADC1 peripheral (ADC master)
            2) Configuration of ADC2 peripheral (ADC slave)
            3) Configuration of channel on ADC1 regular group on rank 1
            4) Configuration of channel on ADC2 regular group on rank 1
            5) Configuration of multimode
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef sConfig;
  ADC_MultiModeTypeDef   mode;

  /*##-1- Configuration of ADC1 peripheral (ADC master) ######################*/
  AdcHandle1.Instance = ADCx;

  /* Configuration of ADC1 init structure: ADC parameters and regular group */
  AdcHandle1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;    /* ADC clock to AHB without prescaler to have maximum frequency 72MHz */
  AdcHandle1.Init.Resolution = ADC_RESOLUTION_6B;                /* ADC resolution 6 bits to have conversion time = 6.5 ADC clock cycles */
  AdcHandle1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle1.Init.ScanConvMode = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  AdcHandle1.Init.LowPowerAutoWait = DISABLE;
  AdcHandle1.Init.ContinuousConvMode = ENABLE;                  /* Continuous mode to have maximum conversion speed (no delay between conversions) */
  AdcHandle1.Init.NbrOfConversion = 1;                          /* Parameter discarded because sequencer is disabled */
  AdcHandle1.Init.DiscontinuousConvMode = DISABLE;              /* Parameter discarded because sequencer is disabled */
  AdcHandle1.Init.NbrOfDiscConversion = 1;                      /* Parameter discarded because sequencer is disabled */
  AdcHandle1.Init.ExternalTrigConv = ADC_SOFTWARE_START;        /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle1.Init.DMAContinuousRequests = ENABLE;               /* ADC-DMA continuous requests to match with DMA in circular mode */
  AdcHandle1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;

  if (HAL_ADC_Init(&AdcHandle1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configuration of ADC2 peripheral (ADC slave) #######################*/
  AdcHandle2.Instance = ADCy;

  /* Configuration of ADC2 init structure: ADC parameters and regular group */
  /* Same configuration as ADC1 */
  AdcHandle2.Init = AdcHandle1.Init;
  
  if (HAL_ADC_Init(&AdcHandle2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-3- Configuration of channel on ADC1 regular group on rank 1 ###########*/
  sConfig.Channel = ADCxy_CHANNEL;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.Offset = 0;

  if (HAL_ADC_ConfigChannel(&AdcHandle1, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /*##-4- Configuration of channel on ADC2 regular group on rank 1 ###########*/
  /* Same channel as ADC1 for dual mode interleaved: both ADC are converting  */
  /* the same channel.                                                        */
  sConfig.Channel = ADCxy_CHANNEL;
  
  if (HAL_ADC_ConfigChannel(&AdcHandle2, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /*##-5- Configuration of multimode #########################################*/
  /* Multimode parameters settings and set ADC2 (slave) under control of      */
  /* ADC1 (master).                                                           */
  mode.Mode = ADC_DUALMODE_INTERL;
  mode.DMAAccessMode = ADC_DMAACCESSMODE_8_6_BITS;
  mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_3CYCLES;
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
  /* Toggle LED1: Conversions results are available */
  /* In the case of this exemple:                                             */
  /* HAL_ADC_ConvCpltCallback() is called when DMA Transfer process is        */
  /* completed.                                                               */
  /* Since ADC and DMA are configured in continuous and circular mode, this   */
  /* function will be called each time the DMA buffer length is reached.      */
  BSP_LED_Toggle(LED1);
}

/**
  * @brief  Conversion DMA half-transfer callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{

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
