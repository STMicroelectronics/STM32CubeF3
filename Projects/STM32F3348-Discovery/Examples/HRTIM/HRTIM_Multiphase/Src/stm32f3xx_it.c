/**
  ******************************************************************************
  * @file    HRTIM\HRTIM_Multiphase/Src/stm32f3xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f3xx_it.h"


/** @addtogroup STM32F3xx_HAL_Examples
  * @{
  */

/** @addtogroup HRTIM\HRTIM_Multiphase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F3xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) , for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f3xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  This function handles HRTIM1 Master timer interrupt request.
  * @param  None
  * @retval None
  */
void HRTIM1_Master_IRQHandler(void)
{

  /* For interrupt duration monitoring purpose */
  BSP_LED_On(LED4);
  
  /* Clear ISR flag */
  HRTIM1->sMasterRegs.MICR = HRTIM_MASTER_IT_MREP;

  switch(DemoState)
  {
  /* ----------------------------------------- */
  /* Run time operation (duty cycle regulation */
  /* Not implemented, example code only        */
  /* ----------------------------------------- */
  case FIVE_PHASE:
    /* Update here duty cycle as per regulation outputs.
    To be noticed: specific management for 5th phase (simplified if TIMER E is used)
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = NewDuty;
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP1xR = NewDuty;
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP1xR = NewDuty;
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP1xR = NewDuty;
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP2xR = PHASE_SHIFT_INIT + NewDuty; */
  break;

  case FOUR_PHASE:
    /* Update here duty cycle as per regulation outputs
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = NewDuty;
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP1xR = NewDuty;
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP1xR = NewDuty;
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP1xR = NewDuty; */
  break;

  case THREE_PHASE:
    /* Update here duty cycle as per regulation outputs.
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = NewDuty;
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP1xR = NewDuty;
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP1xR = NewDuty; */
  break;

  case TWO_PHASE:
    /* Update here duty cycle as per regulation outputs.
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = NewDuty;
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP1xR = NewDuty; */
  break;

  case ONE_PHASE:
    /* Update here duty cycle as per regulation outputs.
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = NewDuty; */
  break;

  case BURST:
    /* Update here duty cycle and burst ratio as per regulation outputs */
  break;

  /* ----------------------------------------- */
  /* Transient operation (phase shedding)      */
  /* ----------------------------------------- */
  case FIVE_PHASE_INIT:
    DemoState = FIVE_PHASE;
    HRTIM1->sMasterRegs.MCMP1R = (MULTIPHASE_PERIOD / 5);
    HRTIM1->sMasterRegs.MCMP2R = 2 * (MULTIPHASE_PERIOD / 5);
    HRTIM1->sMasterRegs.MCMP3R = 3 * (MULTIPHASE_PERIOD / 5);
    HRTIM1->sMasterRegs.MCMP4R = 4 * (MULTIPHASE_PERIOD / 5);
    
    /* Restore waveform generation for TD2 (CMP4 is used as ADC trigger in case of single-phase */
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx2R = HRTIM_OUTPUTSET_MASTERCMP4;

    /* Reconfigure triggers for 5 conversions */
      HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP4xR = PHASE_SHIFT_INIT + (DUTY_CYCLE_INIT/2); /* Samples at 50% of Ton time on phase 5 */
      HRTIM1->sCommonRegs.ADC1R = HRTIM_ADC1R_AD1TAC2 /* ADC trigger event: Timer A compare 2 */
                                + HRTIM_ADC1R_AD1TBC2 /* ADC trigger event: Timer B compare 2 */
                                + HRTIM_ADC1R_AD1TCC2 /* ADC trigger event: Timer C compare 2 */
                                + HRTIM_ADC1R_AD1TDC3 /* ADC trigger event: Timer D compare 3 */
                                + HRTIM_ADC1R_AD1TDC4; /* ADC trigger event: Timer D compare 4 */

    /* Disable burst mode */
    HRTIM1->sCommonRegs.BMCR &= ~(HRTIM_BMCR_BME);

  break;

  case FOUR_PHASE_INIT:
    DemoState = FOUR_PHASE;
    /* Disables 5th phase and re-balance others */
    HRTIM1->sMasterRegs.MCMP1R = (MULTIPHASE_PERIOD / 4);
    HRTIM1->sMasterRegs.MCMP2R = 2 * (MULTIPHASE_PERIOD / 4);
    HRTIM1->sMasterRegs.MCMP3R = 3 * (MULTIPHASE_PERIOD / 4);
    HRTIM1->sMasterRegs.MCMP4R = MULTIPHASE_PERIOD + 1;

    
    /* Reconfigure Timer D compare 4 for a dummy 5th conversion 250ns before multiphase period end */
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP4xR = (MULTIPHASE_PERIOD / 4) - 1 - ADC_CONVERSION_TIME;
  break;

  case THREE_PHASE_INIT:
    DemoState = THREE_PHASE;
    /* Disables 4th phase and re-balance others */
    HRTIM1->sMasterRegs.MCMP1R = (MULTIPHASE_PERIOD / 3);
    HRTIM1->sMasterRegs.MCMP2R = 2 * (MULTIPHASE_PERIOD / 3);
    HRTIM1->sMasterRegs.MCMP3R = MULTIPHASE_PERIOD + 1;

    /* Reconfigure ADC triggers for a dummy 4th and 5th conversion */
    /* Respectively 250ns and 500ns before multiphase period end*/
    HRTIM1->sCommonRegs.ADC1R = HRTIM_ADCTRIGGEREVENT13_TIMERA_CMP2
                             + HRTIM_ADCTRIGGEREVENT13_TIMERB_CMP2
                             + HRTIM_ADCTRIGGEREVENT13_TIMERC_CMP2
                             + HRTIM_ADCTRIGGEREVENT13_TIMERC_CMP3      /* New trigger for dummy conversion */
                             + HRTIM_ADCTRIGGEREVENT13_TIMERC_CMP4;     /* New trigger for dummy conversion */
  break;

  case TWO_PHASE_INIT:
    DemoState = TWO_PHASE;
    /* Disables 3rd phase and re-balance others */
    HRTIM1->sMasterRegs.MCMP1R = (MULTIPHASE_PERIOD / 2);
    HRTIM1->sMasterRegs.MCMP2R = MULTIPHASE_PERIOD + 1;
    
    /* Reconfigure ADC triggers for a dummy 3rd, 4th and 5th conversion */
    /* Respectively 250ns, 500ns and 750ns before multiphase period end*/
    HRTIM1->sCommonRegs.ADC1R = HRTIM_ADCTRIGGEREVENT13_TIMERA_CMP2
                             + HRTIM_ADCTRIGGEREVENT13_TIMERB_CMP2
                             + HRTIM_ADCTRIGGEREVENT13_TIMERB_CMP3      /* New trigger for dummy conversion */
                             + HRTIM_ADCTRIGGEREVENT13_TIMERB_CMP4      /* New trigger for dummy conversion */
                             + HRTIM_ADCTRIGGEREVENT13_TIMERA_CMP3;     /* New trigger for dummy conversion */
  break;

  case ONE_PHASE_INIT:
    DemoState = ONE_PHASE;
    /* Disables 2nd phase */
    HRTIM1->sMasterRegs.MCMP1R = MULTIPHASE_PERIOD + 1;

    /* Reconfigure ADC triggers for a dummy 2nd, 3rd, 4th and 5th conversion */
    /* Respectively 250ns, 500ns, 750ns and 1µs before multiphase period end*/
    HRTIM1->sMasterRegs.MCMP4R = MULTIPHASE_PERIOD - 1 - (ADC_CONVERSION_TIME*4);
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx2R = 0;
    HRTIM1->sCommonRegs.ADC1R = HRTIM_ADCTRIGGEREVENT13_TIMERA_CMP2
                             + HRTIM_ADCTRIGGEREVENT13_TIMERA_CMP3      /* New trigger for dummy conversion */
                             + HRTIM_ADCTRIGGEREVENT13_TIMERA_CMP4      /* New trigger for dummy conversion */
                             + HRTIM_ADCTRIGGEREVENT13_TIMERA_PERIOD    /* New trigger for dummy conversion */
                             + HRTIM_ADCTRIGGEREVENT13_MASTER_CMP4;     /* New trigger for dummy conversion */
  break;

  case BURST_INIT:
    DemoState = BURST;
    /* Enable Burst mode (will start on next MAster timer period */
    HRTIM1->sCommonRegs.BMCR |= HRTIM_BMCR_BME;
  break;

  /* ---------------- */
  /* Fault management */
  /* ---------------- */
  case FAULT:  
  default:
      /* Something went wrong, disable all outputs */
      HRTIM1->sCommonRegs.ODISR = HRTIM_OENR_TA2OEN | HRTIM_OENR_TB1OEN | HRTIM_OENR_TC2OEN | HRTIM_OENR_TD1OEN | HRTIM_OENR_TD2OEN;     
    break;
  }

  BSP_LED_Off(LED4);

}

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
