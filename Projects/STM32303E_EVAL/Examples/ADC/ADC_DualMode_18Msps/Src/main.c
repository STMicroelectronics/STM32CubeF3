/**
  ******************************************************************************
  * @file    ADC/ADC_DualMode_18MSamplesPerSec/Src/main.c
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

/** @addtogroup ADC_DualModeInterleaved_18MSamplesPerSecond
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle_master;
ADC_HandleTypeDef    AdcHandle_slave;

/* Variable containing ADC conversions results */
__IO  uint16_t aADCDualConvertedValue[256];
__IO  uint8_t aADCDualConversionDone = 0;
__IO  uint8_t aADCDualConversionValue = 0;
  
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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


  /*## Configuration of peripherals ##########################################*/
  /* Configure the ADCx and ADCy peripherals */
  ADC_Config();

  /* Run the ADC calibration in single-ended mode */  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle_master, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }
  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle_slave, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }
  
  /*## Enable peripherals ####################################################*/

  /* Enable ADC slave */
  if (HAL_ADC_Start(&AdcHandle_slave) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /*## Start ADC conversions #################################################*/
  
  /* Start ADCx and ADCy multimode conversion with interruption */
  if (HAL_ADCEx_MultiModeStart_DMA(&AdcHandle_master, (uint32_t *)aADCDualConvertedValue, 256) != HAL_OK)
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
    if(aADCDualConversionDone == 1)
    {
      /* Toggle LED1: Conversions results are available                           */
      /* The toggle frequency depends on Conversion Value link to RV2 position    */
      /* In the case of this example:                                             */
      /* HAL_ADC_ConvCpltCallback() is called when DMA Transfer process is        */
      /* completed.                                                               */
      /* Since ADC and DMA are configured in continuous and circular mode, this   */
      /* function will be called each time the DMA buffer length is reached.      */
      BSP_LED_Toggle(LED1);
      HAL_Delay(aADCDualConversionValue*10);
      aADCDualConversionDone = 0;
    }
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
  *            HSE PREDIV                     = RCC_PREDIV_DIV1 (1)
  *            PLLMUL                         = RCC_PLL_MUL9 (9)
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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
  /* User may add here some code to deal with a potential error */
  
  /* In case of error, LED3 is toggling at a frequency of 1Hz */
  while(1)
  {
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);
    HAL_Delay(500);
  }
  
  
}

/**
  * @brief  ADC configuration
  * @note   This function configures the ADC peripheral
            1) Configuration of ADCx peripheral (ADC master)
            2) Configuration of ADCy peripheral (ADC slave)
            3) Configuration of channel on ADCx regular group on rank 1
            4) Configuration of channel on ADCy regular group on rank 1
            5) Configuration of multimode
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef sConfig;
  ADC_MultiModeTypeDef   mode;

  /*##-1- Configuration of ADCx peripheral (ADC master) ######################*/
  /* Configuration of ADCx init structure: ADC parameters and regular group */
  AdcHandle_master.Instance = ADCx;

  AdcHandle_master.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;    /* ADC clock to AHB without prescaler to have maximum frequency 72MHz */
  AdcHandle_master.Init.Resolution = ADC_RESOLUTION_6B;                /* ADC resolution 6 bits to have conversion time = 6.5 ADC clock cycles */
  AdcHandle_master.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle_master.Init.ScanConvMode = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle_master.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  AdcHandle_master.Init.LowPowerAutoWait = DISABLE;
  AdcHandle_master.Init.ContinuousConvMode = ENABLE;                  /* Continuous mode to have maximum conversion speed (no delay between conversions) */
  AdcHandle_master.Init.NbrOfConversion = 1;                          /* Parameter discarded because sequencer is disabled */
  AdcHandle_master.Init.DiscontinuousConvMode = DISABLE;              /* Parameter discarded because sequencer is disabled */
  AdcHandle_master.Init.NbrOfDiscConversion = 1;                      /* Parameter discarded because sequencer is disabled */
  AdcHandle_master.Init.ExternalTrigConv = ADC_SOFTWARE_START;        /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle_master.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle_master.Init.DMAContinuousRequests = ENABLE;               /* ADC-DMA continuous requests to match with DMA in circular mode */
  AdcHandle_master.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;

  if (HAL_ADC_Init(&AdcHandle_master) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configuration of ADCy peripheral (ADC slave) #######################*/
  AdcHandle_slave.Instance = ADCy;

  /* Configuration of ADCy init structure: ADC parameters and regular group */
  /* Same configuration as ADCx */
  AdcHandle_slave.Init = AdcHandle_master.Init;
  
  if (HAL_ADC_Init(&AdcHandle_slave) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-3- Configuration of channel on ADCx regular group on rank 1 ###########*/
  sConfig.Channel = ADCx_CHANNELa;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.Offset = 0;

  if (HAL_ADC_ConfigChannel(&AdcHandle_master, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /*##-4- Configuration of channel on ADCy regular group on rank 1 ###########*/
  /* Same channel as ADCx for dual mode interleaved: both ADC are converting  */
  /* the same channel.                                                        */
  sConfig.Channel = ADCx_CHANNELa;
  
  if (HAL_ADC_ConfigChannel(&AdcHandle_slave, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

  /*##-5- Configuration of multimode #########################################*/
  /* Multimode parameters settings and set ADCy (slave) under control of      */
  /* ADCx (master).                                                           */
  mode.Mode = ADC_DUALMODE_INTERL;
  mode.DMAAccessMode = ADC_DMAACCESSMODE_8_6_BITS;
  mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_3CYCLES;
  if (HAL_ADCEx_MultiModeConfigChannel(&AdcHandle_master, &mode) != HAL_OK)
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
  aADCDualConversionDone = 1;
  aADCDualConversionValue = HAL_ADC_GetValue(hadc);
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
