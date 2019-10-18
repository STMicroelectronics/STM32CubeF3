/**
  ******************************************************************************
  * @file    Demonstrations/Src/stm32f3xx_it.c 
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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_it.h"

/** @addtogroup STM32F3348-Discovery_Demo
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t        TriangleGeneration;
extern __IO uint16_t       TriangCmp;
extern ADC_HandleTypeDef   AdcHandle;
extern HRTIM_HandleTypeDef HrtimHandle;

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
  static uint8_t counter_1ms = 0;
  static uint8_t counter_50us = 0;
  static uint8_t counter32 = 0;
  static uint8_t falling_slope = 0;

  /* Increment tick counter every ms */
  counter_1ms++;
  if(counter_1ms >= 100)
  {
    HAL_IncTick();
    counter_1ms = 0;
  }
  
  /* Triangle Generation on PB14 */
  if (TriangleGeneration == 1)
  {
    counter_50us++;
    if(counter_50us >= 5)
    {
      counter_50us = 0;       
      __HAL_HRTIM_SETCOMPARE(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_1, TriangCmp);
      
      if(falling_slope == 0)
      {                
        counter32++;
        if(counter32 >= 31)
        {   
          counter32 = 0;
          TriangCmp = TriangCmp + 32;    
          if (TriangCmp >= 0x290)
          {
            falling_slope = 1;
          }
        }   
      }
      else
      {           
        TriangCmp--;
        if (TriangCmp <= 0x100)
        {
          falling_slope = 0;
        }
      }
    }
  }
}

/******************************************************************************/
/*                 STM32F3xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) , for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f3xx.s).                                               */
/******************************************************************************/

/**
* @brief  This function handles ADC1 interrupts requests.
* @param  None
* @retval None
*/
void ADC1_2_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&AdcHandle);
}

/**
* @brief  This function handles EXTI0 Line interrupts requests.
* @param  None
* @retval None
*/
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}


/**
* @brief  This function handles HRTIM TIMA interrupts requests.
* @param  None
* @retval None
*/
void HRTIM1_TIMA_IRQHandler(void)
{
  HAL_HRTIM_IRQHandler(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A);
}

/**
* @brief  This function handles HRTIM Master Timer interrupts requests.
* @param  None
* @retval None
*/
void HRTIM1_Master_IRQHandler(void)
{
  HAL_HRTIM_IRQHandler(&HrtimHandle, HRTIM_TIMERINDEX_MASTER);
}

/**
* @}
*/ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
