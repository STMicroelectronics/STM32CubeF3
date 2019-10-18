/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_BuckBoost_AN4449/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes the Buck/Boost converter application of 
  *          STM32F334 Discovery
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

/** @addtogroup HRTIM_BuckBoost_AN4449
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEBUG // for debug purpose only (check comments for use)
#define RANGE_MONITORING_ON // check if the appropriate Vin or Vout voltages are inside range during operation (3V min.; 15V max.)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static int32_t Kp;
static int32_t Ki;
static int32_t Int_term_Buck;
static int32_t Int_term_Boost;
static int32_t Int_term_Mixed;
static uint32_t VoutDelta; 

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void GPIO_BkBt_HW_Config(void);
static void Vout_Check(void);
static void HRTIM_Config(void);
static void ADC_Config(void);
static void SetHRTIM_IdleMode(void);
static void SetHRTIM_NoOutput(void);
static void StopConverter(Led_TypeDef LEDactive);
static void Reset_PI(void);
static void EXTI0_IRQHandler_Config(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/* Private functions ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/




/**
* @brief  Main program.
* @param  None
* @retval None
*/
int main(void)
{
  /* Local variable used to prevent compilation warning */
  uint32_t lVoutT = VoutT;

  /* STM32F3xx HAL library initialization:
  - Configure the Flash prefetch
  - Configure the Systick to generate an interrupt each 1 msec
  - Set NVIC Group Priority to 4
  - Global MSP (MCU Support Package) initialization
  */

  HAL_Init();

  /* Configure the system clock to have a system clock = 72 MHz */
  SystemClock_Config();

  /* Configure EXTI_Line0 (connected to PA0 pin) in interrupt mode */
  EXTI0_IRQHandler_Config();

  /* Reset Proportional Integral parameters */
  Reset_PI();
  
  /* Reset variables that control the converter operating mode */
  Idle2Buck = RESET;
  Idle2Boost = RESET; 
  Run2Stop = RESET;
  CTRange = 0;

  /* Initialize 32F3348DISCOVERY LEDs */
  BSP_LED_Init(LED3); //RED PB6
  BSP_LED_Init(LED4); //ORANGE PB8
  BSP_LED_Init(LED5); //GREEN PB9
  BSP_LED_Init(LED6); //BLUE PB7

  /* Initialize User_Button on STM32F3348-DISCO */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* Computes parameters determined by VOUT_TARGET user settings*/
  Vout_Check();

  /* Initialize ADC to be triggered by the HRTIMER */
  ADC_Config();

  /* Initialize HRTIM and related inputs */
  HRTIM_Config();

  /* Initialize BUCK-BOOST hardware (it has to be done after HRTIM init) */
  GPIO_BkBt_HW_Config();

  /* Set default HRTIM Idle mode active */
  SetHRTIM_IdleMode();

#if defined(DEBUG)
  /* Enable Timer D interrupt */
  /* Timer D is here dedicated to observe ADC trigger event on TD1 (for debug purpose only) */
  HAL_HRTIM_WaveformCountStart_IT(&hhrtim, HRTIM_TIMERID_TIMER_D);
#endif //DEBUG

  HAL_Delay(100);
  /* Determine roughly which is the appropriate mode to start the converter */
  /* Vin > Vout */
  if (VinConversion >= lVoutT)
  {
    /* Ask Idle to buck mode change */
    Idle2Buck = SET;
  }
  else
    /* Vin < Vout */
  {
    /* Ask Idle to boost mode change */
    Idle2Boost = SET;
  }

  /* Infinite loop */
  while (1)
  {
    /* Run2Stop flag has been activated during HRTIM TIMERA interrupt to stop the converter */
    if (Run2Stop == SET)
    {
      /* Stops the converter and highlights failure on LD3 (red) */
      StopConverter(LED3);
    }

#if defined(RANGE_MONITORING_ON)
    /* Check Vin and Vout inside operating conditions min. 3Vdc and max. 15Vdc */
    if ((VinConversion < VINMIN) || (VinConversion >= VINMAX) || (VoutConversion < VOUTMIN) || (VoutConversion >= VOUTMAX))
    {
      /* if Vin or Vout out of the operating conditions CPTRange is incremented */
      CTRange++;
    }
    else
      /* else decremented */
    {
      if (CTRange != 0)
      {
        CTRange--;
      }
    }
    /* Check if CPTRange reaches the maximum value */
    if (CTRange >= MAX_RANGE)
    {
      /* then stops the converter and highlights failure on LD4 (orange)*/
      StopConverter(LED4);
    }
#endif //RANGE_MONITORING_ON

  }
}


/**
* @brief  Configure GPIO for BUCK/BOOST converter
* @param  None
* @retval None
*/
static void GPIO_BkBt_HW_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIOA/GPIOB clock for timer A, B, D outputs */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure HRTIM output: TA1 (PA8) */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure HRTIM output: TA2 (PA9) */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure HRTIM output: TB1 (PA10) */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure HRTIM output: TB2 (PA11) */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

#if defined(DEBUG)
  /* Configure HRTIM output: TD1 (PB14) */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Configure HRTIM output: TD2 (PB15) */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

#endif //DEBUG

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
  HRTIM_CompareCfgTypeDef compare_config;
  HRTIM_DeadTimeCfgTypeDef HRTIM_TIM_DeadTimeConfig;
  HRTIM_ADCTriggerCfgTypeDef adc_trigger_config;
  HRTIM_FaultCfgTypeDef fault_config;
  HRTIM_OutputCfgTypeDef output_config;

  /* ----------------------------*/
  /* HRTIM Global initialization */
  /* ----------------------------*/
  /* Initialize the HRTIM handle (and clear it before for greater portability) */
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

  /* -------------------------------------------------------------- */
  /* TIMERA and TIMERB initialization: timer mode and PWM frequency */
  /* The 2 timers are running with identical parameters.            */
  /* -------------------------------------------------------------- */
  timebase_config.Period = BUCK_PWM_PERIOD; /* 250kHz switching frequency */
  timebase_config.RepetitionCounter = 7; /* 1 ISR every 8 PWM periods */
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &timebase_config);
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, &timebase_config);

#if defined(DEBUG)
  timebase_config.RepetitionCounter = 0; /* 1 ISR every 1 period*/
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, &timebase_config);

#endif //DEBUG

  /* --------------------------------------------------------------------- */
  /* TIMERA global configuration: cnt reset, sync, update, fault, burst... */
  /* timer running in continuous mode, with deadtime enabled               */
  /* Very same configuration for Timer B                                   */
  /* --------------------------------------------------------------------- */
  timer_config.DMARequests = HRTIM_TIM_DMA_NONE;  
  timer_config.DMASrcAddress = 0x0;
  timer_config.DMADstAddress = 0x0;
  timer_config.DMASize = 0x1; 
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
  timer_config.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_ENABLED;
  timer_config.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  timer_config.UpdateTrigger = HRTIM_TIMUPDATETRIGGER_NONE;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;

  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &timer_config);
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, &timer_config);
#if defined(DEBUG)
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, &timer_config);
#endif //DEBUG

  /* Set compare CMP1 registers for duty cycle on TA1 (CurrentDutyA) */
  compare_config.CompareValue = BUCK_PWM_PERIOD / 4;  /* default 25% duty cycle */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, &compare_config);

  /* Set compare CMP1 registers for duty cycle on TB1 (CurrentDutyB) */
  /* default 25% duty cycle as done for TA1/CMP1 */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, &compare_config);

  /* Set compare CMP2 registers for duty cycle on TB1 (CurrentDutyB Mixed)*/
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  /* default 25% duty cycle as done for TB1/CMP2*/
  HAL_HRTIM_WaveformCompareConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_2, &compare_config);

  /* Deadtime configuration for Timer A */
  HRTIM_TIM_DeadTimeConfig.FallingLock = HRTIM_TIMDEADTIME_FALLINGLOCK_WRITE;
  HRTIM_TIM_DeadTimeConfig.FallingSign = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE;
  HRTIM_TIM_DeadTimeConfig.FallingSignLock = HRTIM_TIMDEADTIME_FALLINGSIGNLOCK_READONLY;
  HRTIM_TIM_DeadTimeConfig.FallingValue = DT_FALLING;
  HRTIM_TIM_DeadTimeConfig.Prescaler = HRTIM_TIMDEADTIME_PRESCALERRATIO_MUL8;
  HRTIM_TIM_DeadTimeConfig.RisingLock = HRTIM_TIMDEADTIME_RISINGLOCK_WRITE;
  HRTIM_TIM_DeadTimeConfig.RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE;
  HRTIM_TIM_DeadTimeConfig.RisingSignLock = HRTIM_TIMDEADTIME_RISINGSIGNLOCK_READONLY;
  HRTIM_TIM_DeadTimeConfig.RisingValue = DT_RISING;
  HAL_HRTIM_DeadTimeConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &HRTIM_TIM_DeadTimeConfig);

  /* Same parameters for Timer B */
  HAL_HRTIM_DeadTimeConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, &HRTIM_TIM_DeadTimeConfig);

  /* ------------------------------------------- */
  /* ADC trigger intialization (with CMP2 event) */
  /* ------------------------------------------- */
  compare_config.CompareValue = BUCK_PWM_PERIOD / 8 ; /* Set default ADC trigger event */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_4, &compare_config);

  adc_trigger_config.Trigger = HRTIM_ADCTRIGGEREVENT24_TIMERA_CMP4;
  adc_trigger_config.UpdateSource = HRTIM_ADCTRIGGERUPDATE_TIMER_A;
  HAL_HRTIM_ADCTriggerConfig(&hhrtim, HRTIM_ADCTRIGGER_2, &adc_trigger_config);

#if defined(DEBUG)
  /* --------------------------------- */
  /* TD1 waveform description */
  /* --------------------------------- */
  /* This is to observe the precise timing of ADC trigger event on TD1 ouput */
  output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMEV_2;
  output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1, &output_config);
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD2, &output_config);
#endif //DEBUG

  /* ---------------------*/
  /* FAULT initialization */
  /* ---------------------*/
  fault_config.Filter = HRTIM_FAULTFILTER_NONE;
  fault_config.Lock = HRTIM_FAULTLOCK_READWRITE;
  fault_config.Polarity = HRTIM_FAULTPOLARITY_LOW;
  fault_config.Source = HRTIM_FAULTSOURCE_DIGITALINPUT;
  HAL_HRTIM_FaultConfig(&hhrtim, HRTIM_FAULT_1, &fault_config);
  HAL_HRTIM_FaultModeCtl(&hhrtim, HRTIM_FAULT_1, HRTIM_FAULTMODECTL_ENABLED);

  /* ---------------*/
  /* HRTIM start-up */
  /* ---------------*/
  /* Enable HRTIM's outputs TA1, TA2, TB1 and TB2 */
  /* Note: it is necessary to enable also GPIOs to have outputs functional */
  /* This must be done after HRTIM initialization */
  HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2);

#if defined(DEBUG)
  /* Enable HRTIM's outputs TA1, TA2, TB1, TB2, TD1 and TD2 */
  HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2 | HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2);
#endif //DEBUG

}


/**
* @brief  Configure ADC1 for being used with HRTIM
* @param  None
* @retval None
*/
static void ADC_Config(void)
{
  ADC_MultiModeTypeDef MultiModeConfig;
  ADC_InjectionConfTypeDef InjectionConfig;

  AdcHandle.Instance = ADC1;

  /* ADC2 is working independently */
  MultiModeConfig.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
  MultiModeConfig.Mode = ADC_MODE_INDEPENDENT;
  MultiModeConfig.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_1CYCLE;
  HAL_ADCEx_MultiModeConfigChannel(&AdcHandle, &MultiModeConfig);

  /* ADC1 global initialization */
  /* 12-bit right-aligned format, discontinuous scan mode, running from PLL */
  AdcHandle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode = ENABLE;
  AdcHandle.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  AdcHandle.Init.LowPowerAutoWait = DISABLE;
  AdcHandle.Init.ContinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfConversion = 1;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfDiscConversion = 1;
  AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  AdcHandle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  HAL_ADC_Init(&AdcHandle);

  /* Discontinuous injected mode: 1st injected conversion for Vin on Ch2 */
  InjectionConfig.AutoInjectedConv = DISABLE;
  InjectionConfig.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_HRTIM_TRG2;
  InjectionConfig.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
  InjectionConfig.InjectedChannel = ADC_CHANNEL_2;
  InjectionConfig.InjectedDiscontinuousConvMode = DISABLE;
  InjectionConfig.InjectedNbrOfConversion = 2;
  InjectionConfig.InjectedOffset = 0;
  InjectionConfig.InjectedOffsetNumber = ADC_OFFSET_NONE;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_1;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_7CYCLES_5;
  InjectionConfig.InjectedSingleDiff = ADC_SINGLE_ENDED;
  InjectionConfig.QueueInjectedContext = DISABLE;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectionConfig);

  /* Configure the 2nd injected conversion for Vout on Ch4 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_4;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_2;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_19CYCLES_5;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectionConfig);

  /* Configure and enable ADC1_2_IRQHandler interrupt channel in NVIC */
  HAL_NVIC_SetPriority(ADC1_2_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

  /* Run the ADC calibration in single-ended mode */
  HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED);

  /* Start ADC1 Injected Conversions */
  HAL_ADCEx_InjectedStart_IT(&AdcHandle);

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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
* @brief  This function sets the HRTIM in Idle configuration mode.
* @param  None
* @retval None
*/
static void SetHRTIM_IdleMode(void)
{
  HRTIM_OutputCfgTypeDef output_config;
  Reset_PI();
  /* Reset all active output settings */
  SetHRTIM_NoOutput();
  /* --------------------------------- */
  /* TA1,TB1 and TA2,TB2 waveforms description */
  /* --------------------------------- */
  output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config.SetSource = HRTIM_OUTPUTSET_NONE;
  output_config.ResetSource  = HRTIM_OUTPUTSET_NONE;
  output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &output_config);
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA2, &output_config);
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB1, &output_config);
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB2, &output_config);

  /* Start both HRTIM TIMER A, B and D*/
  HAL_HRTIM_WaveformCountStart_IT(&hhrtim, HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_D);
  Mode = BUCKBOOST_MODE_IDLE;
}


/**
* @brief  This function sets the HRTIM in Buck configuration mode.
* @param  None
* @retval None
*/
void SetHRTIM_BuckMode(void)
{
  HRTIM_OutputCfgTypeDef output_config;
  Reset_PI();
  /* Reset all active output settings */
  SetHRTIM_NoOutput();
  /* --------------------------------- */
  /* TA1 and TA2 waveforms description */
  /* --------------------------------- */
  output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &output_config);
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA2, &output_config);

  /* --------------------------------- */
  /* TB1 and TB2 waveforms description */
  /* --------------------------------- */
  output_config.SetSource = HRTIM_OUTPUTSET_NONE;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMPER;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB1, &output_config);

  output_config.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_NONE;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB2, &output_config);

  /* Start both HRTIM TIMER A, B and D */
  HAL_HRTIM_WaveformCountStart_IT(&hhrtim, HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_D);
  __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, BUCK_PWM_PERIOD / 2);// Duty cycle update
  Mode = BUCKBOOST_MODE_BUCK;
}


/**
* @brief  This function sets the HRTIM in Boost configuration mode.
* @param  None
* @retval None
*/
void SetHRTIM_BoostMode(void)
{
  HRTIM_OutputCfgTypeDef output_config;
  Reset_PI();
  /* Reset all active output settings */
  SetHRTIM_NoOutput();
  /* --------------------------------- */
  /* TB1 and TB2 waveforms description */
  /* --------------------------------- */
  output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config.ResetSource  = HRTIM_OUTPUTSET_TIMCMP1;
  output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB1, &output_config);
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB2, &output_config);

  /* --------------------------------- */
  /* TA1 and TA2 waveforms description */
  /* --------------------------------- */
  output_config.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_NONE;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &output_config);

  output_config.SetSource = HRTIM_OUTPUTSET_NONE;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMPER;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA2, &output_config);

  /* Start both HRTIM TIMER A and B */
  HAL_HRTIM_WaveformCountStart_IT(&hhrtim, HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_D);
  __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1,  BUCK_PWM_PERIOD / 2);// Duty cycle update
  Mode = BUCKBOOST_MODE_BOOST;
}


/**
* @brief  This function sets the HRTIM in Mixed configuration mode (Buck/Boost).
* @param  None
* @retval None
*/
void SetHRTIM_MixedMode(void)
{
  HRTIM_OutputCfgTypeDef output_config;
  Reset_PI();
  /* Reset all active output settings */
  SetHRTIM_NoOutput();
  /* --------------------------------- */
  /* TA1 and TA2 waveforms description */
  /* --------------------------------- */
  output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &output_config);
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA2, &output_config);

  /* --------------------------------- */
  /* TB1 and TB2 waveforms description */
  /* --------------------------------- */
  output_config.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP2;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB1, &output_config);
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB2, &output_config);

  /* Start both HRTIM TIMER A, B and D */
  HAL_HRTIM_WaveformCountStart_IT(&hhrtim, HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_D);
  __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_2, BUCK_PWM_PERIOD / 4);// Duty cycle update
  Mode = BUCKBOOST_MODE_MIXED;
}


/**
* @brief  This function resets all active HRTIM outputs connected to converter.
* @param  None
* @retval None
*/
static void SetHRTIM_NoOutput(void)
{
  HRTIM_OutputCfgTypeDef output_config;

  output_config.SetSource = HRTIM_OUTPUTSET_NONE;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_NONE;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &output_config);
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA2, &output_config);
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB1, &output_config);
  HAL_HRTIM_WaveformOutputConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB2, &output_config);
}

/**
* @brief  This function sets Vout target parameters
* @param  None
* @retval None
*/
static void Vout_Check(void)
{
  VoutT = (VOUT_TARGET * VOUT_RESISTOR_RATIO) / 10000; /* introduced resistor bridge ratio ~20% */
  VoutT = (VoutT * 0x1000) / REAL_3V3; /* converted to 12-bit ADC full range with practical 3.3V of application */
  /* Check where is located VOUT_TARGET (VoutRange will be used as protection table index) */
  VoutRange = VOUT_TARGET / 1000;
  /* Check for non interger value set for VOUT_TARGET */
  VoutDelta = VOUT_TARGET - (VoutRange * 1000);
  /* Compute factors for linear interpolation based on VOUT_TARGET settings */
  if (VoutDelta != 0 && VoutRange < 15)
  {
    VoutA = ((VoutRange * 1000) + 1000) - VOUT_TARGET;
    VoutB = VOUT_TARGET - (VoutRange * 1000);
  }
  /* and finally apply offset to match protection table where index starts from 0 for 1st 3 volts value */
  VoutRange -= 3;

}

/**
* @brief  This function calculates new duty order with PI.
* @param  None
* @retval New duty order
*/
int32_t PI_Buck(void)
{
  /* Local variable used to prevent compilation warning */
  uint32_t lVoutT = VoutT;
  
  /* Compute PI for Buck Mode */
  /* Every time the PI order sets extreme values then CTMax or CTMin are managed */
  int32_t seterr, pid_out;
  int32_t error;

  error = (int32_t ) VoutConversion - (int32_t) lVoutT;
  seterr = (-Kp * error) / 200;

  Int_term_Buck = Int_term_Buck + ((-Ki * error) / 200);

  if (Int_term_Buck > SAT_LIMIT)
  {
    Int_term_Buck = SAT_LIMIT;
  }
  if (Int_term_Buck < -(SAT_LIMIT))
  {
    Int_term_Buck = -(SAT_LIMIT);
  }
  pid_out = seterr + Int_term_Buck;
  pid_out += BUCK_PWM_PERIOD / 2;

  if (pid_out >= MAX_DUTY_A)
  {
    pid_out = MAX_DUTY_A;
    CTMax++;
  }
  else
  {
    if (CTMax != 0)
    {
      CTMax--;
    }
  }
  if (pid_out <= MIN_DUTY_A)
  {
    pid_out = MIN_DUTY_A;
    CTMin++;
  }
  else
  {
    if (CTMin != 0)
    {
      CTMin--;
    }
  }
  return  pid_out;
}



/**
* @brief  This function calculates new duty order with PI.
* @param  None
* @retval New duty order
*/
int32_t PI_Boost(void)
{
  /* Local variable used to prevent compilation warning */
  uint32_t lVoutT = VoutT;

  /* Compute PI for Boost Mode */
  /* Every time the PI order sets extreme values then CTMax or CTMin are managed */
  int32_t seterr, pid_out;
  int32_t error;

  error = (int32_t ) VoutConversion - (int32_t) lVoutT;
  seterr = (-Kp * error) / 200;

  Int_term_Boost = Int_term_Boost + ((-Ki * error) / 200);

  if (Int_term_Boost > SAT_LIMIT)
  {
    Int_term_Boost = SAT_LIMIT;
  }
  if (Int_term_Boost < -(SAT_LIMIT))
  {
    Int_term_Boost = -(SAT_LIMIT);
  }
  pid_out = seterr + Int_term_Boost;
  pid_out += BUCK_PWM_PERIOD / 2;

  if (pid_out >= MAX_DUTY_B)
  {
    pid_out = MAX_DUTY_B;
    CTMax++;
  }
  else
  {
    if (CTMax != 0)
    {
      CTMax--;
    }
  }
  if (pid_out <= MIN_DUTY_B)
  {
    pid_out = MIN_DUTY_B;
    CTMin++;
  }
  else
  {
    if (CTMin != 0)
    {
      CTMin--;
    }
  }
  return  pid_out;
}


/**
* @brief  This function calculates new duty order with PI.
* @param  None
* @retval New duty order
*/
int32_t PI_Mixed(void)
{
  /* Local variable used to prevent compilation warning */
  uint32_t lVoutT = VoutT;

  /* Compute PI for Mixed Mode */
  /* Every time the PI order sets extreme values then CTMax or CTMin are managed */
  int32_t seterr, pid_out;
  int32_t error;

  error = (int32_t ) VoutConversion - (int32_t) lVoutT;
  seterr = (-Kp * error) / 200;

  Int_term_Mixed = Int_term_Mixed + ((-Ki * error) / 200);

  if (Int_term_Mixed > SAT_LIMIT)
  {
    Int_term_Mixed = SAT_LIMIT;
  }
  if (Int_term_Mixed < -(SAT_LIMIT))
  {
    Int_term_Mixed = -(SAT_LIMIT);
  }
  pid_out = seterr + Int_term_Mixed;
  pid_out += BUCK_PWM_PERIOD / 2;

  if (pid_out >= MAX_DUTY_B_MIXED)
  {
    pid_out = MAX_DUTY_B_MIXED;
    CTMax++;
  }
  else
  {
    if (CTMax != 0)
    {
      CTMax--;
    }
  }
  if (pid_out <= MIN_DUTY_B_MIXED)
  {
    pid_out = MIN_DUTY_B_MIXED;
    CTMin++;
  }
  else
  {
    if (CTMin != 0)
    {
      CTMin--;
    }
  }
  return  pid_out;
}


/**
* @brief  Configures EXTI line 0 (connected to PA.00 pin) in interrupt mode
* @param  None
* @retval None
*/
static void EXTI0_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable and set EXTI line 0 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/**
* @brief EXTI line detection callbacks
* @param GPIO_Pin: Specifies the pins connected EXTI line
* @retval None
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_0)
  {
    /* Insert your code here */
  }
}


/**
* @brief  This function initializes or resets the PI parameters when switching to another mode
* @param  None
* @retval None
*/
static void Reset_PI(void)
{
  /* Reset integral terms for PI */
  Int_term_Buck = 0;
  Int_term_Boost = 0;
  Int_term_Mixed = 0;
  /* Reset CountS Min and Max */
  CTMax = 0;
  CTMin = 0;
  /* Set Proportional and Integral constant terms*/
  Ki = 50;
  Kp = 50;
}

/**
* @brief  This function stops the converter and related HRTIM IT then activates signal LEDs
* @param  None
* @retval None
*/
static void StopConverter(Led_TypeDef LEDactive)
{
  /* This function is used whenever the converter is overloaded or when the appropriate Vin conditions are not applied */
  HAL_HRTIM_WaveformOutputStop(&hhrtim, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2 | HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2);
  HAL_HRTIM_WaveformCountStop_IT(&hhrtim, HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_D);
  /* All LEDs OFF except active LED */
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
  while (1)
  {
    BSP_LED_On(LEDactive);
    HAL_Delay(100);
    BSP_LED_Off(LEDactive);
    HAL_Delay(100);
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
  while (1)
  {}
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
  {}
}
#endif

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
