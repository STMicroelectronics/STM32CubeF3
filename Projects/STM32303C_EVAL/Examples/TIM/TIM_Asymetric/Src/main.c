/**
  ******************************************************************************
  * @file    TIM/TIM_Asymetric/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F3xx TIM HAL API to generate
  *          an asymetric signal.
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

/** @addtogroup TIM_Asymetric
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define INITIAL_PHASE         (uint32_t) 15000
#define INITIAL_LENGTH        (uint32_t) 5000
#define PWM_FREQUENCY         (uint32_t) 22000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle8;
TIM_HandleTypeDef    TimHandle1;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Master and slave configurations */
TIM_SlaveConfigTypeDef  sSlaveConfig;
TIM_MasterConfigTypeDef sMasterConfig;

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

  /*##-1- Configure the TIM peripheral #######################################*/ 
  /* --------------------------------------------------------------------------- 
     TIM8 is configured to generate an Asymetric signal with a programmable 
   Phase-Shifted signal on TIM8_CH2:
   - TIM8 Channel 1 is configured in PWM2 mode
   - TIM8 Channel 2 is configured in Asymetric PWM2 mode
   - The counter mode is center aligned mode
   - The pulse length and the phase shift are programmed consecutively in TIM8_CCR2 and TIM8_CCR1.
    
   TIM1 is configured to generating the reference signal on Channel1 used by TIM8:
    - TIM1 is generating a PWM signal with frequency equal to 1.636KHz
    - TIM1 is used as master for TIM8, the update event of TIM1 genarates the Reset counter
      of TIM8 to synchronize the two signals: the reference signal (TIM1_CH1) and 
            the shifted signal (TIM8_CH2). 
          
    In this example TIM1 and TIM8 input clock (TIM18CLK) is set to APB2 clock (PCLK2)    
    TIM1 and TIM8 signals are at frequency of  (SystemCoreClock / (PWM_FREQUENCY + 1))  
       
    TIM8 is gerating a signal with the following caracteristics:
    - Pulse lenght = (TIM8_CCR1 + TIM8_CCR2) / TIM8_CLK
    - Phase shift = TIM8_CCR1/TIM8_CLK
      with TIM8_CLK = (SystemCoreClock / (Period + 1)), as the prescaler is equal to zero.
  
    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f3xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency     
  --------------------------------------------------------------------------- */
  /* Initialize Timers: TIM1 & TIM8 */  
  TimHandle1.Instance = TIM1;
  TimHandle1.Init.Prescaler         = 0;
  TimHandle1.Init.Period            = 2 * PWM_FREQUENCY;
  TimHandle1.Init.ClockDivision     = 0;
  TimHandle1.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle1.Init.RepetitionCounter = 0;
  TimHandle1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_PWM_Init(&TimHandle1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  TimHandle8.Instance = TIM8;
  TimHandle8.Init.Prescaler         = 0;
  TimHandle8.Init.Period            = PWM_FREQUENCY;
  TimHandle8.Init.ClockDivision     = 0;
  TimHandle8.Init.CounterMode       = TIM_COUNTERMODE_CENTERALIGNED1;
  TimHandle8.Init.RepetitionCounter = 0;
  TimHandle8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_PWM_Init(&TimHandle8) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the PWM channels #########################################*/ 
  /* Channels 1&2 configuration on TIM8 */
  sConfig.OCMode = TIM_OCMODE_PWM2;
  sConfig.Pulse = INITIAL_PHASE;
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfig.OCFastMode = TIM_OCFAST_DISABLE;
  sConfig.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle8, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  sConfig.OCMode = TIM_OCMODE_ASSYMETRIC_PWM2;
  sConfig.Pulse = INITIAL_LENGTH;
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle8, &sConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /* Channel1 configuration on TIM1 */
  sConfig.OCMode = TIM_OCMODE_PWM1;
  sConfig.Pulse = PWM_FREQUENCY;
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle1, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*##-3- Configure master/slave mode and trigger ############################*/ 
  /* Synchronization between TIM1 and TIM8
  The aim is to generate a reference signal on TIM1_CH1
  The Phase-Shifted signal generated on TIM8_CH2 is compared to the reference
  signal */
  /* Configure TIM8 in slave mode: an active edge on  trigger input generates a 
  reset on TIM8 */
  sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger     = TIM_TS_ITR0;
  sSlaveConfig.TriggerPolarity  = TIM_TRIGGERPOLARITY_NONINVERTED;
  sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
  sSlaveConfig.TriggerFilter    = 0;
  if(HAL_TIM_SlaveConfigSynchro(&TimHandle8, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  /* Configure TIM1 in master mode */
  sMasterConfig.MasterOutputTrigger  = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;
  if(HAL_TIMEx_MasterConfigSynchronization(&TimHandle1, &sMasterConfig) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  
  /*##-4- Start PWM signals generation #######################################*/ 
  /* Start TIM1 channel 1 */
  if(HAL_TIM_PWM_Start(&TimHandle1, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  /* Start TIM8 channel 1 */
  if(HAL_TIM_PWM_Start(&TimHandle8, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  /* Start TIM8 channel 2 */
  if(HAL_TIM_PWM_Start(&TimHandle8, TIM_CHANNEL_2) != HAL_OK)
  {
    /* PWM Generation Error */
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
