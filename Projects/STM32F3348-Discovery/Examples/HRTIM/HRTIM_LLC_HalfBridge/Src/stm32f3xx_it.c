/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_LLC_HalfBridge/Src/stm32f3xx_it.c
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

/** @addtogroup HRTIM/HRTIM_LLC_HalfBridge
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
      uint32_t CurrentPeriod;

  /* For interrupt duration monitoring purpose */
  BSP_LED_On(LED4);
  /* Clear ISR flag */
  __HAL_HRTIM_MASTER_CLEAR_IT(&hhrtim, HRTIM_MASTER_IT_MREP);

  switch(DemoState)
  {
  case RUN:
    /* Get current LLC period value */
    CurrentPeriod = __HAL_HRTIM_GETPERIOD(&hhrtim, HRTIM_TIMERINDEX_MASTER);
    /* Decrease the LLC period down to half the init value */
    if (CurrentPeriod > (LLC_PWM_PERIOD)/2)
    {
      CurrentPeriod--;
    }
    else  /* Re-start ramp from initial period */
    {
      /* Update duty cycle (CMP1) and ADC triggering point (CMP2)*/
      CurrentPeriod = LLC_PWM_PERIOD;
    }
    /* Set new LLC frequency */
    __HAL_HRTIM_SETPERIOD(&hhrtim, HRTIM_TIMERINDEX_MASTER, CurrentPeriod);
    
    /* Adjust Turn-on and turn-off time for SR1 */
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, (15 * CurrentPeriod)/100);
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_2, (35 * CurrentPeriod)/100);
    
    /* Adjust Turn-on and turn-off time for SR2 */
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_MASTER, HRTIM_COMPAREUNIT_3, (65 * CurrentPeriod)/100);
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_MASTER, HRTIM_COMPAREUNIT_4, (85 * CurrentPeriod)/100);
    
    /* Adjust new sampling points for SR1 */
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_3, (15 * CurrentPeriod)/100 - ADC_CONVERSION_TIME);
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_4, (15 * CurrentPeriod)/100 + ADC_CONVERSION_TIME);
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_MASTER, HRTIM_COMPAREUNIT_2, (35 * CurrentPeriod)/100 - ADC_CONVERSION_TIME);
    __HAL_HRTIM_SETPERIOD(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, (35 * CurrentPeriod)/100 + ADC_CONVERSION_TIME);

    /* Adjust new sampling points for SR2 */
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_C, HRTIM_COMPAREUNIT_2, (65 * CurrentPeriod)/100 - ADC_CONVERSION_TIME);
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_C, HRTIM_COMPAREUNIT_3, (65 * CurrentPeriod)/100 + ADC_CONVERSION_TIME);
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_C, HRTIM_COMPAREUNIT_4, (85 * CurrentPeriod)/100 - ADC_CONVERSION_TIME);
    __HAL_HRTIM_SETPERIOD(&hhrtim, HRTIM_TIMERINDEX_TIMER_C, (85 * CurrentPeriod)/100 + ADC_CONVERSION_TIME);
    break;

  case FAULT:
    break;

  default:
    /* Something wrong happened, shut-down all switches */
    HAL_HRTIM_WaveformOutputStop(&hhrtim,
                                     HRTIM_OUTPUT_TA1
                                   | HRTIM_OUTPUT_TA2
                                   | HRTIM_OUTPUT_TB1
                                   | HRTIM_OUTPUT_TB2);
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
