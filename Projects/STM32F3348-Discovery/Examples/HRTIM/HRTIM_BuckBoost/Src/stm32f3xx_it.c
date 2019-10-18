/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_BuckBoost/Src/stm32f3xx_it.c
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

/** @addtogroup HRTIM/HRTIM_BuckBoost
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
  * @brief  This function handles HRTIM1 TIMA interrupt request.
  * @param  None
  * @retval None
  */
void HRTIM1_TIMA_IRQHandler(void)
{
      uint32_t CurrentDuty;

  /* For interrupt duration monitoring purpose */
  BSP_LED_On(LED4);
  
  /* Clear ISR flag */
  __HAL_HRTIM_TIMER_CLEAR_IT(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_TIM_IT_REP);

  switch(DemoState)
  {
  case BUCK:
    /* Get current duty cycle value */
    CurrentDuty = __HAL_HRTIM_GETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1);
    /* Increase the compare value up to 60% duty cycle and re-start from 33% */
    if (CurrentDuty > (6 * BUCK_PWM_PERIOD)/10)
    {
      __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, BUCK_PWM_PERIOD/3);
      __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_2, BUCK_PWM_PERIOD/6);
    }
    else  /* Re-start ramp from 33% duty cycle */
    {
      /* Update duty cycle (CMP1) and ADC triggering point (CMP2)*/
      CurrentDuty++;
      __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, CurrentDuty);
      __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_2, CurrentDuty/2);
    }
    /* BOOST side PMOS turned ON permanently */
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, BUCK_PWM_PERIOD+1);
    break;

  case BOOST:
    /* BUCK side PMOS turned ON permanently */
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, BUCK_PWM_PERIOD);
    
    /* Get current boost duty cycle value */
    CurrentDuty = __HAL_HRTIM_GETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1);
    // Slowly increase boost factor to limit inrush current (Boost factor is 1/CurrentDuty)
    CurrentDuty -= 0x400;
    /* Increase boost factor up to 50%*/
    if (CurrentDuty >= (BUCK_PWM_PERIOD/2) + 1 )
    {
      __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, CurrentDuty);
    }
    break;

  case FAULT:
  case DE_ENERGIZE:
    /* BUCK side NMOS turned ON permanently */
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, BUCK_PWM_PERIOD+1);
    /* BOOST side NMOS turned ON permanently */
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, BUCK_PWM_PERIOD);
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
