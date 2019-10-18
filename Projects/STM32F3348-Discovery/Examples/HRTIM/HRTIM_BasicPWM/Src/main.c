/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_BasicPWM/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to generate basic PWM waveforms with the 
  * HRTIM, as per HRTIM Cookbook basic examples.
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

/** @addtogroup HRTIM_BasicPWM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
#define HRTIM_INPUT_CLOCK       ((uint64_t)144000000)   /* Value in Hz */

/* Formula below works down to 70.3kHz (with presc ratio = 1) */ 
#define _200KHz_PERIOD ((uint16_t)((HRTIM_INPUT_CLOCK * 32) / 200000))

/* Formula below works down to 70.3kHz (with presc ratio = 1) */ 
#define _100KHz_PERIOD ((uint16_t)((HRTIM_INPUT_CLOCK * 32) / 100000))

/* Formula below works down to 70.3kHz (with presc ratio = 1) */ 
#define _100KHz_Plus_PERIOD ((uint16_t)((HRTIM_INPUT_CLOCK * 32) / 100001))

/* Formula below works down to 17.6kHz (with presc ratio = 4) */ 
#define _33KHz_PERIOD ((uint16_t)((HRTIM_INPUT_CLOCK * 8) / 33333))

/* Define here the snippet to be executed */
#define HRTIM_CHECK
//#define SINGLE_PWM
//#define MULTIPLE_PWM
//#define PWM_MASTER
//#define ARBITRARY_WAVEFORM

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

static void GPIO_HRTIM_outputs_Config(void);

#ifdef HRTIM_CHECK
static void HRTIM_Config_HRTIMCheck(void);
#endif /* HRTIM_CHECK */

#ifdef SINGLE_PWM
static void HRTIM_Config_SinglePWM(void);
#endif /* SINGLE_PWM */

#ifdef MULTIPLE_PWM
static void HRTIM_Config_MultiplePWM(void);
#endif  /* MULTIPLE_PWM */

#ifdef PWM_MASTER
static void HRTIM_Config_MasterPWM(void);
#endif /* PWM_MASTER */

#ifdef ARBITRARY_WAVEFORM
static void HRTIM_Config_ArbitraryWaveform(void);
#endif /* ARBITRARY_WAVEFORM */
  
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
  BSP_LED_Init(LED3);   // Indicates Error
  BSP_LED_Init(LED5);   // Indicates MCU is active


/* ---------------------------------------------------------------------------*/
/* ------------------------ EXAMPLE 1: HRTIM_CHECK ---------------------------*/
/* ---------------------------------------------------------------------------*/
#ifdef HRTIM_CHECK
  
  HRTIM_Config_HRTIMCheck();    /* Initialize HRTIM and related inputs */
  GPIO_HRTIM_outputs_Config();  /* Initialize GPIO's HRTIM outputs */
  BSP_LED_On(LED5);

  while(1)
  {
    /* TD1 output toggles at ~430kHz (this frequency can vary depending on compiler and optimizations options) */
    HAL_HRTIM_WaveformSetOutputLevel(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1, HRTIM_OUTPUTLEVEL_ACTIVE);
    HAL_HRTIM_WaveformSetOutputLevel(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1, HRTIM_OUTPUTLEVEL_INACTIVE);
    
    /* With direct register access, TD1 output toggles at ~10MHz */
//    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx1R = HRTIM_SET1R_SST;
//    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx1R = HRTIM_RST1R_SRT;
  }
#endif  /* HRTIM_CHECK */


/* ---------------------------------------------------------------------------*/
/* ------------------------ EXAMPLE 2: SINGLE PWM ----------------------------*/
/* ---------------------------------------------------------------------------*/
#ifdef SINGLE_PWM

  HRTIM_Config_SinglePWM();     /* Initialize HRTIM and related inputs */
  GPIO_HRTIM_outputs_Config();  /* Initialize GPIO's HRTIM outputs */

  /* Infinite loop */
  while (1)
  {
    BSP_LED_Toggle(LED5);
    HAL_Delay(100);
  }
#endif  /* SINGLE_PWM */

  
/* ---------------------------------------------------------------------------*/
/* ------------------------ EXAMPLE 3: MULTIPLE PWM --------------------------*/
/* ---------------------------------------------------------------------------*/
#ifdef MULTIPLE_PWM
  
  HRTIM_Config_MultiplePWM();   /* Initialize HRTIM and related inputs */
  GPIO_HRTIM_outputs_Config();  /* Initialize GPIO's HRTIM outputs */

  /* Infinite loop */
  while (1)
  {
    BSP_LED_Toggle(LED5);
    HAL_Delay(100);
  }
#endif /* MULTIPLE_PWM */


/* ---------------------------------------------------------------------------*/
/* -------------------- SNIPPET 4: PWM with MASTER TIMER ---------------------*/
/* ---------------------------------------------------------------------------*/
#ifdef PWM_MASTER

  HRTIM_Config_MasterPWM();     /* Initialize HRTIM and related inputs */
  GPIO_HRTIM_outputs_Config();  /* Initialize GPIO's HRTIM outputs */

  /* Infinite loop */
  while (1)
  {
    BSP_LED_Toggle(LED5);
    HAL_Delay(100);
  }
#endif /* PWM_MASTER */


/* ---------------------------------------------------------------------------*/
/* --------------------- EXAMPLE 5: ARBITRARY_WAVEFORM -----------------------*/
/* ---------------------------------------------------------------------------*/
#ifdef ARBITRARY_WAVEFORM

  HRTIM_Config_ArbitraryWaveform();     /* Initialize HRTIM and related inputs */
  GPIO_HRTIM_outputs_Config();  /* Initialize GPIO's HRTIM outputs */

  /* Infinite loop */
  while (1)
  {
    BSP_LED_Toggle(LED5);
    HAL_Delay(100);
  }
#endif /* ARBITRARY_WAVEFORM */

}


/**
* @brief  Configure GPIO outputs for the HRTIM
* @param  None
* @retval None
*/
static void GPIO_HRTIM_outputs_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIOA clock for timer A outputs */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure HRTIM output: TA1 (PA8) and TA2 (PA9)*/
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9; 
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;;  
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;;  
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Enable GPIOB clock for timer D outputs */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure HRTIM output: TD1 (PB14) and TD2 (PB15)*/
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15; 
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;;  
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;;  
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

#ifdef HRTIM_CHECK
/**
* @brief  HRTIM configuration for checking that HRTIM is up and running
* @param  None
* @retval None
*/
static void HRTIM_Config_HRTIMCheck(void)
{

  /* ------------------------------------------------- */
    /* HRTIM Global initialization: Clock and DLL init */
  /* ------------------------------------------------- */
  /* Initialize the HRTIM structure (minimal configuration) */
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
    Error_Handler(); /* if DLL or clock is not correctly set */
  }

  /* ---------------*/
  /* HRTIM start-up */
  /* ---------------*/
  /* Enable HRTIM's outputs TD1 */
  /* Note: it is necessary to enable also GPIOs to have outputs functional */
  /* This must be done after HRTIM initialization */
  HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TD1); 

}
#endif /* HRTIM_CHECK */


#ifdef SINGLE_PWM
/**
* @brief  HRTIM configuration for a single PWM signal
* @param  None
* @retval None
*/
static void HRTIM_Config_SinglePWM(void)
{
  HRTIM_TimeBaseCfgTypeDef timebase_config;
  HRTIM_TimerCfgTypeDef timer_config;
  HRTIM_OutputCfgTypeDef output_config_TD1;
  HRTIM_CompareCfgTypeDef compare_config;

  /* ----------------------------------------------- */
    /* HRTIM Global initialization: Clock and DLL init */
  /* ----------------------------------------------- */
  /* Initialize the HRTIM structure (minimal configuration) */
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
    Error_Handler(); /* if DLL or clock is not correctly set */
  }
  
  /* ------------------------------------------------------------ */
  /* TIMERD initialization: set PWM frequency and continuous mode */
  /* ------------------------------------------------------------ */
  timebase_config.Period = _100KHz_PERIOD;
  timebase_config.RepetitionCounter = 0;
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, &timebase_config);

  
  /* --------------------------------------------------------------------- */
  /* TIMERD global configuration: all values to default                    */
  /* --------------------------------------------------------------------- */
  timer_config.DMARequests = HRTIM_TIM_DMA_NONE;
  timer_config.DMASrcAddress = 0x0;
  timer_config.DMADstAddress = 0x0;
  timer_config.DMASize = 0x0;
  timer_config.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
  timer_config.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  timer_config.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  timer_config.DACSynchro = HRTIM_DACSYNC_NONE;
  timer_config.PreloadEnable = HRTIM_PRELOAD_DISABLED;
  timer_config.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  timer_config.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  timer_config.RepetitionUpdate = HRTIM_UPDATEONREPETITION_DISABLED;
  timer_config.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  timer_config.InterruptRequests = HRTIM_TIM_IT_NONE;
  timer_config.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  timer_config.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
  timer_config.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  timer_config.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  timer_config.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  timer_config.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D,&timer_config);
    
  /* --------------------------------------------------------- */
  /* TD1 waveform description TD1 set on period, reset on CMP1 */
  /* --------------------------------------------------------- */
  output_config_TD1.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config_TD1.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config_TD1.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  output_config_TD1.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config_TD1.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config_TD1.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE;
  output_config_TD1.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config_TD1.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD1,
                                 &output_config_TD1);

  /* Set compare registers for duty cycle on TD1 */
  compare_config.CompareValue = _100KHz_PERIOD/2;     // 50% duty cycle
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* ---------------*/
  /* HRTIM start-up */
  /* ---------------*/
  /* Enable HRTIM's outputs TD1 */
  /* Note: it is necessary to enable also GPIOs to have outputs functional */
  /* This must be done after HRTIM initialization */
  HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TD1); 

  /* Start HRTIM's TIMER D*/
  HAL_HRTIM_WaveformCountStart(&hhrtim, HRTIM_TIMERID_TIMER_D);

}
#endif /* SINGLE_PWM */

#ifdef MULTIPLE_PWM
/**
* @brief  HRTIM configuration for generating multiple PWM outputs
* @param  None
* @retval None
*/
static void HRTIM_Config_MultiplePWM(void)
{
  HRTIM_TimeBaseCfgTypeDef timebase_config;
  HRTIM_TimerCfgTypeDef timer_config;
    HRTIM_OutputCfgTypeDef output_config_TDx;
    HRTIM_OutputCfgTypeDef output_config_TAx;
  HRTIM_CompareCfgTypeDef compare_config;

  /* ----------------------------------------------- */
  /* HRTIM Global initialization: Clock and DLL init */
  /* ----------------------------------------------- */
  /* Initialize the HRTIM structure (minimal configuration) */
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
    Error_Handler(); /* if DLL or clock is not correctly set */
  }
  
  /* ------------------------------------------------------------ */
  /* TIMERD initialization: set PWM frequency and continuous mode */
  /* ------------------------------------------------------------ */
  timebase_config.Period = _100KHz_PERIOD;
  timebase_config.RepetitionCounter = 0;
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, &timebase_config);

  
  /* --------------------------------------------------------- */
  /* TIMERD global configuration: preload enabled on REP event */
  /* --------------------------------------------------------- */
  timer_config.DMARequests = HRTIM_TIM_DMA_NONE;
  timer_config.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
  timer_config.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  timer_config.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  timer_config.DACSynchro = HRTIM_DACSYNC_NONE;
  timer_config.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  timer_config.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  timer_config.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  timer_config.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
  timer_config.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  timer_config.InterruptRequests = HRTIM_TIM_IT_NONE;
  timer_config.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  timer_config.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
  timer_config.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  timer_config.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  timer_config.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  timer_config.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D,&timer_config);
    
  /* ------------------------------------------------------ */
  /* TD1 waveform description: set on period, reset on CMP1 */
  /* ------------------------------------------------------ */
  output_config_TDx.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config_TDx.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config_TDx.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  output_config_TDx.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config_TDx.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config_TDx.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE;
  output_config_TDx.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config_TDx.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD1,
                                 &output_config_TDx);

  /* ----------------------------------------------------------------------- */
  /* TD2 waveform description: set on CMP2, reset on period */
  /* Note: TDx init structure is re-used, identical parameters not set again */
  /* ----------------------------------------------------------------------- */
  output_config_TDx.SetSource = HRTIM_OUTPUTSET_TIMCMP2;
  output_config_TDx.ResetSource  = HRTIM_OUTPUTRESET_TIMPER;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD2,
                                 &output_config_TDx);

  /* Set compare registers for duty cycle on TD1 */
  compare_config.CompareValue = _100KHz_PERIOD/4;     /* 25% duty cycle */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* Set compare register for duty cycle on TD2 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (3*_100KHz_PERIOD)/4;     /* 75% of the period */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

  /* ------------------------------------------------------------ */
  /* TIMERA initialization: set PWM frequency and continuous mode */
  /* Prescaler ratio is different from Timer A settings */
  /* ------------------------------------------------------------ */
  timebase_config.Period = _33KHz_PERIOD;
  timebase_config.RepetitionCounter = 0;
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL8;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &timebase_config);

  
  /* --------------------------------------------------------- */
  /* TIMERA global configuration: preload enabled on REP event */
  /* Uses Timer D very same configuration                      */
  /* --------------------------------------------------------- */
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A,&timer_config);
    
  /* ------------------------------------------------------ */
  /* TA1 waveform description: set on period, reset on CMP1 */
  /* ------------------------------------------------------ */
  output_config_TAx.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config_TAx.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config_TAx.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  output_config_TAx.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config_TAx.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config_TAx.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE;
  output_config_TAx.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config_TAx.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_A,
                                 HRTIM_OUTPUT_TA1,
                                 &output_config_TAx);

  /* ------------------------------------------------------ */
  /* TA2 waveform description: set on CMP2, reset on period */
  /* Note: TAx init structure is re-used                    */
  /* ------------------------------------------------------ */
  output_config_TAx.SetSource = HRTIM_OUTPUTSET_TIMCMP2;
  output_config_TAx.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP3;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_A,
                                 HRTIM_OUTPUT_TA2,
                                 &output_config_TAx);

  /* Set compare registers for duty cycle on TA1 */
  compare_config.CompareValue = _33KHz_PERIOD/4;     /* 25% duty cycle */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_A,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* Set compare register for duty cycle on TA2 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = _33KHz_PERIOD/2;     /* 50% of the period */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_A,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

  /* Set compare register for duty cycle on TA2 */
  compare_config.CompareValue = (3*_33KHz_PERIOD)/4;     /* 75% of the period */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_A,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  /* ---------------*/
  /* HRTIM start-up */
  /* ---------------*/
  /* Enable HRTIM's outputs TA1, TA2, TD1 and TD2 */
  /* Note: it is necessary to enable also GPIOs to have outputs functional */
  /* This must be done after HRTIM initialization */
  HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TA2 
                                       + HRTIM_OUTPUT_TD1 + HRTIM_OUTPUT_TD2);

  /* Start HRTIM's TIMER D*/
  HAL_HRTIM_WaveformCountStart(&hhrtim, HRTIM_TIMERID_TIMER_A + HRTIM_TIMERID_TIMER_D);

}
#endif  /* MULTIPLE_PWM */

#ifdef PWM_MASTER
/**
* @brief  HRTIM configuration for generating a PWM with the master timer
* @param  None
* @retval None
*/
static void HRTIM_Config_MasterPWM(void)
{
  HRTIM_TimeBaseCfgTypeDef timebase_config;
  HRTIM_TimerCfgTypeDef timer_config;
    HRTIM_OutputCfgTypeDef output_config_TDx;
  HRTIM_CompareCfgTypeDef compare_config;

  /* ----------------------------------------------- */
  /* HRTIM Global initialization: Clock and DLL init */
  /* ----------------------------------------------- */
  /* Initialize the HRTIM structure (minimal configuration) */
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
    Error_Handler(); /* if DLL or clock is not correctly set */
  }
  

  /* ------------------------------------------------------------ */
  /* TIMERD initialization: set PWM frequency and continuous mode */
  /* ------------------------------------------------------------ */
  timebase_config.Period = _100KHz_PERIOD;
  timebase_config.RepetitionCounter = 0;
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, &timebase_config);

  /* ------------------------------------------------------------------- */
  /* MASTER TIMER initialization: set PWM frequency and continuous mode  */
  /* The frequency difference is very low to show that the 2 signals are */
  /* not synchronous                                                     */
  /* Note: Timer D init struture is re-used                              */
  /* ------------------------------------------------------------------- */
  timebase_config.Period = _100KHz_Plus_PERIOD;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_MASTER, &timebase_config);

  
  /* --------------------------------------------------------- */
  /* TIMERD global configuration: preload enabled on REP event */
  /* --------------------------------------------------------- */
  timer_config.DMARequests = HRTIM_TIM_DMA_NONE;
  timer_config.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
  timer_config.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  timer_config.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  timer_config.DACSynchro = HRTIM_DACSYNC_NONE;
  timer_config.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  timer_config.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  timer_config.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  timer_config.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
  timer_config.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  timer_config.InterruptRequests = HRTIM_TIM_IT_NONE;
  timer_config.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  timer_config.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
  timer_config.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  timer_config.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  timer_config.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  timer_config.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D,&timer_config);

  /* -------------------------------------------------- */
  /* MASTER TIMER global configuration: same as TIMER D */
  /* -------------------------------------------------- */
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_MASTER,&timer_config);
  
  /* ------------------------------------------------------ */
  /* TD1 waveform description: set on period, reset on CMP1 */
  /* ------------------------------------------------------ */
  output_config_TDx.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config_TDx.SetSource = HRTIM_OUTPUTSET_TIMPER;
  output_config_TDx.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  output_config_TDx.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config_TDx.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config_TDx.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE;
  output_config_TDx.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config_TDx.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD1,
                                 &output_config_TDx);

  /* ------------------------------------------------------ */
  /* TD2 waveform description: set on CMP2, reset on period */
  /* Note: TDx init structure is re-used */
  /* ------------------------------------------------------ */
  output_config_TDx.SetSource = HRTIM_OUTPUTSET_MASTERPER;
  output_config_TDx.ResetSource  = HRTIM_OUTPUTSET_MASTERCMP1;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD2,
                                 &output_config_TDx);

  /* Set compare registers for duty cycle on TD1 in Timer D */
  compare_config.CompareValue = _100KHz_PERIOD/4;     /* 25% duty cycle */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* Set compare registers for duty cycle on TD2 in Master timer */
  compare_config.CompareValue = _100KHz_Plus_PERIOD/2;     /* 50% duty cycle */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_MASTER,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* ---------------*/
  /* HRTIM start-up */
  /* ---------------*/
  /* Enable HRTIM's outputs TD1 and TD2 */
  /* Note: it is necessary to enable also GPIOs to have outputs functional */
  /* This must be done after HRTIM initialization */
  HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TD1 + HRTIM_OUTPUT_TD2);

  /* Start HRTIM's Master TIMER and TIMER D*/
  HAL_HRTIM_WaveformCountStart(&hhrtim, HRTIM_TIMERID_MASTER + HRTIM_TIMERID_TIMER_D);

}
#endif /* PWM_MASTER */


#ifdef ARBITRARY_WAVEFORM
/**
* @brief  HRTIM configuration for generating an arbitrary waveform
* @param  None
* @retval None
*/
static void HRTIM_Config_ArbitraryWaveform(void)
{
  HRTIM_TimeBaseCfgTypeDef timebase_config;
  HRTIM_TimerCfgTypeDef timer_config;
    HRTIM_OutputCfgTypeDef output_config_TDx;
  HRTIM_CompareCfgTypeDef compare_config;

  /* ----------------------------------------------- */
  /* HRTIM Global initialization: Clock and DLL init */
  /* ----------------------------------------------- */
  /* Initialize the HRTIM structure (minimal configuration) */
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
    Error_Handler(); /* if DLL or clock is not correctly set */
  }
  

  /* ------------------------------------------------------------ */
  /* TIMERD initialization: set PWM frequency and continuous mode */
  /* ------------------------------------------------------------ */
  timebase_config.Period = _100KHz_PERIOD;
  timebase_config.RepetitionCounter = 0;
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, &timebase_config);
 
  /* --------------------------------------------------------- */
  /* TIMERD global configuration: preload enabled on REP event */
  /* --------------------------------------------------------- */
  timer_config.DMARequests = HRTIM_TIM_DMA_NONE;
  timer_config.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
  timer_config.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  timer_config.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  timer_config.DACSynchro = HRTIM_DACSYNC_NONE;
  timer_config.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  timer_config.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  timer_config.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  timer_config.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
  timer_config.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  timer_config.InterruptRequests = HRTIM_TIM_IT_NONE;
  timer_config.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  timer_config.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
  timer_config.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  timer_config.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  timer_config.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  timer_config.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D,&timer_config);
 
  /* --------------------------------------------------------------------- */
  /* TD1 waveform description: toggles on TIMD period, CMP1 and CMP2 event */
  /* --------------------------------------------------------------------- */
  output_config_TDx.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config_TDx.SetSource = HRTIM_OUTPUTSET_TIMPER + HRTIM_OUTPUTSET_TIMCMP1 + HRTIM_OUTPUTSET_TIMCMP2;
  output_config_TDx.ResetSource  = HRTIM_OUTPUTRESET_TIMPER + HRTIM_OUTPUTRESET_TIMCMP1 + HRTIM_OUTPUTRESET_TIMCMP2;
  output_config_TDx.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config_TDx.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config_TDx.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE;
  output_config_TDx.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config_TDx.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD1,
                                 &output_config_TDx);

  /* ------------------------------------------------------ */
  /* TD2 waveform description: set on TIMD PER and CMP2 and */
  /* reset on TIMD CMP1 and CMP3 event                      */
  /* Note: TDx init structure is re-used                    */
  /* ------------------------------------------------------ */
  output_config_TDx.SetSource = HRTIM_OUTPUTSET_TIMPER + HRTIM_OUTPUTSET_TIMCMP2;
  output_config_TDx.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1 + HRTIM_OUTPUTRESET_TIMCMP3;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD2,
                                 &output_config_TDx);

  /* Set compare register 1 in Timer D */
  compare_config.CompareValue = _100KHz_PERIOD/4;     /* 25% of the period */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* Set compare register 2 in Timer D */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (3*_100KHz_PERIOD)/8;     /* 37.5% of the period */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

    /* Set compare register 3 in Timer D */
  compare_config.CompareValue = _100KHz_PERIOD/2;     /* 50% of the period */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  /* ---------------*/
  /* HRTIM start-up */
  /* ---------------*/
  /* Enable HRTIM's outputs TD1 and TD2 */
  /* Note: it is necessary to enable also GPIOs to have outputs functional */
  /* This must be done after HRTIM initialization */
  HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TD1 + HRTIM_OUTPUT_TD2);

  /* Start HRTIM's TIMER D */
  HAL_HRTIM_WaveformCountStart(&hhrtim, HRTIM_TIMERID_TIMER_D);

}
#endif /* ARBITRARY_WAVEFORM */

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
