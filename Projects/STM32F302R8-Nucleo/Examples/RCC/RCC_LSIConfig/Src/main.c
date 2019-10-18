/**
  ******************************************************************************
  * @file    RCC/RCC_LSIConfig/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the RCC HAL API to configure the
  *          system clock (SYSCLK) and modify the clock settings on run time.
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

/** @addtogroup RCC_LSIConfig
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void EnableLSI_Config(void);
static void DisableLSI_Config(void);
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

  /* Configure LED_GREEN */
  BSP_LED_Init(LED_GREEN);

  /* Initialize User push-button, will be used to trigger an interrupt each time it's pressed.
     In the ISR the PLL source will be changed from different clocks */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Enable HSI oscillator and configure the PLL to reach the max system frequency (64 MHz)
     when using HSI oscillator as PLL clock source. */
  SystemClock_Config();

  /* Output LSI on MCO pin(PA.08) */
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_LSI, RCC_MCODIV_1);
  


  /* carry on from here */
  /* LED toggling in an infinite loop */
  while (1)
  {
    /* Toggle LED_GREEN */
     if ((RCC->CSR & RCC_CSR_LSION) !=  RCC_CSR_LSION)
    {
      /* if LSI is off, fast LED_GREEN toggle */
      BSP_LED_Toggle(LED_GREEN);
      HAL_Delay(100);
    }
    else
    {
      /* if LSI is off, LED_GREEN is emitting a double flash every half-second */  
      BSP_LED_On(LED_GREEN); 
      HAL_Delay(100);
      BSP_LED_Off(LED_GREEN); 
      HAL_Delay(100);
      BSP_LED_On(LED_GREEN); 
      HAL_Delay(100);
      BSP_LED_Off(LED_GREEN); 
      HAL_Delay(500); 
    }      

  }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == USER_BUTTON_PIN)
  {
     if ((RCC->CSR & RCC_CSR_LSION) !=  RCC_CSR_LSION)
    {
      /* Enable LSI */
      EnableLSI_Config();
    }
    else
    {
      /* Disable LSI */
      DisableLSI_Config();
    }    
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            PLLMUL                         = RCC_PLL_MUL16 (16)
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* HSI Oscillator already ON after system reset, activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler();
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
    Error_Handler();
  }
}




static void EnableLSI_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStructure;
  
  RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStructure.LSIState = RCC_LSI_ON;
  RCC_OscInitStructure.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStructure) !=  HAL_OK)
  {
    Error_Handler();
  }
}


static void DisableLSI_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStructure;
  
  RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStructure.LSIState = RCC_LSI_OFF;
  RCC_OscInitStructure.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStructure) !=  HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
    /* Error if LED_GREEN is slowly blinking (1 sec. period) */
    BSP_LED_Toggle(LED_GREEN); 
    HAL_Delay(1000);       
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
