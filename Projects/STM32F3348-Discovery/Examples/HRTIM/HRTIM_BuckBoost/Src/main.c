/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_BuckBoost/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to control the STM32F334 discovery
  * kit's buck-boost converter with the HRTIM
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

/** @addtogroup HRTIM_BuckBoost
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* BUCK cannot re-start until Vout capacitor is de-energized below this value */
#define DE_ENERGIZING_THRESHOLD         ((uint16_t)2500)

/* BOOST cannot be started if VIN value is above this threshold */
#define OVER_VOLTAGE_PROTECTION         ((uint16_t)5000)

/* This is the VDDA supply in mV */
#define VDDA                            ((uint16_t)3300)

/* Deadtime values for buck and boost stages */
#define DT_RISING       ((uint16_t)230) // 200ns
#define DT_FALLING      ((uint16_t)230) // 200ns

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t Vin, Vout; /* volatile is here for debug convenience */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

static void GPIO_BUCK_BOOST_outputs_Config(void);
static void HRTIM_Config(void);
static void ADC_Config(void);

/* Private functions ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
HRTIM_HandleTypeDef hhrtim;
ADC_HandleTypeDef AdcHandle;
uint32_t DemoState;

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

  /* Configure the system clock to have a system clock = 72 MHz */
  SystemClock_Config();

  /* Initialize STM32F3348-DISCO LEDs */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  /* Initialize User_Button on STM32F3348-DISCO */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);   

  /* Initialize ADC to be triggered by the HRTIMER */
  ADC_Config();

  /* Demo starts by de-energizing output capacitor, should the demo be in boost 
  mode when the MCU is reset */
  DemoState = DE_ENERGIZE;
  
  /* Initialize HRTIM and related inputs */
  HRTIM_Config();
  
  /* Initialize BUCK-BOOST outputs (it has to be done after HRTIM init) */
  GPIO_BUCK_BOOST_outputs_Config();
  
  /* HRTIM start-up */
  /* Enable HRTIM's outputs TA1, TA2, TB1 and TB2 */
  HAL_HRTIM_WaveformOutputStart(&hhrtim,
                                HRTIM_OUTPUT_TA1
                                | HRTIM_OUTPUT_TA2
                                | HRTIM_OUTPUT_TB1
                                | HRTIM_OUTPUT_TB2);

  /* Start HRTIM's TIMER A and B */
  HAL_HRTIM_WaveformCountStart_IT(&hhrtim, HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B);

  
  /* Infinite loop */
  while (1)
  {

    /* ---------------- */
    /* Fault management */
    /* ---------------- */
    if (__HAL_HRTIM_GET_FLAG(&hhrtim, HRTIM_FLAG_FLT1) == SET)
      DemoState = FAULT;

    /* -------------------------------------------------------------------------*/
    /* Input and output voltages measures (can be displayed in a watch window)  */
    /* -------------------------------------------------------------------------*/
    Vin = (HAL_ADCEx_InjectedGetValue(&AdcHandle, ADC_INJECTED_RANK_1) * VDDA)/0xFFF;
    /* VIN bridge conversion is 4.97 (6.8K / 6.8K + 27K) */
    Vin = (497 * Vin )/100; /* Values in mV (for debug convenience) */
 
    Vout = (HAL_ADCEx_InjectedGetValue(&AdcHandle, ADC_INJECTED_RANK_2) * VDDA)/0xFFF;
    /* VOUT bridge conversion is 5.03 (3.3K / 3.3K + 13.3K) */
    Vout = (503 * Vout)/100; /* Values in mV (for debug convenience) */

    /* ---------------- */
    /* Normal operation */
    /* ---------------- */
    switch(DemoState)
    {
    case BUCK:
      /* Duty cycle is modified in the TIMA IRQ */
      /* BOOST cannot be started if VIN value is too high (Vout must not exceed 15V)*/
      if ((BSP_PB_GetState(BUTTON_USER) == SET) && (Vin < OVER_VOLTAGE_PROTECTION))
      {
        DemoState = BOOST;
        while(BSP_PB_GetState(BUTTON_USER) == SET);
      }
      BSP_LED_Toggle(LED5);
      BSP_LED_Off(LED6);
      break;
 
    case BOOST:
      /* Vout capacitor must be de-energized before switching to BUCK mode
      otherwise causing current to flow back into voltage source */
      if (BSP_PB_GetState(BUTTON_USER) == SET)
      {
        DemoState = DE_ENERGIZE;
        while(BSP_PB_GetState(BUTTON_USER) == SET);
      }
      BSP_LED_Toggle(LED6);
      BSP_LED_Off(LED5);
      break;

    case DE_ENERGIZE:
      /* Once Vout is below a safe level (below Vin value), BUCK operation can be started */
      if (Vout < DE_ENERGIZING_THRESHOLD)
      {
        DemoState = BUCK;
      }
      BSP_LED_Off(LED6);
      BSP_LED_Off(LED3);
      break;

    case FAULT:
      /* If fault is triggered, operation acn be re-started with the push-button */
      if((BSP_PB_GetState(BUTTON_USER) == SET))
      {
        __HAL_HRTIM_CLEAR_IT(&hhrtim, HRTIM_IT_FLT1);
        HAL_HRTIM_WaveformOutputStart(&hhrtim,
                                     HRTIM_OUTPUT_TA1
                                   | HRTIM_OUTPUT_TA2
                                   | HRTIM_OUTPUT_TB1
                                   | HRTIM_OUTPUT_TB2);
        DemoState = DE_ENERGIZE;
        while(BSP_PB_GetState(BUTTON_USER) == SET);
      }
      BSP_LED_Toggle(LED3);
      BSP_LED_Off(LED5);
      BSP_LED_Off(LED6);
      break;

    default:
      HAL_HRTIM_WaveformOutputStop(&hhrtim,
                                   HRTIM_OUTPUT_TA1
                                   | HRTIM_OUTPUT_TA2
                                   | HRTIM_OUTPUT_TB1
                                   | HRTIM_OUTPUT_TB2);
      break;
    }

    /* This delay defines the main loop time and LED blinking rate*/
    HAL_Delay(50);
    
  }
}


/**
* @brief  Configure GPIO outputs for the BUCK-BOOST converter
* @param  None
* @retval None
*/
static void GPIO_BUCK_BOOST_outputs_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIOA clock for timer A outputs */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure HRTIM output: TA1 (PA8) */
  GPIO_InitStruct.Pin = GPIO_PIN_8; 
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;;  
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;;  
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
  HRTIM_OutputCfgTypeDef output_config;
  HRTIM_CompareCfgTypeDef compare_config;
  HRTIM_DeadTimeCfgTypeDef HRTIM_TIM_DeadTimeConfig;
  HRTIM_ADCTriggerCfgTypeDef adc_trigger_config;
  HRTIM_FaultCfgTypeDef fault_config;

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
    Error_Handler(); /* if DLL or clock is not correctly set */
  }
  
  /* -------------------------------------------------------------- */
  /* TIMERA and TIMERB initialization: timer mode and PWM frequency */
  /* The 2 timers are running with identical parameters.            */
  /* -------------------------------------------------------------- */
  timebase_config.Period = BUCK_PWM_PERIOD; /* 250kHz switching frequency */
  timebase_config.RepetitionCounter = 31; /* 1 ISR every 32 PWM periods */
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &timebase_config);
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, &timebase_config);

  
  /* --------------------------------------------------------------------- */
  /* TIMERA global configuration: cnt reset, sync, update, fault, burst... */
  /* timer running in continuous mode, with deadtime enabled               */
  /* Very same configuration for Timer B                                   */
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
  timer_config.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_ENABLED;
  timer_config.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  timer_config.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &timer_config);
  
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, &timer_config);
  
    
  /* --------------------------------- */
  /* TA1 and TA2 waveforms description */
  /* --------------------------------- */
  output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config.SetSource = HRTIM_OUTPUTSET_TIMCMP1;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMPER;
  output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_A,
                                 HRTIM_OUTPUT_TA1,
                                 &output_config);

  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_A,
                                 HRTIM_OUTPUT_TA2,
                                 &output_config);

  /* Set compare registers for duty cycle on TA1 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = BUCK_PWM_PERIOD - 0x60;  /* Start with minimal duty cycle */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_A,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);
  
  /* --------------------------------- */
  /* TB1 and TB2 waveforms description */
  /* --------------------------------- */
  output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config.SetSource = HRTIM_OUTPUTSET_TIMCMP1;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMPER;
  output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_B,
                                 HRTIM_OUTPUT_TB1,
                                 &output_config);

  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_B,
                                 HRTIM_OUTPUT_TB2,
                                 &output_config);

  /* Set compare registers for duty cycle on TB1 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = BUCK_PWM_PERIOD+1;  /* 0% duty cycle */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_B,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

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
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.CompareValue = BUCK_PWM_PERIOD/2; /* Samples at 50% of period */
  compare_config.AutoDelayedTimeout = 0;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_A,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

  adc_trigger_config.Trigger = HRTIM_ADCTRIGGEREVENT24_TIMERA_CMP2;
  adc_trigger_config.UpdateSource = HRTIM_ADCTRIGGERUPDATE_TIMER_A;
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

  /* ADC1 is working independently */
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
  AdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
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
  
  /* Run the ADC calibration in single-ended mode */
  HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED);

  /* Start ADC1 Injected Conversions */ 
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
