/**
  ******************************************************************************
  * @file    HRTIM\HRTIM_Multiphase/Src/main.c 
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

/** @addtogroup HRTIM_Multiphase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* HRTIM initialisation done at register level (no HAL call) */
/* #define SNIPPET */

/* Private define ------------------------------------------------------------*/
#define DLL_CALIBRATIONTIMEOUT ((uint32_t)   10)        /* Timeout in ms */

/* Size of array aADCxConvertedValues[]: set to ADC sequencer number of ranks converted, to have a rank in each address */
#define ADCCONVERTEDVALUES_BUFFER_SIZE ((uint32_t)   5)

/* Define the interrupt rate vs switching frequency */
#define REPETITON_RATE ((uint32_t)   31)


#define BURST_PERIOD ((uint32_t)   1)
#define BURST_IDLE ((uint32_t)   0)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Buffer containing ADC conversions results */
__IO uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];


/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

static void GPIO_Multiphase_outputs_Config(void);
static void ADC_Config(void);

#ifdef SNIPPET
static void HRTIM_Config_NoHAL(void);
#else /* HAL */
static void HRTIM_Config(void);
#endif


/* Private functions ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
HRTIM_HandleTypeDef hhrtim;
ADC_HandleTypeDef AdcHandle;
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

  /* Initialize ADC to be triggered by the HRTIMER */
  ADC_Config();

  /* Demo starts in 5-phase mode */
  DemoState = FIVE_PHASE;
  
  /* Initialize HRTIM and related inputs */
  #ifdef SNIPPET
    HRTIM_Config_NoHAL();
  #else
  HRTIM_Config();
  #endif

  
  /* Initialize Multiphase outputs (it has to be done after HRTIM init) */
  GPIO_Multiphase_outputs_Config();
  
  /* -------------- */
  /* HRTIM start-up */
  /* -------------- */
  #ifdef SNIPPET
  
  /* Enable HRTIM's outputs TA2, TB1, TC2, TD1, TD2 */
    HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TA2OEN | HRTIM_OENR_TB1OEN | HRTIM_OENR_TC2OEN | HRTIM_OENR_TD1OEN | HRTIM_OENR_TD2OEN;
    /* Start HRTIM's TIMER A, B, C, D */
    HRTIM1->sMasterRegs.MCR |= HRTIM_MCR_MCEN | HRTIM_MCR_TACEN | HRTIM_MCR_TBCEN | HRTIM_MCR_TCCEN | HRTIM_MCR_TDCEN;
    
  #else

    /* Enable HRTIM's outputs TA2, TB1, TC2, TD1, TD2 */
    HAL_HRTIM_WaveformOutputStart(&hhrtim, HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TC2 | HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2);
  /* Start HRTIM's TIMER A, B, C, D */
  HAL_HRTIM_WaveformCountStart_IT(&hhrtim, HRTIM_TIMERID_MASTER | HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B | HRTIM_TIMERID_TIMER_C | HRTIM_TIMERID_TIMER_D);

  #endif
  
  /* Infinite loop */
  while (1)
  {

    /* ---------------- */
    /* Fault management */
    /* ---------------- */
    #ifdef SNIPPET
      if(HRTIM1->sCommonRegs.ISR & HRTIM_ICR_FLT1C)
    #else /* HAL version */
    if (__HAL_HRTIM_GET_FLAG(&hhrtim, HRTIM_FLAG_FLT1) == SET)
    #endif
    {
      DemoState = FAULT;
    }

    /* ---------------- */
    /* Normal operation */
    /* ---------------- */
    switch(DemoState)
    {
    case FIVE_PHASE:
      /* Phase shedding is done in the Master timer IRQ */
      if (BSP_PB_GetState(BUTTON_USER) == SET)
      {
        DemoState = FOUR_PHASE_INIT;
        while(BSP_PB_GetState(BUTTON_USER) == SET);
      }
      BSP_LED_Toggle(LED5);
      BSP_LED_Off(LED6);
      break;
 
    case FOUR_PHASE:
      if (BSP_PB_GetState(BUTTON_USER) == SET)
      {
        DemoState = THREE_PHASE_INIT;
        while(BSP_PB_GetState(BUTTON_USER) == SET);
      }
      BSP_LED_Off(LED5);
      BSP_LED_Toggle(LED6);
      break;

    case THREE_PHASE:
      if (BSP_PB_GetState(BUTTON_USER) == SET)
      {
        DemoState = TWO_PHASE_INIT;
        while(BSP_PB_GetState(BUTTON_USER) == SET);
      }
      BSP_LED_On(LED5);
      BSP_LED_Off(LED6);
      break;

    case TWO_PHASE:
      if (BSP_PB_GetState(BUTTON_USER) == SET)
      {
        DemoState = ONE_PHASE_INIT;
        while(BSP_PB_GetState(BUTTON_USER) == SET);
      }
      BSP_LED_Off(LED5);
      BSP_LED_On(LED6);
      break;

    case ONE_PHASE:
      if (BSP_PB_GetState(BUTTON_USER) == SET)
      {
        DemoState = BURST_INIT;
        while(BSP_PB_GetState(BUTTON_USER) == SET);
      }
      BSP_LED_On(LED5);
      BSP_LED_On(LED6);
      break;

    case BURST:
      if (BSP_PB_GetState(BUTTON_USER) == SET)
      {
        DemoState = FIVE_PHASE_INIT;
        while(BSP_PB_GetState(BUTTON_USER) == SET);
      }
      BSP_LED_Toggle(LED5);
      BSP_LED_Toggle(LED6);
      break;

    case FIVE_PHASE_INIT:
    case FOUR_PHASE_INIT:
    case THREE_PHASE_INIT:
    case TWO_PHASE_INIT:
    case ONE_PHASE_INIT:
    case BURST_INIT:
    break;

    case FAULT:
      /* If fault is triggered, operation can be re-started with the push-button */
      if((BSP_PB_GetState(BUTTON_USER) == SET))
      {
        #ifdef SNIPPET
          HRTIM1->sCommonRegs.ICR = HRTIM_ICR_FLT1C;
          /* Re-enable TA1 and TA2 */
          HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TA2OEN | HRTIM_OENR_TB1OEN | HRTIM_OENR_TC2OEN | HRTIM_OENR_TD1OEN | HRTIM_OENR_TD2OEN;
        #else /* HAL version */
        __HAL_HRTIM_CLEAR_IT(&hhrtim, HRTIM_IT_FLT1);
        HAL_HRTIM_WaveformOutputStart(&hhrtim,
                                  HRTIM_OUTPUT_TA2
                                | HRTIM_OUTPUT_TB1
                                | HRTIM_OUTPUT_TC2
                                | HRTIM_OUTPUT_TD1
                                | HRTIM_OUTPUT_TD2);
        #endif

        DemoState = FIVE_PHASE_INIT;
        while(BSP_PB_GetState(BUTTON_USER) == SET);
      }
      BSP_LED_Toggle(LED3);
      BSP_LED_Off(LED5);
      BSP_LED_Off(LED6);
      break;

    default:
      #ifdef SNIPPET
          HRTIM1->sCommonRegs.ODISR = HRTIM_OENR_TA2OEN | HRTIM_OENR_TB1OEN | HRTIM_OENR_TC2OEN | HRTIM_OENR_TD1OEN | HRTIM_OENR_TD2OEN;     
      #else
      HAL_HRTIM_WaveformOutputStop(&hhrtim,
                                  HRTIM_OUTPUT_TA2
                                | HRTIM_OUTPUT_TB1
                                | HRTIM_OUTPUT_TC2
                                | HRTIM_OUTPUT_TD1
                                | HRTIM_OUTPUT_TD2);
      #endif

      break;
    }

    /* This delay defines the main loop time and LED blinking rate*/
    HAL_Delay(50);
    
  }
}


/**
* @brief  Configure GPIO outputs for the Multiphase converter
* @param  None
* @retval None
*/
static void GPIO_Multiphase_outputs_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIOA clock for timer A, B outputs */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Enable GPIOB clock for timer C, D outputs */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure HRTIM output: TA2 (PA9) */
  GPIO_InitStruct.Pin = GPIO_PIN_9; 
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;;  
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;;  
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
 
  /* Configure HRTIM output: TB1 (PA10) */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure HRTIM output: TC2 (PB13) */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Configure HRTIM output: TD1 (PB14) */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Configure HRTIM output: TD2 (PB15) */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
  
  /* Configure and enable HRTIM Master interrupt channel in NVIC */
  HAL_NVIC_SetPriority(HRTIM1_Master_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(HRTIM1_Master_IRQn);

  /* --------------------------*/
  /* HRTIM Clock and DLL init  */
  /* --------------------------*/

  /* Use the PLLx2 clock for HRTIM */
  __HAL_RCC_HRTIM1_CONFIG(RCC_HRTIM1CLK_PLLCLK);

  /* Enable HRTIM clock*/
  __HAL_RCC_HRTIM1_CLK_ENABLE();

  /* DLL calibration: periodic calibration enabled, period set to 14µs */
  HRTIM1->sCommonRegs.DLLCR = HRTIM_CALIBRATIONRATE_14 | HRTIM_DLLCR_CALEN;
    
  /* Check DLL end of calibration flag */
  while(HRTIM1->sCommonRegs.ISR & HRTIM_IT_DLLRDY == RESET);

  /* ---------------------------------------------------------------- */
  /* Master timer initialization: PWM frequency and mode (continuous) */
  /* Update on repetition, interrupt issued on repetition event       */
  /* ---------------------------------------------------------------- */
  HRTIM1->sMasterRegs.MPER = MULTIPHASE_PERIOD;
  HRTIM1->sMasterRegs.MREP = REPETITON_RATE; /* 1 ISR every REPETITON_RATE PWM periods */
  HRTIM1->sMasterRegs.MCR = HRTIM_MCR_CONT + HRTIM_MCR_PREEN + HRTIM_MCR_MREPU;
  HRTIM1->sMasterRegs.MDIER = HRTIM_MICR_MREP;
  
  /* Set compare registers for phase-shifts in master timer */
  /* Each compare is coding for the phase-shift of one phase */
  HRTIM1->sMasterRegs.MCMP1R = PHASE_SHIFT_INIT;
  HRTIM1->sMasterRegs.MCMP2R = 2 * PHASE_SHIFT_INIT;
  HRTIM1->sMasterRegs.MCMP3R = 3 * PHASE_SHIFT_INIT;
  HRTIM1->sMasterRegs.MCMP4R = 4 * PHASE_SHIFT_INIT;
 

  /* ---------------------------------------------------------------------- */
  /* TIMERA initialization: PWM frequency and Repetition rate               */
  /* Single-shot retriggerable mode, preload enabled on Master update event */
  /* Reset on Master period                                                 */
  /* ---------------------------------------------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].PERxR = MULTIPHASE_PERIOD - 1 - ADC_CONVERSION_TIME;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].REPxR = REPETITON_RATE; /* 1 ISR every REPETITON_RATE PWM periods */

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].TIMxCR = HRTIM_TIMCR_RETRIG
                                                       + HRTIM_TIMCR_PREEN
                                                       + HRTIM_TIMCR_MSTU;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].RSTxR = HRTIM_RSTR_MSTPER;

  /* ----------------------------------------------------------------- */
  /* TA2 waveforms description: Set on master period, reset on CMP1    */
  /* Fault1 selected, disabling TA2 output, IDLE active for burst mode */
  /* ----------------------------------------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].SETx2R = HRTIM_SET2R_MSTPER;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].RSTx2R = HRTIM_RST2R_CMP1;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].OUTxR = HRTIM_OUTR_FAULT2_1 + HRTIM_OUTR_IDLM2;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].FLTxR = HRTIM_FLTR_FLT1EN;
  
  /* Set compare registers for duty cycle on TA2 */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = DUTY_CYCLE_INIT;
  
  /* ----------------------------------- */
  /* TIMERB initialization: Idem Timer A */
  /* except Reset on Master Compare 1    */
  /* ----------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].PERxR = MULTIPHASE_PERIOD - 1 - ADC_CONVERSION_TIME;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].REPxR = REPETITON_RATE; /* 1 ISR every REPETITON_RATE PWM periods */

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].TIMxCR = HRTIM_TIMCR_RETRIG
                                                       + HRTIM_TIMCR_PREEN
                                                       + HRTIM_TIMCR_MSTU;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].RSTxR = HRTIM_RSTR_MSTCMP1;

  /* ----------------------------------------------------------------- */
  /* TB1 waveforms description: Set on master compare1, reset on CMP1  */
  /* Fault1 selected, disabling TB1 output, IDLE active for burst mode */
  /* ----------------------------------------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].SETx1R = HRTIM_SET1R_MSTCMP1;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].RSTx1R = HRTIM_RST1R_CMP1;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].OUTxR = HRTIM_OUTR_FAULT1_1 + HRTIM_OUTR_IDLM1;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].FLTxR = HRTIM_FLTR_FLT1EN;
  
  /* Set compare registers for duty cycle */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP1xR = DUTY_CYCLE_INIT;

  /* ----------------------------------- */
  /* TIMERC initialization: Idem Timer A */
  /* except Reset on Master Compare 2    */
  /* ----------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].PERxR = MULTIPHASE_PERIOD - 1 - ADC_CONVERSION_TIME;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].REPxR = REPETITON_RATE; /* 1 ISR every REPETITON_RATE PWM periods */

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].TIMxCR = HRTIM_TIMCR_RETRIG
                                                       + HRTIM_TIMCR_PREEN
                                                       + HRTIM_TIMCR_MSTU;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].RSTxR = HRTIM_RSTR_MSTCMP2;

  /* ----------------------------------------------------------------- */
  /* TC2 waveform description: Set on master compare2, reset on CMP1   */
  /* Fault1 selected, disabling TB1 output, IDLE active for burst mode */
  /* ----------------------------------------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].SETx2R = HRTIM_SET2R_MSTCMP2;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].RSTx2R = HRTIM_RST2R_CMP1;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].OUTxR = HRTIM_OUTR_FAULT2_1 + HRTIM_OUTR_IDLM2;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].FLTxR = HRTIM_FLTR_FLT1EN;
  
  /* Set compare registers for duty cycle */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP1xR = DUTY_CYCLE_INIT;


  /* ----------------------------------- */
  /* TIMERD initialization: Idem Timer A */
  /* except Reset on Master Compare 3    */
  /* ----------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].PERxR = MULTIPHASE_PERIOD - 1 - ADC_CONVERSION_TIME;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].REPxR = REPETITON_RATE; /* 1 ISR every REPETITON_RATE PWM periods */

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].TIMxCR = HRTIM_TIMCR_RETRIG
                                                       + HRTIM_TIMCR_PREEN
                                                       + HRTIM_TIMCR_MSTU;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTxR = HRTIM_RSTR_MSTCMP3;

  /* ----------------------------------------------------------------------- */
  /* TD1 and TD2 waveform description: Set on master compare2, reset on CMP1 */
  /* Fault1 selected, disabling TB1 output, IDLE active for burst mode       */
  /* ----------------------------------------------------------------------- */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx1R = HRTIM_SET1R_MSTCMP3;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx1R = HRTIM_RST1R_CMP1;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx2R = HRTIM_SET2R_MSTCMP4;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx2R = HRTIM_RST2R_CMP2;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].OUTxR = HRTIM_OUTR_FAULT2_1 
                                                      + HRTIM_OUTR_IDLM2
                                                      + HRTIM_OUTR_FAULT1_1
                                                      + HRTIM_OUTR_IDLM1;

  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].FLTxR = HRTIM_FLTR_FLT1EN;
  
  /* Set compare registers for duty cycle */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP1xR = DUTY_CYCLE_INIT;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP2xR = PHASE_SHIFT_INIT + DUTY_CYCLE_INIT;

  /* -------------------------------------------------- */
  /* Burst mode configuration for light load management */
  /* Trigger is master reset/roll-over event            */
  /* Preload enabled, continous mode                    */
  /* -------------------------------------------------- */ 
  HRTIM1->sCommonRegs.BMCR = HRIM_BURSTMODEPRELOAD_ENABLED + HRTIM_BURSTMODE_CONTINOUS;
  HRTIM1->sCommonRegs.BMTRGR = HRTIM_BURSTMODETRIGGER_MASTER_RESET;
  HRTIM1->sCommonRegs.BMPER = BURST_PERIOD;     /* burst mode period */
  HRTIM1->sCommonRegs.BMCMPR = BURST_IDLE;      /* burst mode compare value */
  
  /* ------------------------------------------- */
  /* ADC trigger intialization (with CMP2 event) */
  /* ------------------------------------------- */
  /* Default sampling points for 5-phase configuration */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP2xR = DUTY_CYCLE_INIT/2; /* Samples at 50% of Ton time */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP2xR = DUTY_CYCLE_INIT/2; /* Samples at 50% of Ton time */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP2xR = DUTY_CYCLE_INIT/2; /* Samples at 50% of Ton time */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP3xR = DUTY_CYCLE_INIT/2; /* Samples at 50% of Ton time */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP4xR = PHASE_SHIFT_INIT + (DUTY_CYCLE_INIT/2); /* Samples at 50% of Ton time on phase 5 */

  /* Dummy triggers for 3-phase configuration, using Timer C compare 3 and 4 */
  /* Respectively 250ns and 500ns before multiphase period end*/
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP3xR = (MULTIPHASE_PERIOD/3) - 1  - ADC_CONVERSION_TIME;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP4xR = (MULTIPHASE_PERIOD/3) - 1 - (ADC_CONVERSION_TIME*2);
  
  /* Dummy triggers for 2-phase configuration */
  /* Configure Timer B compare 3 and 4 and timer A compare 3 for a dummy 3rd, 4th and 5th conversion */
  /* Respectively 250ns, 500ns and 750ns before multiphase period end*/
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP3xR = (MULTIPHASE_PERIOD/2) - 1 - ADC_CONVERSION_TIME;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP4xR = (MULTIPHASE_PERIOD/2) - 1 - (ADC_CONVERSION_TIME*2);
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP3xR = MULTIPHASE_PERIOD - 1 - (ADC_CONVERSION_TIME*3);
  
  /* Dummy triggers for 1-phase configuration */
  /* Configure Timer A compare 3 and 4 for dummy 2nd and 3rd conversion */
  /* Dummy 4th conversion triggered by Timer A period event */
  /* Respectively 250ns, 500ns and 750ns before multiphase period end*/
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP4xR = MULTIPHASE_PERIOD - 1 - (ADC_CONVERSION_TIME*2);
    
    
  HRTIM1->sCommonRegs.CR1 = 0; /* ADC trigger 1 update source: Master */

  /* Use 5 trigger sources, one per phase */
  HRTIM1->sCommonRegs.ADC1R = HRTIM_ADC1R_AD1TAC2 /* ADC trigger event: Timer A compare 2 */
                            + HRTIM_ADC1R_AD1TBC2 /* ADC trigger event: Timer B compare 2 */
                            + HRTIM_ADC1R_AD1TCC2 /* ADC trigger event: Timer C compare 2 */
                            + HRTIM_ADC1R_AD1TDC3 /* ADC trigger event: Timer D compare 3 */
                            + HRTIM_ADC1R_AD1TDC4; /* ADC trigger event: Timer D compare 4 */

  /* ---------------------------------------------------------- */
  /* FAULT1 global init: no filter, low polarity, Fault1 enable */
  /* ---------------------------------------------------------- */
  HRTIM1->sCommonRegs.FLTINR1 = HRTIM_FLTINR1_FLT1E;

  /* ---------------*/
  /* HRTIM start-up */
  /* ---------------*/
  /* Force register update before starting */
  HRTIM1->sCommonRegs.CR2 |= HRTIM_CR2_TASWU
                           + HRTIM_CR2_TBSWU
                           + HRTIM_CR2_TCSWU
                           + HRTIM_CR2_TDSWU;
  
}
#else /* HAL version */
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
  HRTIM_ADCTriggerCfgTypeDef adc_trigger_config;
  HRTIM_FaultCfgTypeDef fault_config;
  HRTIM_BurstModeCfgTypeDef burst_config;

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

  /* ---------------------------------------------------------------- */
  /* Master timer initialization: PWM frequency and mode (continuous) */
  /* ---------------------------------------------------------------- */
  timebase_config.Period = MULTIPHASE_PERIOD; /*  */
  timebase_config.RepetitionCounter = REPETITON_RATE; /* 1 ISR every REPETITON_RATE PWM periods */
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_CONTINUOUS;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_MASTER, &timebase_config);
  
  /* ---------------------------------------------------------- */
  /* Master timer initialization                                */
  /* Update on repetition, interrupt issued on repetition event */
  /* ---------------------------------------------------------- */
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
  timer_config.InterruptRequests = HRTIM_MASTER_IT_MREP;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_MASTER, &timer_config);
  
  /* -------------------------------------------------------------- */
  /* TIMERA, TIMERB, TIMERC, TIMERD initialization: timer mode and PWM frequency */
  /* The 4 timers are running with identical parameters.            */
  /* -------------------------------------------------------------- */
  /* Rationale for period value: Timer A period event is used as event for ADC sampling */
  /* during phase shedding (for single-phase use case */
  timebase_config.Period = MULTIPHASE_PERIOD - 1 - ADC_CONVERSION_TIME;
  timebase_config.RepetitionCounter = REPETITON_RATE; /* 1 ISR every REPETITON_RATE PWM periods */
  timebase_config.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
  timebase_config.Mode = HRTIM_MODE_SINGLESHOT_RETRIGGERABLE;
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &timebase_config);
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, &timebase_config);
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_C, &timebase_config);
  HAL_HRTIM_TimeBaseConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, &timebase_config);

  /* --------------------------------------------------------------------- */
  /* TIMERA, TIMERB, TIMERC, TIMERD global configuration                   */
  /* Preload enabled, triggered by master, Fault1 enabled                  */
  /* Counter reset triggered by master: MSTPER for TIMA, MSTCMP1 for TIMB, */
  /* MSTCMP2 for TIMC, MSTCMP3 for TIMD                                    */
  /* All other parameter to default value                                  */
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
  timer_config.RepetitionUpdate = HRTIM_UPDATEONREPETITION_DISABLED;
  timer_config.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
  timer_config.InterruptRequests = HRTIM_TIM_IT_NONE;
  timer_config.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
  timer_config.FaultEnable = HRTIM_TIMFAULTENABLE_FAULT1;
  timer_config.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
  timer_config.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  timer_config.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  timer_config.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_MASTER;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_MASTER_PER;
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, &timer_config);

  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_MASTER_CMP1; 
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, &timer_config);
  
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_MASTER_CMP2; 
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_C, &timer_config);

  timer_config.DelayedProtectionMode = HRTIM_TIMER_D_E_DELAYEDPROTECTION_DISABLED;
  timer_config.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_MASTER;
  timer_config.ResetTrigger = HRTIM_TIMRESETTRIGGER_MASTER_CMP3; 
  HAL_HRTIM_WaveformTimerConfig(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, &timer_config);

  /* -------------------------------------------- */
  /* TA2, TB1, TC2, TD1, TD2 waveform description */
  /* -------------------------------------------- */
  output_config.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
  output_config.SetSource = HRTIM_OUTPUTSET_MASTERPER;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  output_config.IdleMode = HRTIM_OUTPUTIDLEMODE_IDLE;
  output_config.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
  output_config.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_INACTIVE;
  output_config.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
  output_config.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;

  /* TA2 waveform */
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_A,
                                 HRTIM_OUTPUT_TA2,
                                 &output_config);

  /* TB1 waveform */
  output_config.SetSource = HRTIM_OUTPUTSET_MASTERCMP1;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_B,
                                 HRTIM_OUTPUT_TB1,
                                 &output_config);

  /* TC2 waveform */
  output_config.SetSource = HRTIM_OUTPUTSET_MASTERCMP2;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_C,
                                 HRTIM_OUTPUT_TC2,
                                 &output_config);

  /* TD1 waveform */
  output_config.SetSource = HRTIM_OUTPUTSET_MASTERCMP3;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP1;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD1,
                                 &output_config);

  /* TD2 waveform */
  output_config.SetSource = HRTIM_OUTPUTSET_MASTERCMP4;
  output_config.ResetSource  = HRTIM_OUTPUTRESET_TIMCMP2;
  HAL_HRTIM_WaveformOutputConfig(&hhrtim,
                                 HRTIM_TIMERINDEX_TIMER_D,
                                 HRTIM_OUTPUT_TD2,
                                 &output_config);
  
  /* Set compare registers for duty cycle on TA2 */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.AutoDelayedTimeout = 0;
  compare_config.CompareValue = DUTY_CYCLE_INIT; /* Start-up duty cycle */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_A,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* Set compare registers for duty cycle on TB1 */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_B,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* Set compare registers for duty cycle on TC2 */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_C,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* Set compare registers for duty cycle on TD1 */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  /* Set compare registers for duty cycle on TD2 */
  /* TD2 reset is related to mstcmp4 (phase-shift) and not timer D counter reset */
  compare_config.CompareValue = PHASE_SHIFT_INIT + DUTY_CYCLE_INIT;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

  /* Set compare registers for phase-shifts in master timer */
  /* Each compare is coding for the phase-shift of one phase */
  compare_config.CompareValue = PHASE_SHIFT_INIT;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_MASTER,
                                  HRTIM_COMPAREUNIT_1,
                                  &compare_config);

  compare_config.CompareValue = 2 * PHASE_SHIFT_INIT;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_MASTER,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);

  compare_config.CompareValue = 3 * PHASE_SHIFT_INIT;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_MASTER,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  compare_config.CompareValue = 4 * PHASE_SHIFT_INIT;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_MASTER,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);

  /* -------------------------------------------------- */
  /* Burst mode configuration for light load management */
  /* -------------------------------------------------- */ 
  burst_config.ClockSource = HRTIM_BURSTMODECLOCKSOURCE_MASTER;
  burst_config.IdleDuration = BURST_IDLE;
  burst_config.Mode = HRTIM_BURSTMODE_CONTINOUS;
  burst_config.Period = BURST_PERIOD;
  burst_config.PreloadEnable = HRIM_BURSTMODEPRELOAD_ENABLED;
  burst_config.Prescaler = HRTIM_BURSTMODEPRESCALER_DIV1;
  burst_config.Trigger = HRTIM_BURSTMODETRIGGER_MASTER_RESET;
  HAL_HRTIM_BurstModeConfig(&hhrtim, &burst_config);

  /* -------------------------------------------------------------- */
  /* ADC trigger intialization (inc. for phase shedding management) */
  /* -------------------------------------------------------------- */
  compare_config.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
  compare_config.CompareValue = DUTY_CYCLE_INIT/2; /* Samples at 50% of Ton time */
  compare_config.AutoDelayedTimeout = 0;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_A,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_B,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_C,
                                  HRTIM_COMPAREUNIT_2,
                                  &compare_config);
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  compare_config.CompareValue = PHASE_SHIFT_INIT + (DUTY_CYCLE_INIT/2); /* Samples at 50% of Ton time on phase 5 */
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_D,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);
  
  /* Dummy triggers for 3-phase configuration, using Timer C compare 3 and 4 */
  /* Respectively 250ns and 500ns before multiphase period end*/
  compare_config.CompareValue = (MULTIPHASE_PERIOD/3) - 1  - ADC_CONVERSION_TIME;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_C,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  compare_config.CompareValue = (MULTIPHASE_PERIOD/3) - 1 - (ADC_CONVERSION_TIME*2);
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_C,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);


  /* Dummy triggers for 2-phase configuration */
  /* Configure Timer B compare 3 and 4 and timer A compare 3 for a dummy 3rd, 4th and 5th conversion */
  /* Respectively 250ns, 500ns and 750ns before multiphase period end*/
  compare_config.CompareValue = (MULTIPHASE_PERIOD/2) - 1 - ADC_CONVERSION_TIME;
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_B,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  compare_config.CompareValue = (MULTIPHASE_PERIOD/2) - 1 - (ADC_CONVERSION_TIME*2);
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_B,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);
    
  compare_config.CompareValue = MULTIPHASE_PERIOD - 1 - (ADC_CONVERSION_TIME*3);
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_A,
                                  HRTIM_COMPAREUNIT_3,
                                  &compare_config);

  /* Dummy triggers for 1-phase configuration */
  /* Configure Timer A compare 3 and 4 for dummy 2nd and 3rd conversion */
  /* Dummy 4th conversion triggered by Timer A period event */
  /* Respectively 250ns, 500ns and 750ns before multiphase period end*/
  compare_config.CompareValue = MULTIPHASE_PERIOD - 1 - (ADC_CONVERSION_TIME*2);
  HAL_HRTIM_WaveformCompareConfig(&hhrtim,
                                  HRTIM_TIMERINDEX_TIMER_A,
                                  HRTIM_COMPAREUNIT_4,
                                  &compare_config);


  /* Use 5 trigger sources, one per phase */
  adc_trigger_config.Trigger = HRTIM_ADCTRIGGEREVENT13_TIMERA_CMP2
                             + HRTIM_ADCTRIGGEREVENT13_TIMERB_CMP2
                             + HRTIM_ADCTRIGGEREVENT13_TIMERC_CMP2
                             + HRTIM_ADCTRIGGEREVENT13_TIMERD_CMP3
                             + HRTIM_ADCTRIGGEREVENT13_TIMERD_CMP4;


  adc_trigger_config.UpdateSource = HRTIM_ADCTRIGGERUPDATE_MASTER;
  HAL_HRTIM_ADCTriggerConfig(&hhrtim,
                             HRTIM_ADCTRIGGER_1,
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
#endif /* HAL version */




/**
  * @brief  Configure ADC1 for being used with HRTIM
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
    ADC_ChannelConfTypeDef sConfig;

  /* Configuration of ADCx init structure: ADC parameters and regular group */
  AdcHandle.Instance = ADC1;

  AdcHandle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode          = ENABLE;                        /* Sequencer enabled */
  AdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.LowPowerAutoWait = DISABLE;
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 sequence converted at each conversion trig, and because discontinuous mode is enabled */
  AdcHandle.Init.NbrOfConversion       = 5;                             /* Sequencer of regular group will convert the 3 first ranks: rank1, rank2, rank3 */
  AdcHandle.Init.DiscontinuousConvMode = ENABLE;                        /* Sequencer of regular group will convert the sequence in several sub-divided sequences */
  AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Sequencer of regular group will convert ranks one by one, at each conversion trig */
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONVHRTIM_TRG1;            /* Trig of conversion start done manually by software, without external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
  AdcHandle.Init.DMAContinuousRequests = ENABLE;
  AdcHandle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
 
  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  sConfig.Channel      = ADC_CHANNEL_3;
  sConfig.Rank         = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
  
  /* Configuration of channel on ADCx regular group on sequencer rank 2 */
  /* Replicate previous rank settings, change only channel and rank */
  sConfig.Channel      = ADC_CHANNEL_3;
  sConfig.Rank         = ADC_REGULAR_RANK_2;
  HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
  
  /* Configuration of channel on ADCx regular group on sequencer rank 3 */
  sConfig.Channel      = ADC_CHANNEL_3;
  sConfig.Rank         = ADC_REGULAR_RANK_3;
  HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
  
  /* Configuration of channel on ADCx regular group on sequencer rank 3 */
  sConfig.Channel      = ADC_CHANNEL_3;
  sConfig.Rank         = ADC_REGULAR_RANK_4;
  HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

  /* Configuration of channel on ADCx regular group on sequencer rank 3 */
  sConfig.Channel      = ADC_CHANNEL_3;
  sConfig.Rank         = ADC_REGULAR_RANK_5;
  HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
  
  /* Run the ADC calibration in single-ended mode */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }


  HAL_ADC_Start_DMA(&AdcHandle,
                        (uint32_t *)aADCxConvertedValues,
                        ADCCONVERTEDVALUES_BUFFER_SIZE
                       );

   HAL_ADC_Start(&AdcHandle);

  
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
