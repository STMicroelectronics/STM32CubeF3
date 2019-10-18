/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_DualBuck/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to control two buck converters in 
  * parallel using the HRTIM and a single timing unit
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

/** @addtogroup HRTIM_DualBuck
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t ADCReadout;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

static void GPIO_HRTIM_outputs_Config(void);
static void HRTIM_Config(void);
static void ADC_Config(void);

/* Private functions ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
HRTIM_HandleTypeDef hhrtim;
ADC_HandleTypeDef AdcHandle;

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
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to have a system clock = 72 MHz */
  SystemClock_Config();

  /* Initialize STM32F3348-DISCO LEDs */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED6);

  /* Initialize User_Button on STM32F3348-DISCO */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);   

  /* Initialize ADC to be triggered by the HRTIMER */
  ADC_Config();
  
  /* Initialize HRTIM and related inputs */
  HRTIM_Config();
  /* Initialize HRTIM outputs (it has to be done after HRTIM init) */
  GPIO_HRTIM_outputs_Config();
  
  /* Infinite loop */
  while (1)
  {
    
    /* ---------------- */
    /* Fault management */
    /* ---------------- */
    while(__HAL_HRTIM_GET_FLAG(&hhrtim, HRTIM_FLAG_FLT1) == SET)
    {
      /* LED3 is flashing in case of fault */
      BSP_LED_On(LED3);
      HAL_Delay(20);
      BSP_LED_Off(LED3);
      HAL_Delay(80);
      
      /* Re-arm HRTIM TD1 output if "User" push button is pressed*/
      if((BSP_PB_GetState(BUTTON_USER) == SET))
      {
          __HAL_HRTIM_CLEAR_IT(&hhrtim, HRTIM_IT_FLT1);
          HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TD1);
      }
    }

    /* ---------------- */
    /* Normal operation */
    /* ---------------- */
    /* LED6 toggling to show MCU activity */
    BSP_LED_On(LED6);
    HAL_Delay(100);
    BSP_LED_Off(LED6);
    HAL_Delay(400);
    
    /* -------------------------------------------------------------------- */
    /* ADC monitoring (to be viewed with watch)                             */
    /* PA4 can be connected to the RC filter or any voltage to be monitored */
    /* -------------------------------------------------------------------- */
    ADCReadout = HAL_ADCEx_InjectedGetValue(&AdcHandle, ADC_INJECTED_RANK_1);
    
  }
}


/**
* @brief  Configure GPIO outputs for the HRTIM
* @param  None
* @retval None
*/
static void GPIO_HRTIM_outputs_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIOB clock for timer D outputs */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure HRTIM output: TD1 (PB14) */
  GPIO_InitStruct.Pin = GPIO_PIN_14; 
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;;  
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;;  
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Configure HRTIM output: TD2 (PB15) */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}


/**
* @brief  HRTIM configuration
* @param  None
* @retval None
*/
static void HRTIM_Config(void)
{
  HRTIM_TimeBaseCfgTypeDef timebase_config;
  HRTIM_TimerCfgTypeDef timer_config;
  HRTIM_OutputCfgTypeDef output_config_TD1;
  HRTIM_OutputCfgTypeDef output_config_TD2;
  HRTIM_CompareCfgTypeDef compare_config;
  HRTIM_ADCTriggerCfgTypeDef adc_trigger_config;
  HRTIM_FaultCfgTypeDef fault_config;

  /* ----------------------------*/
  /* HRTIM Global initialization */
  /* ----------------------------*/
  /* Initialize the HRTIM structure (minimal configuration) */
  hhrtim.Instance = HRTIM1;
  hhrtim.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
  hhrtim.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;

  /* Initialize HRTIM */
  HAL_HRTIM_Init(&hhrtim);

  /* HRTIM DLL calibration: periodic calibration, set period to 14µs */
  HAL_HRTIM_DLLCalibrationStart(&hhrtim, HRTIM_CALIBRATIONRATE_14);
  /* Wait calibration completion*/
  if (HAL_HRTIM_PollForDLLCalibration(&hhrtim, 100) != HAL_OK)
  {
    Error_Handler(); // if DLL or clock is not correctly set
  }
  
  /* --------------------------------------------------- */
  /* TIMERD initialization: timer mode and PWM frequency */
  /* --------------------------------------------------- */
  timebase_config.Period = BUCK_PWM_PERIOD;// 100kHz switching frequency
  timebase_config.RepetitionCounter = 3;
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, &timebase_config);

  
  /* --------------------------------------------------------------------- */
  /* TIMERD global configuration: cnt reset, sync, update, fault, burst... */
  /* --------------------------------------------------------------------- */
  timer_config.DMARequests = HRTIM_TIM_DMA_NONE;
  timer_config.DMASrcAddress = 0x0;
  timer_config.DMADstAddress = 0x0;
  timer_config.DMASize = 0x0;
  timer_config.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
  timer_config.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  timer_config.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  timer_config.DACSynchro = HRTIM_DACSYNC_NONE;
  timer_config.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  timer_config.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  timer_config.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  timer_config.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
  timer_config.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  timer_config.InterruptRequests = HRTIM_TIM_IT_REP;
  timer_config.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  timer_config.FaultEnable = HRTIM_TIMFAULTENABLE_FAULT1;
  timer_config.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  timer_config.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  timer_config.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  timer_config.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D,&timer_config);
    
  /* --------------------------------- */
  /* TD1 and TD2 waveforms description */
  /* --------------------------------- */
  output_config_TD1.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config_TD1.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config_TD1.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  output_config_TD1.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config_TD1.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config_TD1.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config_TD1.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config_TD1.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD1,
                                 &output_config_TD1);

  output_config_TD2.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config_TD2.SetSource = HRTIM_OUTPUTSET_TIMCMP2;
  output_config_TD2.ResetSource  = HRTIM_OUTPUTRESET_TIMPER;
  output_config_TD2.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config_TD2.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config_TD2.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE;
  output_config_TD2.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config_TD2.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD2,
                                 &output_config_TD2);

  /* Set compare registers for duty cycle on TD1 */
  compare_config.CompareValue = BUCK_PWM_PERIOD/20;     // 5% duty cycle
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* Set compare registers for duty cycle on TD2 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = BUCK_PWM_PERIOD - (BUCK_PWM_PERIOD/3); // 33% duty cycle (NB: from CMP2 to period)
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);


  /* --------------------------------------------------- */
  /* ADC trigger intialization (CMP3 and CMP4 registers) */
  /* --------------------------------------------------- */
  /* Set compare 3 registers for 1st ADC trigger */
  compare_config.CompareValue = BUCK_PWM_PERIOD/10; // samples in middle of duty cycle (NB: from CMP2 to period)
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  /* Set compare 4 registers for 2nd ADC trigger */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (BUCK_PWM_PERIOD - ((BUCK_PWM_PERIOD/3)/2)); // samples in middle of duty cycle
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);

  adc_trigger_config.Trigger = HRTIM_ADCTRIGGEREVENT24_TIMERD_CMP3 + HRTIM_ADCTRIGGEREVENT24_TIMERD_CMP4;
  adc_trigger_config.UpdateSource = HRTIM_ADCTRIGGERUPDATE_TIMER_D;
  HAL_HRTIM_ADCTriggerConfig(&hhrtim,
                             HRTIM_ADCTRIGGER_2,
                             &adc_trigger_config);


  /* ---------------------*/
  /* FAULT initialization */
  /* ---------------------*/
  fault_config.Filter = HRTIM_FAULTFILTER_NONE;
  fault_config.Lock = HRTIM_FAULTLOCK_READWRITE;
  fault_config.Polarity = HRTIM_FAULTPOLARITY_LOW;
  fault_config.Source = HRTIM_FAULTSOURCE_DIGITALINPUT;
  HAL_HRTIM_FaultConfig(&hhrtim,
                        HRTIM_FAULT_1,
                        &fault_config);

  HAL_HRTIM_FaultModeCtl(&hhrtim,
                        HRTIM_FAULT_1,
                        HRTIM_FAULTMODECTL_ENABLED);

  /* ---------------*/
  /* HRTIM start-up */
  /* ---------------*/
  /* Enable HRTIM's outputs TD1 and TD2 */
  /* Note: it is necessary to enable also GPIOs to have outputs functional */
  /* This must be done after HRTIM initialization */
  HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2); 

  /* Start HRTIM's TIMER D*/
  HAL_HRTIM_WaveformCountStart_IT(&hhrtim, HRTIM_TIMERID_TIMER_D);

}


/**
  * @brief  Configure ADC2 for being used with HRTIM
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_MultiModeTypeDef MultiModeConfig;
  ADC_InjectionConfTypeDef InjectionConfig;

  AdcHandle.Instance = ADC2;

  /* ADC2 is working independently */
  MultiModeConfig.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
  MultiModeConfig.Mode = ADC_MODE_INDEPENDENT;
  MultiModeConfig.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_1CYCLE;
  HAL_ADCEx_MultiModeConfigChannel(&AdcHandle, &MultiModeConfig);
  
  /* ADC2 global initialization */
  /* 12-bit right-aligned format, discontinuous scan mode, running from PLL */
  AdcHandle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode = ENABLE;
  AdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.LowPowerAutoWait = DISABLE;
  AdcHandle.Init.ContinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfConversion = 1;
  AdcHandle.Init.DiscontinuousConvMode = ENABLE;
  AdcHandle.Init.NbrOfDiscConversion = 1;
  AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  AdcHandle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  HAL_ADC_Init(&AdcHandle);

  /* Discontinuous injected mode: ADC ch1 is sampled 2 times per PWM period */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_1;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_1;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  InjectionConfig.InjectedSingleDiff = ADC_SINGLE_ENDED;
  InjectionConfig.InjectedOffsetNumber = ADC_OFFSET_NONE;
  InjectionConfig.InjectedOffset = 0;
  InjectionConfig.InjectedNbrOfConversion = 2;
  InjectionConfig.InjectedDiscontinuousConvMode = ENABLE;
  InjectionConfig.AutoInjectedConv = DISABLE;
  InjectionConfig.QueueInjectedContext = DISABLE;
  InjectionConfig.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_HRTIM_TRG2;
  InjectionConfig.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectionConfig);

  /* Configure the 2nd injected conversion (all other parameters are identical) */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_1;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_2;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectionConfig);
  
  /* Run the ADC calibration in single-ended mode */
  HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED);

  /* Start ADC2 Injected Conversions */ 
  HAL_ADCEx_InjectedStart(&AdcHandle);
  
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
  /* Turn LED3 (RED) on */
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
