/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_TM_PFC/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to control a transition mode PFC with HRTIM
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

/** @addtogroup HRTIM_TM_PFC
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

//#define SNIPPET /* HRTIM initialisation done at register level (no HAL call) */

#define TON_MAX 11520   /* 2.5탎 normal Ton time */
#define TON_MIN 2304    /* 500ns overcurrent blanking time */

#define TOFF_MIN 4608   /* 1탎, 285kHz max PFC frequency: 1/(TONmax + Toffmin)*/
#define TOFF_MAX 34560  /* 7.5탎 timeout if no ZCD occurs */

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

static void GPIO_HRTIM_outputs_Config(void);
static void HRTIM_Config(void);
#ifdef SNIPPET
static void HRTIM_Config_NoHAL(void);
#endif /* SNIPPET */

/* Private functions ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
HRTIM_HandleTypeDef hhrtim;

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
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);

  /* Initialize User_Button on STM32F3348-DISCO */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);   

  /* Initialize HRTIM and related inputs */
  #ifdef SNIPPET
    HRTIM_Config_NoHAL();
  #else
  HRTIM_Config();
  #endif

  /* Initialize HRTIM outputs (it has to be done after HRTIM init) */
  GPIO_HRTIM_outputs_Config();
  
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
        /* LED4 is flashing in case of fault */
        BSP_LED_On(LED4);
        HAL_Delay(20);
        BSP_LED_Off(LED4);
        HAL_Delay(80);

        /* Re-arm HRTIM TD1 output if "User" push button is pressed*/
        if((BSP_PB_GetState(BUTTON_USER) == SET))
        {
          /* Clear interrupt flag */
          HRTIM1->sCommonRegs.ICR = HRTIM_ICR_FLT1C;
          /* Re-enable TD1 */
          HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TD1OEN;
        }
      }
    #else
    while(__HAL_HRTIM_GET_FLAG(&hhrtim, HRTIM_FLAG_FLT1) == SET)
    {
        /* LED4 is flashing in case of fault */
        BSP_LED_On(LED4);
      HAL_Delay(20);
        BSP_LED_Off(LED4);
      HAL_Delay(80);
      
      /* Re-arm HRTIM TD1 output if "User" push button is pressed*/
      if((BSP_PB_GetState(BUTTON_USER) == SET))
      {
          __HAL_HRTIM_CLEAR_IT(&hhrtim, HRTIM_IT_FLT1);
          HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TD1);
      }
    }
    #endif
    

    /* ---------------- */
    /* Normal operation */
    /* ---------------- */
    /* LED5 toggling to show MCU activity */
    BSP_LED_On(LED5);
    HAL_Delay(100);
    BSP_LED_Off(LED5);
    HAL_Delay(400);
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
  HRTIM_TimerEventFilteringCfgTypeDef TimerEventFiltering_config;
  HRTIM_FaultCfgTypeDef fault_config;
    HRTIM_EventCfgTypeDef pEventCfg;
    HRTIM_CaptureCfgTypeDef pCaptureCfg;


  /* ----------------------------*/
  /* HRTIM Global initialization */
  /* ----------------------------*/
  /* Initialize the hrtim structure (minimal configuration) */
  hhrtim.Instance = HRTIM1;
  hhrtim.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
  hhrtim.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;

  /* Initialize HRTIM */
  HAL_HRTIM_Init(&hhrtim);

  /* HRTIM DLL calibration: periodic calibration, set period to 14탎 */
  HAL_HRTIM_DLLCalibrationStart(&hhrtim, HRTIM_CALIBRATIONRATE_14);
  /* Wait calibration completion*/
  if (HAL_HRTIM_PollForDLLCalibration(&hhrtim, 100) != HAL_OK)
  {
    Error_Handler(); // if DLL or clock is not correctly set
  }
  
  /* --------------------------------------------------- */
  /* TIMERD initialization: timer mode and PWM frequency */
  /* --------------------------------------------------- */
  timebase_config.Period = 0xFFDF;      /* Set max period */
  timebase_config.RepetitionCounter = 0;
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, &timebase_config);

  
  /* ------------------------------------------------------------------------ */
  /* TIMERD global configuration: fault 1 enabled, reset on CMP4 or ZCD(EEV4) */
  /* ------------------------------------------------------------------------ */
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
  timer_config.RepetitionUpdate = HRTIM_UPDATEONREPETITION_DISABLED;
  timer_config.ResetUpdate = HRTIM_TIMUPDATEONRESET_ENABLED;
  timer_config.InterruptRequests = HRTIM_TIM_IT_NONE;
  timer_config.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  timer_config.FaultEnable = HRTIM_TIMFAULTENABLE_FAULT1;
  timer_config.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  timer_config.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  timer_config.DelayedProtectionMode = HRTIM_TIMER_D_E_DELAYEDPROTECTION_DISABLED;
  timer_config.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_CMP4 + HRTIM_TIMRESETTRIGGER_EEV_4;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D,&timer_config);
    
  /* --------------------------------- */
  /* TD1 and TD2 waveforms description */
  /* TD1 is the PFC output             */
  /* TD2 is here for debugging purpose */
  /* --------------------------------- */
  output_config_TD1.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config_TD1.SetSource = HRTIM_OUTPUTSET_TIMCMP4 + HRTIM_OUTPUTSET_EEV_4;
  output_config_TD1.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1 + HRTIM_OUTPUTRESET_EEV_3;
  output_config_TD1.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config_TD1.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config_TD1.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config_TD1.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config_TD1.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD1,
                                 &output_config_TD1);

  /* TD2 allows to monitor either the OC or the ZCD blaking windows */
  output_config_TD2.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  
  /* Uncomment the 2 following lines to monitor the ZCD blanking window on TD2 output */
  output_config_TD2.SetSource = HRTIM_OUTPUTSET_TIMCMP4+ HRTIM_OUTPUTSET_EEV_4;
  output_config_TD2.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP2;

  /* Uncomment the 2 following lines to monitor the OC blanking window on TD2 output */
/*  output_config_TD2.SetSource = HRTIM_OUTPUTSET_TIMCMP4+ HRTIM_OUTPUTSET_EEV_4; */
/*  output_config_TD2.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP3; */
  output_config_TD2.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config_TD2.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config_TD2.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE;
  output_config_TD2.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config_TD2.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD2,
                                 &output_config_TD2);

  
  /* Set compare 1 register for TonMax time */
  compare_config.CompareValue = TON_MAX;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* Set compare 3 register for TonMin time */
  compare_config.CompareValue = TON_MIN;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);


  /* Set compare 2 register for ToffMin time, autodelayed vs ZCD event */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_AUTODELAYED_TIMEOUTCMP1;
  compare_config.AutoDelayedTimeout = TON_MAX;
  compare_config.CompareValue = TOFF_MIN;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

  /* Set compare 4 register for ToffMax time, autodelayed vs ZCD event */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_AUTODELAYED_TIMEOUTCMP1;
  compare_config.AutoDelayedTimeout = TON_MAX;
  compare_config.CompareValue = TOFF_MAX;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);

  /* ------------------------------ */
  /* OC and ZCD filters programming */
  /* ------------------------------ */
  /* OC (EEV3 signal) blanking done with CMP3 */
  TimerEventFiltering_config.Filter = HRTIM_TIMEVENTFILTER_BLANKINGCMP3;
  TimerEventFiltering_config.Latch = HRTIM_TIMEVENTLATCH_DISABLED;
  HAL_HRTIM_TimerEventFilteringConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_EVENT_3,
                                  &TimerEventFiltering_config);

  /* ZCD (EEV4 signal blanking done on CMP2, latch is enabled (Toff min 
  operation if ZCD occurs too early) */
  TimerEventFiltering_config.Filter = HRTIM_TIMEVENTFILTER_BLANKINGCMP2;
  TimerEventFiltering_config.Latch = HRTIM_TIMEVENTLATCH_ENABLED;
  HAL_HRTIM_TimerEventFilteringConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_EVENT_4,
                                  &TimerEventFiltering_config);

  
  /* ------------------------------------------------- */
  /* Capture units programming for autodelayed compare */
  /* ------------------------------------------------- */
  HAL_HRTIM_EventPrescalerConfig(&hhrtim, HRTIM_EVENTPRESCALER_DIV1);
  /* OC event is rising edge sensitive */
  pEventCfg.Source = HRTIM_EVENTSRC_1;
  pEventCfg.Polarity = HRTIM_EVENTPOLARITY_HIGH;
  pEventCfg.Sensitivity = HRTIM_EVENTSENSITIVITY_RISINGEDGE;
  pEventCfg.FastMode = HRTIM_EVENTFASTMODE_DISABLE;
  HAL_HRTIM_EventConfig(&hhrtim, HRTIM_EVENT_3, &pEventCfg);
  
  /* ZCD event is rising edge sensitive */
  pEventCfg.Sensitivity = HRTIM_EVENTSENSITIVITY_FALLINGEDGE;
  HAL_HRTIM_EventConfig(&hhrtim, HRTIM_EVENT_4, &pEventCfg);

  /* OC (EEV3) triggers Capture 1 on rising edge for autodelayed CMP2 */
  /* OC (EEV3) triggers Capture 2 on rising edge for autodelayed CMP4 */
  pCaptureCfg.Trigger = HRTIM_CAPTURETRIGGER_EEV_3;
  HAL_HRTIM_WaveformCaptureConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_CAPTUREUNIT_1, &pCaptureCfg);
  
  HAL_HRTIM_WaveformCaptureConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_CAPTUREUNIT_2, &pCaptureCfg);

  
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
  HAL_HRTIM_WaveformCountStart(&hhrtim, HRTIM_TIMERID_TIMER_D);

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

  /* ---------------- */
  /* HRTIM Input init */
  /* ---------------- */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /* Configure HRTIM input: FAULT1 (PA12) */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;;  
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;;  
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
  /* Configure HRTIM input: EEV3 and EEV4 external events (PB7 and PB6) */
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* --------------------------*/
  /* HRTIM Clock and DLL init  */
  /* --------------------------*/
  /* Use the PLLx2 clock for HRTIM */
  __HAL_RCC_HRTIM1_CONFIG(RCC_HRTIM1CLK_PLLCLK);

  /* Enable HRTIM clock*/
  __HAL_RCC_HRTIM1_CLK_ENABLE();

  /* DLL calibration: periodic calibration enabled, period set to 14탎 */
  HRTIM1->sCommonRegs.DLLCR = HRTIM_CALIBRATIONRATE_14| HRTIM_DLLCR_CALEN;
    
  /* Check DLL end of calibration flag */
  while(HRTIM1->sCommonRegs.ISR & HRTIM_IT_DLLRDY == RESET);
  
  /* --------------------------------------------------------------- */
  /* TIMERD initialization: continuous mode, preload enabled on rst */
  /* Autodelayed with timeout on CMP1 for both CMP2 and CMP4         */
  /* Counter reset on CMP4 or ZCD(EEV4)*/
  /* --------------------------------------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].TIMxCR = HRTIM_TIMCR_CONT
                                                       + HRTIM_TIMCR_PREEN
                                                       + HRTIM_TIMCR_TRSTU
                                                       + HRTIM_TIMCR_DELCMP2_1
                                                       + HRTIM_TIMCR_DELCMP4_1;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTxR = HRTIM_RSTR_EXTEVNT4 + HRTIM_RSTR_CMP4;
 
  /* --------------------------------- */
  /* TD1 and TD2 waveforms description */
  /* TD1 is the PFC output             */
  /* TD2 is here for debugging purpose */
  /* --------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx1R = HRTIM_SET1R_CMP4 + HRTIM_SET1R_EXTVNT4;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx1R = HRTIM_RST1R_CMP1 + HRTIM_RST1R_EXTVNT3;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].OUTxR = HRTIM_OUTR_FAULT1_1;

  
  /* TD2 allows to monitor either the OC or the ZCD blaking windows */
  /* Uncomment the following lines to monitor the ZCD blanking window on TD2 output */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx2R = HRTIM_SET2R_CMP4 + HRTIM_SET2R_EXTVNT4;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx2R = HRTIM_RST2R_CMP2;
  /* Uncomment the following lines to monitor the OC blanking window on TD2 output */
  /* HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx2R = HRTIM_RST2R_CMP3; */

  /* Set compare 1 register for TonMax time */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP1xR = TON_MAX;

  /* Set compare 3 register for TonMin time */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP3xR = TON_MIN;

  /* Set compare 2 register for ToffMin time, autodelayed vs ZCD event */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP2xR = TOFF_MIN;
  
  /* Set compare 4 register for ToffMax time, autodelayed vs ZCD event */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP4xR = TOFF_MAX;
  
  /* ------------------------------ */
  /* OC and ZCD filters programming */
  /* ------------------------------ */
  /* OC (EEV3 signal) blanking done with CMP3 */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].EEFxR1 = HRTIM_EEFR1_EE3FLTR_0 + HRTIM_EEFR1_EE3FLTR_1;

  /* ZCD (EEV4 signal blanking done on CMP2, latch is enabled (Toff min 
  operation if ZCD occurs too early) */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].EEFxR1 |= HRTIM_EEFR1_EE4FLTR_1 + HRTIM_EEFR1_EE4LTCH;
  
  /* ------------------------------------------------- */
  /* Capture units programming for autodelayed compare */
  /* ------------------------------------------------- */
  /* ZCD (EEV4) triggers Capture 1 on rising edge for autodelayed CMP2 */
  /* ZCD (EEV4) triggers Capture 2 on rising edge for autodelayed CMP4 */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CPT1xCR =HRTIM_CPT1CR_EXEV3CPT;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CPT2xCR =HRTIM_CPT2CR_EXEV3CPT;
  /* ZCD (EEV4) falling edge sensitive and OC (EEV3) rising edge sensitive */
  HRTIM1->sCommonRegs.EECR1 = HRTIM_EECR1_EE4SNS_1 + HRTIM_EECR1_EE3SNS_0;
  
  /* ---------------------*/
  /* FAULT initialization */
  /* ---------------------*/
  /* FAULT1 global init: no filter, low polarity, Fault1 enable */
  HRTIM1->sCommonRegs.FLTINR1 = HRTIM_FLTINR1_FLT1E;
  /* Fault 1 enabled for Timer D */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].FLTxR = HRTIM_FLTR_FLT1EN;

  /* ---------------*/
  /* HRTIM start-up */
  /* ---------------*/
  /* Force register update before starting (optional) */
  HRTIM1->sCommonRegs.CR2 |= HRTIM_CR2_TASWU;

  /* Enable HRTIM's outputs TD1 and TD2 */
  /* Note: it is necessary to enable also GPIOs to have outputs functional */
  /* This must be done after HRTIM initialization */
  HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TD1OEN + HRTIM_OENR_TD2OEN; 

  /* Start HRTIM's TIMER D*/
  HRTIM1->sMasterRegs.MCR = HRTIM_MCR_TDCEN;
  
}
#endif /* SNIPPET */

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
  /* Turn on LED4 (ORANGE) */
  BSP_LED_On(LED4);
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
