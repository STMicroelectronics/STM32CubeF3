/**
  ******************************************************************************
  * @file    ADC/ADC_AnalogWatchdog/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the ADC
  *          peripheral to perform conversions with analog watchdog and 
  *          interruptions. Other peripherals used: DMA, TIM (ADC group regular
  *          conversions triggered by TIM, ADC group regular conversion data
  *          transferred by DMA).
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

/** @addtogroup ADC_AnalogWatchdog
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RANGE_12BITS                   ((uint32_t)4095)    /* Max value with a full range of 12 bits */
#define USERBUTTON_CLICK_COUNT_MAX     ((uint32_t)   4)    /* Maximum value of variable "UserButtonClickCount" */
#define TIMER_FREQUENCY_HZ             ((uint32_t)1000)    /* Timer frequency (unit: Hz). With SysClk set to 72MHz, timer frequency TIMER_FREQUENCY_HZ range is min=1Hz, max=32.757kHz. */

#define ADCCONVERTEDVALUES_BUFFER_SIZE 256                 /* Size of array aADCxConvertedValues[] */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

/* TIM handler declaration */
TIM_HandleTypeDef    TimHandle;

/* DAC handler declaration */
DAC_HandleTypeDef    DacHandle;

/* Variable containing ADC conversions results */
__IO uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];

/* Variables to manage push button on board: interface between ExtLine interruption and main program */
uint8_t         ubUserButtonClickCount = 0;      /* Count number of clicks: Incremented after User Button interrupt */
__IO uint8_t    ubUserButtonClickEvent = RESET;  /* Event detection: Set after User Button interrupt */

/* Variable to report ADC analog watchdog status:   */
/*   RESET <=> voltage into AWD window   */
/*   SET   <=> voltage out of AWD window */
uint8_t         ubAnalogWatchdogStatus = RESET;  /* Set into analog watchdog interrupt callback */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static void ADC_Config(void);
static void TIM_Config(void);
static void DAC_Config(void);

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
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 64 MHz */
  SystemClock_Config();
  
  /*## Configure peripherals #################################################*/
  /* Initialize LED on board */
  BSP_LED_Init(LED2);

  /* Configure User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Configure the ADC peripheral */
  ADC_Config();
  
  /* Run the ADC calibration in single-ended mode */  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

  /* Configure the TIM peripheral */
  TIM_Config();
  
  /* Configure the DAC peripheral */
  DAC_Config();
  

  /*## Enable peripherals ####################################################*/

  /* Timer counter enable */
  if (HAL_TIM_Base_Start(&TimHandle) != HAL_OK)
  {
    /* Counter Enable Error */
    Error_Handler();
  }
  
  /* Set DAC Channel data register: channel corresponding to ADC channel CHANNELa */
  /* Set DAC output to 1/2 of full range (4095 <=> Vdda=3.3V): 2048 <=> 1.65V              */
  if (HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL_TO_ADCx_CHANNELa, DAC_ALIGN_12B_R, RANGE_12BITS/2) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }
  
  /* Enable DAC Channel: channel corresponding to ADC channel CHANNELa */
  if (HAL_DAC_Start(&DacHandle, DACx_CHANNEL_TO_ADCx_CHANNELa) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
    
  /*## Start ADC conversions #################################################*/
  
  /* Start ADC conversion on regular group with transfer by DMA */
  if (HAL_ADC_Start_DMA(&AdcHandle,
                        (uint32_t *)aADCxConvertedValues,
                        ADCCONVERTEDVALUES_BUFFER_SIZE
                       ) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  
  
  /* Infinite loop */
  while (1)
  {
    /* Turn-on/off LED2 in function of ADC conversion result */
    /*  - Turn-off if voltage is into AWD window */ 
    /*  - Turn-on if voltage is out of AWD window */

    /* Variable of analog watchdog status is set into analog watchdog         */
    /* interrupt callback                                                     */
    if (ubAnalogWatchdogStatus == RESET)
    {
      BSP_LED_Off(LED2);
    }
    else
    {
      BSP_LED_On(LED2);
      
      /* Reset analog watchdog status for next loop iteration */
      ubAnalogWatchdogStatus = RESET;
    }
  
    /* For information: ADC conversion results are stored into array          */
    /* "aADCxConvertedValues" (for debug: check into watch window)            */
  
  
    /* Wait for event on push button to perform following actions */
    while ((ubUserButtonClickEvent) == RESET)
    {
      __NOP();
    }
    /* Reset variable for next loop iteration */
    ubUserButtonClickEvent = RESET;

    /* Set DAC voltage on channel corresponding to ADCx_CHANNELa              */
    /* in function of user button clicks count.                               */
    /* Set DAC output successively to:                                        */
    /*  - minimum of full range (0 <=> ground 0V)                             */
    /*  - 1/4 of full range (4095 <=> Vdda=3.3V): 1023 <=> 0.825V             */
    /*  - 1/2 of full range (4095 <=> Vdda=3.3V): 2048 <=> 1.65V              */
    /*  - 3/4 of full range (4095 <=> Vdda=3.3V): 3071 <=> 2.475V             */
    /*  - maximum of full range (4095 <=> Vdda=3.3V)                          */
    if (HAL_DAC_SetValue(&DacHandle,
                         DACx_CHANNEL_TO_ADCx_CHANNELa,
                         DAC_ALIGN_12B_R,
                         (RANGE_12BITS * ubUserButtonClickCount / USERBUTTON_CLICK_COUNT_MAX)
                        ) != HAL_OK)
    {
      /* Start Error */
      Error_Handler();
    }
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 8000000
  *            PREDIV                         = RCC_PREDIV_DIV2 (2)
  *            PLLMUL                         = RCC_PLL_MUL16 (16)
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* HSI Oscillator already ON after system reset, activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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
  * @brief  ADC configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  ADC_AnalogWDGConfTypeDef AnalogWDGConfig;
  
  /* Configuration of ADCx init structure: ADC parameters and regular group */
  AdcHandle.Instance = ADCx;

  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_Tx_TRGO;  /* Trig of conversion start done by external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
  AdcHandle.Init.DMAContinuousRequests = ENABLE;
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
 
  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  /* Note: Considering IT occurring after each ADC conversion if ADC          */
  /*       conversion is out of the analog watchdog widow selected (ADC IT    */
  /*       enabled), select sampling time and ADC clock with sufficient       */
  /*       duration to not create an overhead situation in IRQHandler.        */
  sConfig.Channel      = ADCx_CHANNELa;
  sConfig.Rank         = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
  /* Set analog watchdog thresholds in order to be between steps of DAC       */
  /* voltage.                                                                 */
  /*  - High threshold: between DAC steps 1/2 and 3/4 of full range:          */
  /*                    5/8 of full range (4095 <=> Vdda=3.3V): 2559<=> 2.06V */
  /*  - Low threshold:  between DAC steps 0 and 1/4 of full range:            */
  /*                    1/8 of full range (4095 <=> Vdda=3.3V): 512 <=> 0.41V */

  /* Analog watchdog 1 configuration */
  AnalogWDGConfig.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_ALL_REG;
  AnalogWDGConfig.Channel = ADCx_CHANNELa;
  AnalogWDGConfig.ITMode = ENABLE;
  AnalogWDGConfig.HighThreshold = (RANGE_12BITS * 5/8);
  AnalogWDGConfig.LowThreshold = (RANGE_12BITS * 1/8);
  HAL_ADC_AnalogWDGConfig(&AdcHandle, &AnalogWDGConfig);
  
}

/**
  * @brief  TIM configuration
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;
  
  /* Time Base configuration */
  TimHandle.Instance = TIMx;
  
  /* Configure timer frequency */
  /* Note: Setting of timer prescaler to 1099 to increase the maximum range   */
  /*       of the timer, to fit within timer range of 0xFFFF.                 */
  /*       Setting of reload period to SysClk/1099 to maintain a base         */
  /*       frequency of 1us.                                                  */
  /*       With SysClk set to 72MHz, timer frequency (defined by label        */
  /*       TIMER_FREQUENCY_HZ range) is min=1Hz, max=32.757kHz.               */
  /* Note: Timer clock source frequency is retrieved with function            */
  /*       HAL_RCC_GetPCLK1Freq().                                            */
  /*       Alternate possibility, depending on prescaler settings:            */
  /*       use variable "SystemCoreClock" holding HCLK frequency, updated by  */
  /*       function HAL_RCC_ClockConfig().                                    */
  TimHandle.Init.Period = ((HAL_RCC_GetPCLK1Freq() / (1099 * TIMER_FREQUENCY_HZ)) - 1);
  TimHandle.Init.Prescaler = (1099-1);
  TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0x0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Timer initialization Error */
    Error_Handler();
  }

  /* Timer TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&TimHandle, &sMasterConfig) != HAL_OK)
  {
    /* Timer TRGO selection Error */
    Error_Handler();
  }
  
}

/**
  * @brief  DAC configuration
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
  static DAC_ChannelConfTypeDef sConfig;

  /* Configuration of DACx peripheral */
  DacHandle.Instance = DACx;

  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* DAC initialization error */
    Error_Handler();
  }

  /* Configuration of DACx channel 1 */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL_TO_ADCx_CHANNELa) != HAL_OK)
  {
    /* Channel configuration error */
    Error_Handler();
  }
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == USER_BUTTON_PIN)
  {
    /* Set variable to report push button event to main program */
    ubUserButtonClickEvent = SET;
  
    /* Manage ubUserButtonClickCount to increment it circularly from 0 to     */
    /* maximum value defined                                                  */
    if (ubUserButtonClickCount < USERBUTTON_CLICK_COUNT_MAX)
    {
      ubUserButtonClickCount++;
    }      
    else
    {
      ubUserButtonClickCount=0;
    }
    
  }
}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{

}

/**
  * @brief  Conversion DMA half-transfer callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{

}
  
/**
  * @brief  Analog watchdog callback in non blocking mode. 
  * @note:  In case of several analog watchdog enabled, if needed to know
            which one triggered and on which ADCx, check Analog Watchdog flag
            ADC_FLAG_AWD1/2/3 into HAL_ADC_LevelOutOfWindowCallback() function.
            For example:"if (__HAL_ADC_GET_FLAG(hadc1, ADC_FLAG_AWD1) != RESET)"
  * @param  hadc: ADC handle
  * @retval None
  */
  void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
  /* Set variable to report analog watchdog out of window status to main      */
  /* program.                                                                 */
  ubAnalogWatchdogStatus = SET;
}

/**
  * @brief  ADC error callback in non blocking mode
  *        (ADC conversion with interruption or transfer by DMA)
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* In case of ADC error, call main error handler */
  Error_Handler();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with a potential error */
  
  /* In case of error, LED2 is toggling at a frequency of 1Hz */
  while(1)
  {
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
    HAL_Delay(500);
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
