/**
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the COMP 
  *          peripheral to make an analog watchdog.
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

/** @addtogroup COMP_AnalogWatchdog
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef    Comp1Handle;
COMP_HandleTypeDef    Comp2Handle;
__IO uint32_t State = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void COMP_Config(void);
static void StopSequence_Config(void);

/* Private functions ---------------------------------------------------------*/

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

  /* Configure the system clock to have a system clock = 72 Mhz */
  SystemClock_Config();

  /******* Initialize LEDs available on STM32303C-EVAL board ******************/
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  
  /* configure COMP1 and COMP2 with interrupts enabled */
  COMP_Config();

  /* Check input voltage level: within the thresholds, above the upper threshold
     or under the lower threshold */
  InputVoltageLevel_Check();
  
  /* Infinite loop */
  while (1)
  {
    if (State == STATE_OVER_THRESHOLD)
    {
      /* Restore config: clock, GPIO... */
      SystemClock_Config();
  
      /* Restore GPIO configuration */
      BSP_LED_Init(LED1);
      BSP_LED_Init(LED2);
      BSP_LED_Init(LED3);
      BSP_LED_Init(LED4);

      /* Turn on LD1 and LD3 and turn off LD2 and LD4 */
      BSP_LED_On(LED1);
      BSP_LED_Off(LED2);
      BSP_LED_On(LED3);
      BSP_LED_Off(LED4);
      
      while(State == STATE_OVER_THRESHOLD)
      {
        /* add your code here */
      }
    }
    else if (State == STATE_WITHIN_THRESHOLD)
    {
      /* Input voltage is within the thresholds: higher and lower thresholds */
      /* Enter STOP mode with regulator in low power */
      StopSequence_Config();
    }
    else /* (State == STATE_UNDER_THRESHOLD) */
    {
      /* Restore config: clock, GPIO... */
      SystemClock_Config();

      /* Restore GPIO configuration */
      BSP_LED_Init(LED1);
      BSP_LED_Init(LED2);
      BSP_LED_Init(LED3);
      BSP_LED_Init(LED4);

      /* Turn on LD2 & LD4 and turn off LD1 & LD3 */
      BSP_LED_Off(LED1);
      BSP_LED_On(LED2);
      BSP_LED_Off(LED3);
      BSP_LED_On(LED4);

      while(State == STATE_UNDER_THRESHOLD)
      {
         /* add your code here */
      }
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *=============================================================================
  *=============================================================================
  *                         Supported STM32F30x device                          
  *-----------------------------------------------------------------------------
  *        System Clock source                    | PLL (HSE)
  *-----------------------------------------------------------------------------
  *        SYSCLK(Hz)                             | 72000000
  *-----------------------------------------------------------------------------
  *        HCLK(Hz)                               | 72000000
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                          | 1
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler                         | 2
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler                         | 2
  *-----------------------------------------------------------------------------
  *        HSE Frequency(Hz)                      | 8000000
  *----------------------------------------------------------------------------
  *        PLLMUL                                 | 9
  *-----------------------------------------------------------------------------
  *        PREDIV                                 | 1
  *-----------------------------------------------------------------------------
  *        USB Clock                              | DISABLE
  *-----------------------------------------------------------------------------
  *        Flash Latency(WS)                      | 2
  *-----------------------------------------------------------------------------
  *        Prefetch Buffer                        | OFF
  *-----------------------------------------------------------------------------
  *=============================================================================
  ******************************************************************************
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
#ifdef USE_FULL_ASSERT
  uint32_t ret = HAL_OK;
#endif /* USE_FULL_ASSERT */

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;

#ifdef USE_FULL_ASSERT
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    assert_failed((uint8_t *)__FILE__, __LINE__);
  }
#else
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
#endif /* USE_FULL_ASSERT */

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

#ifdef USE_FULL_ASSERT
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
  if(ret != HAL_OK)
  {
    assert_failed((uint8_t *)__FILE__, __LINE__);
  }
#else
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
#endif /* USE_FULL_ASSERT */
}

/**
  * @brief  Configure COMP1 and COMP2 with interrupt
  * @param  None
  * @retval None
  */
static void COMP_Config(void)
{
 
  /*##-1- Configure the COMPx peripheral ###################################*/
  /* COMP1 Init: the higher threshold is set to VREFINT ~ 1.22V
     but can be changed to other available possibilities */
  Comp1Handle.Instance = COMP1;

  Comp1Handle.Init.InvertingInput = COMP_INVERTINGINPUT_VREFINT;
  Comp1Handle.Init.NonInvertingInput = COMP_NONINVERTINGINPUT_IO1;
  Comp1Handle.Init.Output = COMP_OUTPUT_NONE;
  Comp1Handle.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
  Comp1Handle.Init.Hysteresis = COMP_HYSTERESIS_HIGH;
  Comp1Handle.Init.Mode = COMP_MODE_LOWPOWER;
  Comp1Handle.Init.BlankingSrce = COMP_BLANKINGSRCE_NONE;
  Comp1Handle.Init.WindowMode = COMP_WINDOWMODE_DISABLE;
  Comp1Handle.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING_FALLING;
  HAL_COMP_Init(&Comp1Handle);

  /* COMP2 Init: the lower threshold is set to VREFINT/4 ~ 1.22 / 4 ~ 0.305 V
     but can be changed to other available possibilities */
  Comp2Handle.Instance = COMP2;

  Comp2Handle.Init.InvertingInput = COMP_INVERTINGINPUT_1_4VREFINT;
  Comp2Handle.Init.NonInvertingInput = COMP_NONINVERTINGINPUT_IO1;
  Comp2Handle.Init.Output = COMP_OUTPUT_NONE;
  Comp2Handle.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
  Comp2Handle.Init.Hysteresis = COMP_HYSTERESIS_HIGH;
  Comp2Handle.Init.Mode = COMP_MODE_LOWPOWER;
  Comp2Handle.Init.BlankingSrce = COMP_BLANKINGSRCE_NONE;
  Comp2Handle.Init.WindowMode = COMP_WINDOWMODE_ENABLE;
  Comp2Handle.Init.TriggerMode = COMP_TRIGGERMODE_IT_RISING_FALLING;
  HAL_COMP_Init(&Comp2Handle);

  /*##-2- Start the comparator process #####################################*/ 
  /* Enable COMP1: the higher threshold is set to VREFINT ~ 1.22 V */
  HAL_COMP_Start_IT(&Comp1Handle);

  /* Enable COMP2: the lower threshold is set to VREFINT/4 ~ 0.305 V */
  HAL_COMP_Start_IT(&Comp2Handle);
}

/**
  * @brief  Prepare the system to enter STOP mode.
  * @param  None
  * @retval None
  */
static void StopSequence_Config(void)
{
  GPIO_InitTypeDef      GPIO_InitStruct;
  
  /* PWR Peripheral clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /* Configure all GPIO port pins in Analog mode */
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  
  /* Request to enter STOP mode with regulator in low power */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}

/**
  * @brief  check input voltage level: within the thresholds, above the upper 
  *         threshold or under the lower threshold
  * @param  None
  * @retval None
  */
void InputVoltageLevel_Check(void)
{
  /*  Check if COMP1 and COMP2 output level is high */
  if (((HAL_COMP_GetOutputLevel(&Comp1Handle)) == COMP_OUTPUTLEVEL_HIGH) 
   && ((HAL_COMP_GetOutputLevel(&Comp2Handle)) == COMP_OUTPUTLEVEL_HIGH))
  {
    /* A rising edge is detected so the input voltage is higher than VREFINT */
    State = STATE_OVER_THRESHOLD;
  }
  else if (((HAL_COMP_GetOutputLevel(&Comp1Handle)) == COMP_OUTPUTLEVEL_LOW)
       && ((HAL_COMP_GetOutputLevel(&Comp2Handle)) == COMP_OUTPUTLEVEL_HIGH))
  {
    /* A falling edge is detected so the input voltage is lower than VREFINT */
    State = STATE_WITHIN_THRESHOLD;
  }
  else if (((HAL_COMP_GetOutputLevel(&Comp1Handle)) == COMP_OUTPUTLEVEL_LOW)
       && ((HAL_COMP_GetOutputLevel(&Comp2Handle)) == COMP_OUTPUTLEVEL_LOW))
  {
    State = STATE_UNDER_THRESHOLD;
  }
}

/**
  * @brief  Comparator callback.
  * @param  hcomp: COMP handle
  * @retval None
  */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{
  /* Check input voltage level: within the thresholds, */
  /* above the upper threshold or under the lower threshold */
  InputVoltageLevel_Check();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
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
