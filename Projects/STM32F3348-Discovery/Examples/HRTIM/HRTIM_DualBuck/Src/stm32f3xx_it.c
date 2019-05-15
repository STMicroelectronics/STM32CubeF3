/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_DualBuck/Src/stm32f3xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_it.h"


/** @addtogroup STM32F3xx_HAL_Examples
  * @{
  */

/** @addtogroup HRTIM_DualBuck
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
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void HRTIM1_TIMD_IRQHandler(void)
{
      uint32_t CurrentDuty;

  /* For interrupt duration monitoring purpose */
  BSP_LED_On(LED4);
  
  __HAL_HRTIM_TIMER_CLEAR_IT(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_TIM_IT_REP);

  /* Get current duty cycle value */
  CurrentDuty = __HAL_HRTIM_GETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_1);

  /* Increase the compare value up to 50% duty cycle and re-start from 5% */
  if (CurrentDuty < BUCK_PWM_PERIOD/2)
  {
    CurrentDuty++;
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_1, CurrentDuty);// Duty cycle update
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_3, CurrentDuty/2); // ADC trigger position update
  }
  else  // Re-start ramp from 5% duty cycle
  {
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_1, BUCK_PWM_PERIOD/20);
    __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_3, BUCK_PWM_PERIOD/40);
  }

//  HAL_HRTIM_IRQHandler(&hhrtim, HRTIM_TIMERINDEX_TIMER_D);
  BSP_LED_Off(LED4);
}

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
