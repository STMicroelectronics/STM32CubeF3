/**
  ******************************************************************************
  * @file    COMP/COMP_OutputBlanking/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the COMP 
  *          peripheral in mode output blanking feature.
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

/** @addtogroup COMP_OutputBlanking
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef   CompxHandle;
TIM_HandleTypeDef    PWM_Handle;

__IO uint32_t State = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void COMPx_Config(void);
static void TIM1_Config(void);
static void Error_Handler(void);

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

  /* Configure the system clock to 72 MHz */
  SystemClock_Config();

  /******* Initialize LEDs available on STM32303E-EVAL RevC board ******************/
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* configure COMPx : output redirected to TIM1 BKIN and blanking source is TIM1 OC5 */
  COMPx_Config();

  /* TIM1 Configuration in PWM mode */
  TIM1_Config();
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configure COMPx
  * @param  None
  * @retval None
  */
static void COMPx_Config(void)
{
 
  /*##-1- Configure the COMPx peripheral ###################################*/
  /* COMPx init: COMPx Output is connected to GPIO only (not redirected     */
  /* to timers) and output blanking source is connected to                  */
  /* TIM1 OC5.                                              */
  CompxHandle.Instance = COMPx;

  CompxHandle.Init.InvertingInput    = COMP_INVERTINGINPUT_VREFINT;
  CompxHandle.Init.NonInvertingInput = COMP_NONINVERTINGINPUT_IO1;
  CompxHandle.Init.Output            = COMP_OUTPUT_NONE;
  CompxHandle.Init.OutputPol         = COMP_OUTPUTPOL_NONINVERTED;
  CompxHandle.Init.BlankingSrce      = COMP_BLANKINGSRCE_TIM1OC5;
  CompxHandle.Init.WindowMode        = COMP_WINDOWMODE_DISABLE;
  CompxHandle.Init.TriggerMode       = COMP_TRIGGERMODE_NONE;
  HAL_COMP_Init(&CompxHandle);

  /*##-2- Start the comparator process #####################################*/ 
  /* Enable COMPx: the higher threshold is set to VREFINT ~ 1.22 V */
  HAL_COMP_Start(&CompxHandle);
}

/**             
  * @brief  TIM1 Configuration 2 and channel 5 in PWM mode
  * @note   TIM1 configuration is based on APB1 frequency
  * @note   TIM1 Update event occurs each SystemCoreClock/FREQ   
  * @param  None
  * @retval None
  */
void TIM1_Config(void)
{
  TIM_OC_InitTypeDef    TIMPWM_Config;
  
  /*##-1- Configure the TIM peripheral #######################################*/
  /* Configure TIM1 */
  /* PWM configuration */
  PWM_Handle.Instance = TIM1;
  
  /* Time Base configuration: Channel 2 and channel 5 frequency is 
     APB2 clock / period = 72000000 / 50000 = 1440 Hz */
  PWM_Handle.Init.Period = 50000;          
  PWM_Handle.Init.Prescaler = 0;       
  PWM_Handle.Init.ClockDivision = 0;    
  PWM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP; 
  PWM_Handle.Init.RepetitionCounter = 0;
  PWM_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_PWM_Init(&PWM_Handle);
  
  /*##-2- Configure the PWM Output Capture ########################################*/  
  /* PWM Output Capture configuration of TIM1 channel 2 */
  /* Duty cycle is pulse/period = 100 * (37500 / 50000) =  75% */
  TIMPWM_Config.OCMode  = TIM_OCMODE_PWM1;
  TIMPWM_Config.OCIdleState = TIM_OCIDLESTATE_RESET;
  TIMPWM_Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  TIMPWM_Config.Pulse = 37500;
  TIMPWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
  TIMPWM_Config.OCNPolarity = TIM_OCNPOLARITY_LOW;
  TIMPWM_Config.OCFastMode = TIM_OCFAST_DISABLE;
  if(HAL_TIM_PWM_ConfigChannel(&PWM_Handle, &TIMPWM_Config, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- Configure the PWM Output Capture ########################################*/  
  /* PWM Output Capture configuration of TIM1 channel 5 */
  /* Channel 5 is an internal channel (not available on GPIO): */
  /* TIM1 OC5 is high during 2000 / 72000000 = 27.7 micro second */
  TIMPWM_Config.OCMode  = TIM_OCMODE_PWM1;
  TIMPWM_Config.OCIdleState = TIM_OCIDLESTATE_RESET;
  TIMPWM_Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  TIMPWM_Config.Pulse = 2000;
  TIMPWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
  TIMPWM_Config.OCNPolarity = TIM_OCNPOLARITY_LOW;
  TIMPWM_Config.OCFastMode = TIM_OCFAST_DISABLE;
  if(HAL_TIM_PWM_ConfigChannel(&PWM_Handle, &TIMPWM_Config, TIM_CHANNEL_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- Enable TIM peripheral counter ######################################*/
  /* Start the TIM1 Channel 2 PWM */
  if(HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Start the TIM1 Channel 5 PWM */
  if(HAL_TIM_PWM_Start(&PWM_Handle, TIM_CHANNEL_5) != HAL_OK)
  {
    Error_Handler();
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
  *            HSE PREDIV                     = RCC_PREDIV_DIV1 (1)
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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
