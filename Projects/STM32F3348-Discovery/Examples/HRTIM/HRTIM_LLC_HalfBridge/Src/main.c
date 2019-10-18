/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_LLC_HalfBridge/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a HRTIM initialization example for LLC 
  * half-bridge converter
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

/** @addtogroup HRTIM_LLC_HalfBridge
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
#define DLL_CALIBRATIONTIMEOUT ((uint32_t)   10)        /* Timeout in ms */

/* Define the interrupt rate vs switching frequency */
#define REPETITION_RATE ((uint32_t)   15)

/* Define the Over-curent blanking window */
/* 0x1000 x 217ps = 888ns */
#define OVER_CURRENT_BLANKING ((uint32_t) 0x1000)


/* Deadtime values for buck and boost stages, set to 200ns for both */
/* 230 x 868ps = 200ns */
#define DT_RISING       ((uint16_t)230)
#define DT_FALLING      ((uint16_t)230)

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

static void GPIO_LLC_HB_outputs_Config(void);
static void HRTIM_Config(void);
static void ADC_Config(void);
static void COMP2_Config(void);
static void DAC1_CH2_Config(void);

/* Private functions ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
HRTIM_HandleTypeDef hhrtim;
ADC_HandleTypeDef AdcHandle1;
ADC_HandleTypeDef AdcHandle2;
COMP_HandleTypeDef   Comp2Handle;
DAC_HandleTypeDef    DacHandle;


DemoModeTypeDef DemoState;

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

  /* Initialize STM32F3348-DISCO LEDs */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  /* Configure the system clock to have a system clock = 72 Mhz */
  SystemClock_Config();

  /* Initialize User_Button on STM32F3348-DISCO */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);   

  /* Demo directly starts in RUN mode */
  DemoState = RUN;
  
  /* Initialize DAC1_CH2 for over-current threshold on COMP6 */
  DAC1_CH2_Config();
  
  /* Initialize COMP2 for over-current protection */
  COMP2_Config();

  /* Initialize HRTIM and related inputs */
  HRTIM_Config();

  /* Initialize ADC to be triggered by the HRTIMER */
  ADC_Config();
  
  /* Initialize outputs (it has to be done after HRTIM init) */
  GPIO_LLC_HB_outputs_Config();
  
  /* HRTIM start-up */
  /* Enable HRTIM's outputs TA1, TA2, TB1 and TB2 */
  HAL_HRTIM_WaveformOutputStart(&hhrtim,
                                  HRTIM_OUTPUT_TA1
                                | HRTIM_OUTPUT_TA2
                                | HRTIM_OUTPUT_TB1
                                | HRTIM_OUTPUT_TB2);

  /* Start HRTIM's Master timer, TIMER A and B */
  HAL_HRTIM_WaveformCountStart_IT(&hhrtim, HRTIM_TIMERID_MASTER | HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_C);

  
  /* Infinite loop */
  while (1)
  {

    /* ---------------- */
    /* Fault management */
    /* ---------------- */
    if ((__HAL_HRTIM_GET_FLAG(&hhrtim, HRTIM_FLAG_FLT1) == SET)
     || (__HAL_HRTIM_TIMER_GET_FLAG(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_TIM_IT_DLYPRT)))
    {
      DemoState = FAULT;
    }

    /* ---------------- */
    /* Normal operation */
    /* ---------------- */
    switch(DemoState)
    {
      case RUN:
        BSP_LED_Toggle(LED5);
        BSP_LED_Off(LED3);
        break;

    case FAULT:
      /* If fault is triggered, operation can be re-started with the push-button */
      if((BSP_PB_GetState(BUTTON_USER) == SET))
      {
        __HAL_HRTIM_CLEAR_IT(&hhrtim, HRTIM_IT_FLT1);
        __HAL_HRTIM_TIMER_CLEAR_FLAG(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_TIM_IT_DLYPRT);
        HAL_HRTIM_WaveformOutputStart(&hhrtim,
                                     HRTIM_OUTPUT_TA1
                                   | HRTIM_OUTPUT_TA2
                                   | HRTIM_OUTPUT_TB1
                                   | HRTIM_OUTPUT_TB2);
        DemoState = RUN;
        while(BSP_PB_GetState(BUTTON_USER) == SET);
      }
      BSP_LED_Toggle(LED3);
      BSP_LED_Off(LED5);
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
* @brief  Configure GPIO outputs for the LLC converter
* @param  None
* @retval None
*/
static void GPIO_LLC_HB_outputs_Config(void)
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
* @brief  HRTIM configuration for both primary and secondary side switches
* Includes also time critical ADC triggers set-up
* Uses master and TIMER A, B and C
* Timer D and E are completely free and available for other tasks or converters
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
  HRTIM_TimerEventFilteringCfgTypeDef TimerEventFiltering_config;
  HRTIM_EventCfgTypeDef pEventCfg;

  /* ----------------------------*/
  /* HRTIM Global initialization */
  /* ----------------------------*/
  /* Initialize the HRTIM handle */
  hhrtim.Instance = HRTIM1;

  /* Initialize HRTIM */
  HAL_HRTIM_Init(&hhrtim);

  /* HRTIM DLL calibration: periodic calibration, set period to 14µs */
  HAL_HRTIM_DLLCalibrationStart(&hhrtim, HRTIM_CALIBRATIONRATE_14);
  /* Wait calibration completion*/
  if (HAL_HRTIM_PollForDLLCalibration(&hhrtim, DLL_CALIBRATIONTIMEOUT) != HAL_OK)
  {
    Error_Handler(); /* if DLL or clock is not correctly set */
  }
  
  /* ------------------------------------------------------------------------ */
  /* Master timer and Timer A initialization: timer mode and PWM frequency    */
  /* The 2 timers are running with identical parameters.                      */
  /* ------------------------------------------------------------------------ */
  timebase_config.Period = LLC_PWM_PERIOD;
  timebase_config.RepetitionCounter = 0; /* 1st repetition event is immediate */
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_MASTER, &timebase_config);
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &timebase_config);

  /* Timer B is managed differently, so that TIMERB period event can serve as */
  /* ADC trigger (to optimize timer's ressources). TimerBPeriod event is used */
  /* as 4th sampling point for SR1 monitoring */
  /* Period event is used ampling after turn-off on SR1 */
  timebase_config.Period = (35 * LLC_PWM_PERIOD)/100 + ADC_CONVERSION_TIME;
  timebase_config.Mode = HRTIM_MODE_SINGLESHOT_RETRIGGERABLE;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, &timebase_config);
  
  /* Timer C is managed differently, so that TIMERC period event can serve as */
  /* ADC trigger (to optimize timer's ressources). TimerCPeriod event is used */
  /* as 4th sampling point for SR2 monitoring */
  timebase_config.Period = (85 * LLC_PWM_PERIOD)/100 + ADC_CONVERSION_TIME;
  timebase_config.Mode = HRTIM_MODE_SINGLESHOT_RETRIGGERABLE;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_C, &timebase_config);
  
  /* ---------------------------------------------------------- */
  /* Master timer initialization                                */
  /* Update on repetition, interrupt issued on repetition event */
  /* Master timer is used for primary side switches             */
  /* ---------------------------------------------------------- */
  timer_config.DMARequests = HRTIM_TIM_DMA_NONE;
  timer_config.DMASrcAddress = 0x0;
  timer_config.DMADstAddress = 0x0;
  timer_config.DMASize = 0x0;
  timer_config.HalfModeEnable = HRTIM_HALFMODE_ENABLED;
  timer_config.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  timer_config.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  timer_config.DACSynchro = HRTIM_DACSYNC_NONE;
  timer_config.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  timer_config.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  timer_config.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  timer_config.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
  timer_config.InterruptRequests = HRTIM_MASTER_IT_MREP;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_MASTER, &timer_config);
  
  /* Set compare registers for Turn-on on SR2 */
  compare_config.CompareValue = (55 * LLC_PWM_PERIOD)/100;     /* turn-on at 55% */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_MASTER,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);
  
  /* Set compare registers for Turn-on on SR2 */
  compare_config.CompareValue = (95 * LLC_PWM_PERIOD)/100;     /* turn-on at 95% */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_MASTER,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);
  
  /* --------------------------------------------------------------------- */
  /* TIMERA global configuration: cnt reset, sync, update, fault, burst... */
  /* timer running in continuous mode, running at twice the LLC frequency  */
  /* reset by master CMP1                                                  */
  /* Timer A is used for overcurrent blanking, TA1 and TA2 are driven by the master timer */
  /* Delayed Idle protection is implemented on Timer A                     */
  /* Timer B is used for the synchronous rectification switches            */
  /* Timer C is used for providing additional sampling points              */
  /* --------------------------------------------------------------------- */
  timer_config.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
  timer_config.StartOnSync = HRTIM_SYNCSTART_DISABLED;
  timer_config.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
  timer_config.DACSynchro = HRTIM_DACSYNC_NONE;
  timer_config.PreloadEnable = HRTIM_PRELOAD_ENABLED;
  timer_config.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
  timer_config.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  timer_config.RepetitionUpdate = HRTIM_UPDATEONREPETITION_DISABLED;
  timer_config.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  timer_config.InterruptRequests = HRTIM_TIM_IT_NONE;
  timer_config.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  timer_config.FaultEnable = HRTIM_TIMFAULTENABLE_FAULT1;
  timer_config.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  timer_config.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_ENABLED;
  timer_config.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DELAYEDBOTH_EEV6;
  timer_config.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_MASTER;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_MASTER_PER + HRTIM_TIMRESETTRIGGER_MASTER_CMP1;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &timer_config);
  
  timer_config.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_MASTER_PER;
  timer_config.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, &timer_config);
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_C, &timer_config);
  
  /* ----------------------------------------------- */
  /* Configure Master timer compare events           */
  /* Compare 1 is automatically computed (Half mode) */
  /* ----------------------------------------------- */

  /* Compare 2 is used as ADC trigger for SR1 sampling before turn-off */
  /* 1st value is set above period to avoid a 1st ADC sampling */

  compare_config.CompareValue = LLC_PWM_PERIOD + 1;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_MASTER,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

  /* Compare 3 is used for SR2 turn-on */
  compare_config.CompareValue = (65 * LLC_PWM_PERIOD)/100;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_MASTER,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  /* Compare 4 is used for SR2 turn-off */
  compare_config.CompareValue = (85 * LLC_PWM_PERIOD)/100;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_MASTER,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);
  
  /* --------------------------------- */
  /* TA1 and TA2 waveforms description */
  /* --------------------------------- */
  output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config.SetSource = HRTIM_OUTPUTSET_MASTERPER;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_MASTERCMP1;
  output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_A,
                                 HRTIM_OUTPUT_TA1,
                                 &output_config);

  /* For debug purposes only, to monitor the blanking window */
  /* In this case, deadtime must be disabled and TA1 forced at reset 
  (conflict when both TA1 and TA2 are both set due to discovery hardware)
  output_config.SetSource = HRTIM_OUTPUTSET_MASTERPER + HRTIM_OUTPUTSET_MASTERCMP1;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
*/
  
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_A,
                                 HRTIM_OUTPUT_TA2,
                                 &output_config);

  /* Set compare registers for blanking window */
  __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, OVER_CURRENT_BLANKING);

  /* Configure deadtime for primary side switches */
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

  
  /* --------------------------------- */
  /* TB1 and TB2 waveforms description */
  /* --------------------------------- */
  output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config.SetSource = HRTIM_OUTPUTSET_TIMCMP1;
  output_config.ResetSource  = HRTIM_OUTPUTSET_TIMCMP2;
  output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
  output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_B,
                                 HRTIM_OUTPUT_TB1,
                                 &output_config);

  output_config.SetSource = HRTIM_OUTPUTSET_MASTERCMP3;
  output_config.ResetSource  = HRTIM_OUTPUTSET_MASTERCMP4;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_B,
                                 HRTIM_OUTPUT_TB2,
                                 &output_config);

  /* Set compare registers for turn-on time on TB1 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (15 * LLC_PWM_PERIOD)/100;  /* Turn-on at 5% */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_B,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* Set compare registers for turn-off time on TB1 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (35 * LLC_PWM_PERIOD)/100;  /* Turn-off at 45% */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_B,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

  /* Set compare 3 for sampling before turn-on on SR1 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (15 * LLC_PWM_PERIOD)/100 - ADC_CONVERSION_TIME;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_B,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  /* Set compare 4 for sampling before turn-off on SR1 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (15 * LLC_PWM_PERIOD)/100 + ADC_CONVERSION_TIME;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_B,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);


  /* ------------------------------------------------------------------------------- */
  /* Timer C compare register set-up for SR2 turn-on and ADC sampling points for SR2 */
  /* ------------------------------------------------------------------------------- */

  /* Set compare registers for 1st sample before SR2 turn-on */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (65 * LLC_PWM_PERIOD)/100 - ADC_CONVERSION_TIME;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_C,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

  /* Set compare registers for 2nd sample after SR2 turn-on */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (65 * LLC_PWM_PERIOD)/100 + ADC_CONVERSION_TIME;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_C,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  /* Set compare registers for 3rd sample before SR2 turn-off */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = (85 * LLC_PWM_PERIOD)/100 - ADC_CONVERSION_TIME;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_C,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);

  /* ------------------------------------------- */
  /* ADC1 trigger intialization (with 4 event)   */
  /* 4 injected events for SR1 monitoring        */
  /* ------------------------------------------- */
  adc_trigger_config.Trigger = HRTIM_ADCTRIGGEREVENT24_TIMERB_CMP3 
                             + HRTIM_ADCTRIGGEREVENT24_TIMERB_CMP4
                             + HRTIM_ADCTRIGGEREVENT24_MASTER_CMP2
                             + HRTIM_ADCTRIGGEREVENT24_TIMERB_PERIOD;

  adc_trigger_config.UpdateSource = HRTIM_ADCTRIGGERUPDATE_MASTER;
  HAL_HRTIM_ADCTriggerConfig(&hhrtim,
                             HRTIM_ADCTRIGGER_2,
                             &adc_trigger_config);

  /* ------------------------------------------- */
  /* ADC2 trigger intialization (with 4 event)   */
  /* 4 injected events for SR2 monitoring        */
  /* ------------------------------------------- */
  adc_trigger_config.Trigger = HRTIM_ADCTRIGGEREVENT24_TIMERC_CMP2 
                             + HRTIM_ADCTRIGGEREVENT24_TIMERC_CMP3
                             + HRTIM_ADCTRIGGEREVENT24_TIMERC_CMP4
                             + HRTIM_ADCTRIGGEREVENT24_TIMERC_PERIOD;

  adc_trigger_config.UpdateSource = HRTIM_ADCTRIGGERUPDATE_MASTER;
  HAL_HRTIM_ADCTriggerConfig(&hhrtim,
                             HRTIM_ADCTRIGGER_4,
                             &adc_trigger_config);

  /* ---------------------*/
  /* EVENT initialization */
  /* ---------------------*/
  HAL_HRTIM_EventPrescalerConfig(&hhrtim, HRTIM_EVENTPRESCALER_DIV1);
  /* OC event is rising edge sensitive */
  pEventCfg.Source = HRTIM_EVENTSRC_2;
  pEventCfg.Polarity = HRTIM_EVENTPOLARITY_HIGH;
  pEventCfg.Sensitivity = HRTIM_EVENTSENSITIVITY_LEVEL;
  pEventCfg.FastMode = HRTIM_EVENTFASTMODE_DISABLE;
  HAL_HRTIM_EventConfig(&hhrtim, HRTIM_EVENT_6, &pEventCfg);
  
  /* OC (EEV6 signal blanking done on TACMP1 */
  TimerEventFiltering_config.Filter = HRTIM_TIMEVENTFILTER_BLANKINGCMP1;
  TimerEventFiltering_config.Latch = HRTIM_TIMEVENTLATCH_DISABLED;
  HAL_HRTIM_TimerEventFilteringConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_A,
                                  HRTIM_EVENT_6,
                                  &TimerEventFiltering_config);

  
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
  
  /* ---------------------------------------------- */
  /* Force a global register update before starting */
  /* ---------------------------------------------- */
  HAL_HRTIM_SoftwareUpdate(&hhrtim,
                        HRTIM_TIMERUPDATE_MASTER
                      + HRTIM_TIMERUPDATE_A
                      + HRTIM_TIMERUPDATE_B
                      + HRTIM_TIMERUPDATE_C
                      + HRTIM_TIMERUPDATE_D
                      + HRTIM_TIMERUPDATE_E);

  /* ------------------------------------------------ */
  /* Prepare new values for steady state operation    */
  /* They will be taken into account after 1st update */
  /* ------------------------------------------------ */
  /* Compare 2 is used as ADC trigger for SR1 sampling before turn-off */
  compare_config.CompareValue = (35 * LLC_PWM_PERIOD)/100 - ADC_CONVERSION_TIME;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_MASTER,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

  HRTIM1->sMasterRegs.MREP = REPETITION_RATE;  /* Nominal repetition rate */
  
}

/**
  * @brief  Configure ADC1 and ADC2 for being used with HRTIM
  * For each ADC, 4 injected channels are used
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_MultiModeTypeDef MultiModeConfig;
  ADC_InjectionConfTypeDef InjectionConfig;

  AdcHandle1.Instance = ADC1;
  AdcHandle2.Instance = ADC2;

  /* ADC1 and ADC2 are working independently */
  MultiModeConfig.DMAAccessMode = ADC_DMAACCESSMODE_DISABLED;
  MultiModeConfig.Mode = ADC_MODE_INDEPENDENT;
  MultiModeConfig.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_1CYCLE;
  HAL_ADCEx_MultiModeConfigChannel(&AdcHandle1, &MultiModeConfig);
  
  /* ADC1 global initialization */
  /* 12-bit right-aligned format, discontinuous scan mode, running from PLL */
  AdcHandle1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  AdcHandle1.Init.Resolution = ADC_RESOLUTION_12B;
  AdcHandle1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle1.Init.ScanConvMode = ENABLE;
  AdcHandle1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  AdcHandle1.Init.LowPowerAutoWait = DISABLE;
  AdcHandle1.Init.ContinuousConvMode = DISABLE;
  AdcHandle1.Init.NbrOfConversion = 1;
  AdcHandle1.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle1.Init.NbrOfDiscConversion = 1;
  AdcHandle1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  AdcHandle1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle1.Init.DMAContinuousRequests = DISABLE;
  AdcHandle1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  HAL_ADC_Init(&AdcHandle1);
  
  /* Very same init for the 2nd ADC*/
  /* ADC1 global initialization */
  /* 12-bit right-aligned format, discontinuous scan mode, running from PLL */
  AdcHandle2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  AdcHandle2.Init.Resolution = ADC_RESOLUTION_12B;
  AdcHandle2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle2.Init.ScanConvMode = ENABLE;
  AdcHandle2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  AdcHandle2.Init.LowPowerAutoWait = DISABLE;
  AdcHandle2.Init.ContinuousConvMode = DISABLE;
  AdcHandle2.Init.NbrOfConversion = 0;
  AdcHandle2.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle2.Init.NbrOfDiscConversion = 1;
  AdcHandle2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  AdcHandle2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle2.Init.DMAContinuousRequests = DISABLE;
  AdcHandle2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  HAL_ADC_Init(&AdcHandle2);

  /* Discontinuous injected mode: 1st injected conversion on Ch3 */
  InjectionConfig.AutoInjectedConv = DISABLE;
  InjectionConfig.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_HRTIM_TRG2;
  InjectionConfig.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
  InjectionConfig.InjectedChannel = ADC_CHANNEL_3;
  InjectionConfig.InjectedDiscontinuousConvMode = ENABLE;
  InjectionConfig.InjectedNbrOfConversion = 4;
  InjectionConfig.InjectedOffset = 0;
  InjectionConfig.InjectedOffsetNumber = ADC_OFFSET_NONE;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_1;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  InjectionConfig.InjectedSingleDiff = ADC_SINGLE_ENDED;
  InjectionConfig.QueueInjectedContext = DISABLE;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle1, &InjectionConfig);
  
  /* Configure the 2nd injected conversion on Ch3 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_3;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_2;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle1, &InjectionConfig);
  
  /* Configure the 3rd injected conversion on Ch3 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_3;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_3;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle1, &InjectionConfig);

  /* Configure the 4th injected conversion on Ch3 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_3;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_4;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle1, &InjectionConfig);

  /* The only difference for ADC2 is the trigger source */
  /* All other parameters are identical */
  /* Discontinuous injected mode: 1st injected conversion on Ch3 */
  InjectionConfig.AutoInjectedConv = DISABLE;
  InjectionConfig.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJECCONV_HRTIM_TRG4;
  InjectionConfig.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
  InjectionConfig.InjectedChannel = ADC_CHANNEL_2;
  InjectionConfig.InjectedDiscontinuousConvMode = ENABLE;
  InjectionConfig.InjectedNbrOfConversion = 4;
  InjectionConfig.InjectedOffset = 0;
  InjectionConfig.InjectedOffsetNumber = ADC_OFFSET_NONE;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_1;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  InjectionConfig.InjectedSingleDiff = ADC_SINGLE_ENDED;
  InjectionConfig.QueueInjectedContext = DISABLE;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle2, &InjectionConfig);
  
  /* Configure the 2nd injected conversion on Ch3 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_2;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_2;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle2, &InjectionConfig);
  
  /* Configure the 3rd injected conversion on Ch3 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_2;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_3;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle2, &InjectionConfig);

  /* Configure the 4th injected conversion on Ch3 */
  InjectionConfig.InjectedChannel = ADC_CHANNEL_2;
  InjectionConfig.InjectedRank = ADC_INJECTED_RANK_4;
  InjectionConfig.InjectedSamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  HAL_ADCEx_InjectedConfigChannel(&AdcHandle2, &InjectionConfig);

  
  /* Run the ADC calibration in single-ended mode */
  HAL_ADCEx_Calibration_Start(&AdcHandle1, ADC_SINGLE_ENDED);
  
  /* Very same init for the 2nd ADC*/
  HAL_ADCEx_Calibration_Start(&AdcHandle2, ADC_SINGLE_ENDED);

  /* Start ADC1 Injected Conversions */ 
  HAL_ADCEx_InjectedStart(&AdcHandle1);
  
  /* Start ADC2 Injected Conversions */ 
  HAL_ADCEx_InjectedStart(&AdcHandle2);

}



/**
  * @brief  Comparator 2 configuration for over-current protection on TA1/TA2
  * @param  None
  * @retval None
  */
void COMP2_Config(void)
{
 
  /*##-1- Configure the COMP2 peripheral ###################################*/
  Comp2Handle.Instance = COMP2;

  Comp2Handle.Init.InvertingInput    = COMP_INVERTINGINPUT_DAC1_CH2;
  Comp2Handle.Init.NonInvertingInput = COMP_NONINVERTINGINPUT_IO1;
  Comp2Handle.Init.Output            = COMP_OUTPUT_NONE;
  Comp2Handle.Init.OutputPol         = COMP_OUTPUTPOL_NONINVERTED;
  Comp2Handle.Init.BlankingSrce      = COMP_BLANKINGSRCE_NONE;
  Comp2Handle.Init.Hysteresis        = COMP_HYSTERESIS_NONE;
  Comp2Handle.Init.WindowMode        = COMP_WINDOWMODE_DISABLE;
  Comp2Handle.Init.TriggerMode       = COMP_TRIGGERMODE_NONE;
  HAL_COMP_Init(&Comp2Handle);

  /*##-2- Start the comparator process #####################################*/ 
  /* Enable COMP2 */
  HAL_COMP_Start(&Comp2Handle);

}


/**
  * @brief  DAC1 CH2 configuration used as COMP6 threshold
  * @param  None
  * @retval None
  */
void DAC1_CH2_Config(void)
{
  static DAC_ChannelConfTypeDef sConfig;
  
  /*##-1- Configure the DAC peripheral #######################################*/
  DacHandle.Instance = DAC1;

  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initiliazation Error */
    Error_Handler();
  }

  /*##-2- Configure DAC channel1 #############################################*/
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }

  /*##-3- Set DAC Channel1 DHR register ######################################*/
  if (HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x4FF) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }

  /*##-4- Enable DAC Channel1 ################################################*/
  if (HAL_DAC_Start(&DacHandle, DAC_CHANNEL_2) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
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
