/** 
  ******************************************************************************
  * @file    Examples_LL/HRTIM/HRTIM_BuckBoost/Src/stm32f3xx_it.c
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
#include "stm32f3xx_it.h"

/** @addtogroup STM32F3xx_LL_Examples
  * @{
  */

/** @addtogroup HRTIM_BuckBoost
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
  * @brief  This function handles NMI exception.
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
}

/******************************************************************************/
/*                 STM32F3xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f3xx.s).                                               */
/******************************************************************************/

/**
* @brief  This function handles HRTIM1 TIMA interrupt request.
* @param  None
* @retval None
*/
void HRTIM1_TIMA_IRQHandler(void)
{
  uint32_t CurrentDuty;
  
  /* For interrupt duration monitoring purpose */
  LED_On(LED4);
  
  /* Clear ISR flag */
  LL_HRTIM_ClearFlag_REP(HRTIM1, LL_HRTIM_TIMER_A);
  
  switch(DemoState)
  {
  case DEMO_STATE_BUCK:
    /* Get current duty cycle value */
    CurrentDuty = LL_HRTIM_TIM_GetCompare1(HRTIM1, LL_HRTIM_TIMER_A);
    /* Increase the compare value up to 60% duty cycle and re-start from 33% */
    if (CurrentDuty > (6 * BUCK_PWM_PERIOD)/10)
    {
      LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_A, BUCK_PWM_PERIOD/3);
      LL_HRTIM_TIM_SetCompare2(HRTIM1, LL_HRTIM_TIMER_A, BUCK_PWM_PERIOD/6);
    }
    else  /* Re-start ramp from 33% duty cycle */
    {
      /* Update duty cycle (CMP1) and ADC triggering point (CMP2)*/
      CurrentDuty++;
      LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_A, CurrentDuty);
      LL_HRTIM_TIM_SetCompare2(HRTIM1, LL_HRTIM_TIMER_A, CurrentDuty/2);
    }
    /* BOOST side PMOS turned ON permanently */
    LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_B, BUCK_PWM_PERIOD+1);
    break;
    
  case DEMO_STATE_BOOST:
    /* BUCK side PMOS turned ON permanently */
    LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_A, BUCK_PWM_PERIOD);
    
    /* Get current boost duty cycle value */
    CurrentDuty = LL_HRTIM_TIM_GetCompare1(HRTIM1, LL_HRTIM_TIMER_B);
    // Slowly increase boost factor to limit inrush current (Boost factor is 1/CurrentDuty)
    CurrentDuty -= 0x400;
    /* Increase boost factor up to 50%*/
    if (CurrentDuty >= (BUCK_PWM_PERIOD/2) + 1 )
    {
      LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_B, CurrentDuty);
    }
    break;
    
  case DEMO_STATE_FAULT:
  case DEMO_STATE_DE_ENERGIZE:
    /* BUCK side NMOS turned ON permanently */
    LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_A, BUCK_PWM_PERIOD+1);
    /* BOOST side NMOS turned ON permanently */
    LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_B, BUCK_PWM_PERIOD);
    break;
    
  default:
    /* Something wrong happened, shut-down all switches */
    LL_HRTIM_DisableOutput(HRTIM1,
                           LL_HRTIM_OUTPUT_TA1 |
                           LL_HRTIM_OUTPUT_TA2 |
                           LL_HRTIM_OUTPUT_TB1 |
                           LL_HRTIM_OUTPUT_TB2);
    break;
  }
  
  LED_Off(LED4);
  
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
