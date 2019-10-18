/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_BuckSyncRect/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to control a buck converter with
synchronous rectification using the HRTIM
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

/** @addtogroup HRTIM_BuckSyncRect
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
#define ADC_VREF ((uint16_t)3300) /* Vref+ input voltage in mV */

//#define SNIPPET /* HRTIM initialisation done at register level (no HAL call) */

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t Vin, Vout;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

static void GPIO_BUCK_outputs_Config(void);
static void HRTIM_Config(void);
#ifdef SNIPPET
static void HRTIM_Config_NoHAL(void);
#endif /* SNIPPET */
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
  #ifdef SNIPPET
    HRTIM_Config_NoHAL();
  #else
  HRTIM_Config();
  #endif
  
  /* Initialize BUCK outputs (it has to be done after HRTIM init) */
  GPIO_BUCK_outputs_Config();
  
  /* Turn ON T6 MOSFET on discovery board */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
  
  /* Infinite loop */
  while (1)
  {
    
    /* ---------------- */
    /* Fault management */
    /* ---------------- */
    #ifdef SNIPPET
      /* If Fault occured */
      while(HRTIM1->sCommonRegs.ISR & HRTIM_ICR_FLT1C)
      {
        /* LED3 is flashing in case of fault */
        BSP_LED_On(LED3);
        HAL_Delay(20);
        BSP_LED_Off(LED3);
        HAL_Delay(80);    

        /* Re-arm HRTIM TD1 output if "User" push button is pressed*/
        if((BSP_PB_GetState(BUTTON_USER) == SET))
        {
          /* Clear interrupt flag */
          HRTIM1->sCommonRegs.ICR = HRTIM_ICR_FLT1C;
          /* Re-enable TA1 and TA2 */
          HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TA1OEN + HRTIM_OENR_TA2OEN; 
        }
      }
    #else
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
          HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2);
      }
    }
    #endif

    /* ---------------- */
    /* Normal operation */
    /* ---------------- */
    /* LED6 toggling to show MCU activity */
    BSP_LED_On(LED6);
    HAL_Delay(100);
    BSP_LED_Off(LED6);
    HAL_Delay(400);
    
    /* -----------------------------------------------------------------------*/
    /* Input and output voltages can be displayed real-time in a watch window */
    /* -----------------------------------------------------------------------*/
    Vin = (HAL_ADCEx_InjectedGetValue(&AdcHandle, ADC_INJECTED_RANK_1) * ADC_VREF)/0x1000;
    /* VIN bridge conversion is 4.97 (6.8K / 6.8K + 27K) */
    Vin = (497 * Vin )/100;
    
    Vout = (HAL_ADCEx_InjectedGetValue(&AdcHandle, ADC_INJECTED_RANK_2) * ADC_VREF)/0x1000;
    /* VOUT bridge conversion is 5.03 (3.3K / 3.3K + 13.3K) */
    Vout = (503 * Vout)/100;

  }
}


/**
* @brief  Configure GPIO outputs for the BUCK converter
* @param  None
* @retval None
*/
static void GPIO_BUCK_outputs_Config(void)
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
  
  /* Configure PA11 as output to control T6 FET on discovery board */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;;  
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;;  

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
  HRTIM_OutputCfgTypeDef output_config_TA1_TA2;
  HRTIM_CompareCfgTypeDef compare_config;
  HRTIM_DeadTimeCfgTypeDef HRTIM_TIM_DeadTimeConfig;
  HRTIM_ADCTriggerCfgTypeDef adc_trigger_config;
  HRTIM_FaultCfgTypeDef fault_config;

  /* ----------------------------*/
  /* HRTIM Global initialization */
  /* ----------------------------*/
  /* Initialize the hrtim structure (minimal configuration) */
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
  /* TIMERA initialization: timer mode and PWM frequency */
  /* --------------------------------------------------- */
  timebase_config.Period = BUCK_PWM_PERIOD; /* 400kHz switching frequency */
  timebase_config.RepetitionCounter = 127; /* 1 ISR every 128 PWM periods */
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &timebase_config);

  
  /* --------------------------------------------------------------------- */
  /* TIMERA global configuration: cnt reset, sync, update, fault, burst... */
  /* timer running in continuous mode, with deadtime enabled               */
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
    
  /* --------------------------------- */
  /* TA1 and TA2 waveforms description */
  /* --------------------------------- */
  output_config_TA1_TA2.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config_TA1_TA2.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config_TA1_TA2.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  output_config_TA1_TA2.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config_TA1_TA2.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config_TA1_TA2.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config_TA1_TA2.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config_TA1_TA2.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_A,
                                 HRTIM_OUTPUT_TA1,
                                 &output_config_TA1_TA2);

  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_A,
                                 HRTIM_OUTPUT_TA2,
                                 &output_config_TA1_TA2);

  /* Set compare registers for duty cycle on TA1 */
  compare_config.CompareValue = BUCK_PWM_PERIOD/20;  /* 5% duty cycle */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_A,
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

  /* ------------------------------------------- */
  /* ADC trigger intialization (with CMP2 event) */
  /* ------------------------------------------- */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = BUCK_PWM_PERIOD/10; /* Samples in middle of ON time */
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

  /* ---------------*/
  /* HRTIM start-up */
  /* ---------------*/
  /* Enable HRTIM's outputs TA1 and TA2 */
  /* Note: it is necessary to enable also GPIOs to have outputs functional */
  /* This must be done after HRTIM initialization */
  HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2);

  /* Start HRTIM's TIMER A */
  HAL_HRTIM_WaveformCountStart_IT(&hhrtim, HRTIM_TIMERID_TIMER_A);

}

#ifdef SNIPPET
/**
* @brief  HRTIM configuration at register level (no HAL call)
* @param  None
* @retval None
*/
static void HRTIM_Config_NoHAL(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

  /* --------------------------*/
  /* HRTIM Input and NVIC init */
  /* --------------------------*/

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure HRTIM input: FAULT1 (PA12) */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;;  
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;;  
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* Configure and enable HRTIM TIMERA interrupt channel in NVIC */
  HAL_NVIC_SetPriority(HRTIM1_TIMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(HRTIM1_TIMA_IRQn);

  /* --------------------------*/
  /* HRTIM Clock and DLL init  */
  /* --------------------------*/

  /* Use the PLLx2 clock for HRTIM */
  __HAL_RCC_HRTIM1_CONFIG(RCC_HRTIM1CLK_PLLCLK);

  /* Enable HRTIM clock*/
  __HAL_RCC_HRTIM1_CLK_ENABLE();

  /* DLL calibration: periodic calibration enabled, period set to 14µs */
  HRTIM1->sCommonRegs.DLLCR = HRTIM_CALIBRATIONRATE_14| HRTIM_DLLCR_CALEN;
    
  /* Check DLL end of calibration flag */
  while(HRTIM1->sCommonRegs.ISR & HRTIM_IT_DLLRDY == RESET);
  
  /* -------------------------------------------------------- */
  /* TIMERA initialization: PWM frequency and Repetition rate */
  /* Continuous mode, preload enabled on REP event            */
  /* -------------------------------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].PERxR = BUCK_PWM_PERIOD; /* 400kHz switching frequency */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].REPxR = 127; /* 1 ISR every 128 PWM periods */

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].TIMxCR = HRTIM_TIMCR_CONT
                                                       + HRTIM_TIMCR_PREEN
                                                       + HRTIM_TIMCR_TREPU;
  /* Enable REP interrupts */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].TIMxDIER = HRTIM_TIMDIER_REPIE;

  /* --------------------------------------------------------------- */
  /* TA1 and TA2 waveforms description: Set on period, reset on CMP1 */
  /* deadtime enabled, fault1 selected, disabling both outputs                   */
  /* --------------------------------------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].SETx1R = HRTIM_SET1R_PER;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].RSTx1R = HRTIM_RST1R_CMP1;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].OUTxR = HRTIM_OUTR_DTEN + HRTIM_OUTR_FAULT1_1 + HRTIM_OUTR_FAULT2_1;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].FLTxR = HRTIM_FLTR_FLT1EN;
  
  /* Set compare registers for duty cycle on TA1 */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = BUCK_PWM_PERIOD/20;    /* 5% duty cycle */
  /* Set deadtime values and lock deadtime signs */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].DTxR = HRTIM_DTR_DTFSLK + (DT_FALLING<<16) + HRTIM_DTR_DTRSLK + DT_RISING;
  
  /* ------------------------------------------- */
  /* ADC trigger intialization (with CMP2 event) */
  /* ------------------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP2xR = BUCK_PWM_PERIOD/10;    /* Samples in middle of ON time */
  HRTIM1->sCommonRegs.CR1 = HRTIM_CR1_ADC2USRC_0; /* ADC trigger update: TimerA */
  HRTIM1->sCommonRegs.ADC2R = HRTIM_ADC2R_AD2TAC2; /* ADC trigger event: Timer A compare 2*/

  /* ---------------------------------------------------------- */
  /* FAULT1 global init: no filter, low polarity, Fault1 enable */
  /* ---------------------------------------------------------- */
  HRTIM1->sCommonRegs.FLTINR1 = HRTIM_FLTINR1_FLT1E;

  /* ---------------*/
  /* HRTIM start-up */
  /* ---------------*/
  /* Force register update before starting (optional) */
  HRTIM1->sCommonRegs.CR2 |= HRTIM_CR2_TASWU;
  
  /* Enable HRTIM's outputs TA1 and TA2 */
  /* Note: it is necessary to enable also GPIOs to have outputs functional */
  /* This must be done after HRTIM initialization */
  HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TA1OEN + HRTIM_OENR_TA2OEN; 

  /* Start HRTIM's TIMER A */
  HRTIM1->sMasterRegs.MCR = HRTIM_MCR_TACEN;
}
#endif /* SNIPPET */

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
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfDiscConversion = 1;
  AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  AdcHandle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  HAL_ADC_Init(&AdcHandle);

  /* Discontinuous injected mode: 1st injected conversion for Vin on Ch2 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_2;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_1;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_7CYCLES_5;
  InjectionConfig.InjectedSingleDiff = ADC_SINGLE_ENDED;
  InjectionConfig.InjectedOffsetNumber = ADC_OFFSET_NONE;
  InjectionConfig.InjectedOffset = 0;
  InjectionConfig.InjectedNbrOfConversion = 2;
  InjectionConfig.InjectedDiscontinuousConvMode = DISABLE;
  InjectionConfig.AutoInjectedConv = DISABLE;
  InjectionConfig.QueueInjectedContext = DISABLE;
  InjectionConfig.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_HRTIM_TRG2;
  InjectionConfig.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectionConfig);
  
  /* Configure the 2nd injected conversion for Vout on Ch4 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_4;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_2;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_19CYCLES_5;
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
