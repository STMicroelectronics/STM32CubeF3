/**
  ******************************************************************************
  * @file    TIM/TIM_ComplementarySignals/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F3xx TIM HAL API to generate
  *          3 signals in PWM with its complementaries.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F3xx_HAL_Examples
  * @{
  */

/** @addtogroup TIM_ComplementarySignals
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
#define  PERIOD_VALUE       (1800 - 1)  /* Period Value  */
#define  PULSE1_VALUE       900         /* Capture Compare 1 Value  */
#define  PULSE2_VALUE       450         /* Capture Compare 2 Value  */
#define  PULSE3_VALUE       225         /* Capture Compare 3 Value  */

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef              sPWMConfig;
/* Timer Break Configuration Structure declaration */
TIM_BreakDeadTimeConfigTypeDef sBreakConfig;

/* Counter Prescaler value */
uint32_t uwPrescalerValue = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
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

  /* Configure the system clock to 72 MHz */
  SystemClock_Config();

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* Compute the prescaler value to have TIM1 counter clock equal to 18MHz */
  uwPrescalerValue = (uint32_t) ((SystemCoreClock  / 18000000) - 1);
  
  /*##-1- Configure the TIM peripheral #######################################*/ 
  /* --------------------------------------------------------------------------- 
  1/ Generate 3 complementary PWM signals with 3 different duty cycles:
  
    TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2), since APB2 
    prescaler is 1.   
    TIM1CLK = PCLK2  
    PCLK2 = HCLK
    => TIM1CLK = HCLK = SystemCoreClock
  
    TIM1CLK is fixed to SystemCoreClock, the TIM1 Prescaler is set to have
    TIM1 counter clock = 18MHz.

    The objective is to generate PWM signal at 10 KHz:
    - TIM1_Period = (TIM1 counter clock / 10000) - 1

    The Three Duty cycles are computed as the following description: 

    The channel 1 duty cycle is set to 50% so channel 1N is set to 50%.
    The channel 2 duty cycle is set to 25% so channel 2N is set to 75%.
    The channel 3 duty cycle is set to 12.5% so channel 3N is set to 87.5%.
    
   The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
          
  2/ Insert a dead time equal to (100/SystemCoreClock) us

  3/ Configure the break feature, active at High level, and using the automatic 
     output enable feature
       
  4/ Use the Locking parameters level1. 
  
    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f3xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency     
  --------------------------------------------------------------------------- */

  /* Initialize TIM peripheral as follows:
       + Prescaler = (SystemCoreClock/18000000) - 1
       + Period = (1800 - 1)  (to have an output frequency equal to 10 KHz)
       + ClockDivision = 0
       + Counter direction = Up
  */
  /* Select the Timer instance */
  TimHandle.Instance = TIM1;
  
  TimHandle.Init.Prescaler         = uwPrescalerValue;
  TimHandle.Init.Period            = PERIOD_VALUE;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the PWM channels #########################################*/ 
  /* Common configuration for all channels */
  sPWMConfig.OCMode       = TIM_OCMODE_PWM1;
  sPWMConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sPWMConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sPWMConfig.OCIdleState  = TIM_OCIDLESTATE_SET;
  sPWMConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;  
  sPWMConfig.OCFastMode   = TIM_OCFAST_DISABLE;  

  /* Set the pulse value for channel 1 */
  sPWMConfig.Pulse = PULSE1_VALUE;  
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Set the pulse value for channel 2 */
  sPWMConfig.Pulse = PULSE2_VALUE;
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Set the pulse value for channel 3 */
  sPWMConfig.Pulse = PULSE3_VALUE;
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sPWMConfig, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Set the Break feature & Dead time */
  sBreakConfig.BreakState       = TIM_BREAK_ENABLE;
  sBreakConfig.DeadTime         = 100;
  sBreakConfig.OffStateRunMode  = TIM_OSSR_ENABLE;
  sBreakConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakConfig.LockLevel        = TIM_LOCKLEVEL_1;  
  sBreakConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
  sBreakConfig.BreakFilter      = 0;
  sBreakConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_ENABLE;
  sBreakConfig.Break2State      = TIM_BREAK2_DISABLE;
  sBreakConfig.Break2Polarity   = TIM_BREAK2POLARITY_LOW;
  sBreakConfig.Break2Filter     = 0;
  if(HAL_TIMEx_ConfigBreakDeadTime(&TimHandle, &sBreakConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*##-3- Start PWM signals generation #######################################*/ 
  /* Start channel 1 */
  if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /* Start channel 1N */
  if(HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  
  /* Start channel 2 */
  if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /* Start channel 2N */
  if(HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  
  /* Start channel 3 */
  if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  /* Start channel 3N */
  if(HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }

  while (1)
  {
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
void SystemClock_Config(void)
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
    /* Initialization Error */
    while(1); 
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
    /* Initialization Error */
    while(1); 
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
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
