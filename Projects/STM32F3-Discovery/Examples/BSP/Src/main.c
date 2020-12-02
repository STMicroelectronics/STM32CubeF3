/**
  ******************************************************************************
  * @file    BSP/Src/main.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the STM32F3-Discovery BSP Drivers
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

/** @addtogroup TM32F3xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t DemoIndex = 0;
BSP_DemoTypedef  BSP_examples[]={
  {ACCELERO_MEMS_Test, "LSM303DLHC/LSM303AGR", 0},
  {GYRO_MEMS_Test, "L3GD20/I3G4250D", 1},
};

__IO uint8_t UserPressButton = 0;

/* Counter for User button presses*/
__IO uint32_t PressCount = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

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

  /* Configure the system clock to 72 Mhz */
  SystemClock_Config();

  /* Initialize LEDs and User_Button on STM32F3-Discovery ------------------*/
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED7);
  BSP_LED_Init(LED9);
  BSP_LED_Init(LED10);
  BSP_LED_Init(LED8);
  BSP_LED_Init(LED6);

  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Toggle LEDs between each Test */
  while (!UserPressButton) Toggle_Leds();
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);

  /* 1. Start Test: Wait For User inputs -------------------------------------*/
  while (1)
  {
    UserPressButton = 0;
    BSP_examples[DemoIndex++].DemoFunc();

    /* If all Demo has been already executed, Reset DemoIndex to restart BSP example*/
    if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
    {
      DemoIndex = 0;
    }
    /* Toggle LEDs between each Test */
    UserPressButton = 0;
    while (!UserPressButton) Toggle_Leds();
    BSP_LED_Off(LED3);
    BSP_LED_Off(LED4);
    BSP_LED_Off(LED5);
    BSP_LED_Off(LED6);
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
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
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
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (USER_BUTTON_PIN == GPIO_Pin)
  {
    while (BSP_PB_GetState(BUTTON_USER) != RESET);
    UserPressButton = 1;
  }
}

/**
  * @brief Toggle Leds
  * @param  None
  * @retval None
  */
void Toggle_Leds(void)
{
    BSP_LED_Toggle(LED3);
    HAL_Delay(100);
    BSP_LED_Toggle(LED4);
    HAL_Delay(100);
    BSP_LED_Toggle(LED6);
    HAL_Delay(100);
    BSP_LED_Toggle(LED8);
    HAL_Delay(100);
    BSP_LED_Toggle(LED10);
    HAL_Delay(100);
    BSP_LED_Toggle(LED9);
    HAL_Delay(100);
    BSP_LED_Toggle(LED7);
    HAL_Delay(100);
    BSP_LED_Toggle(LED5);
    HAL_Delay(100);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
    /* Turn LED10/3 (RED) on */
    BSP_LED_On(LED10);
    BSP_LED_On(LED3);
    while(1)
 {
 }
}

#ifdef USE_FULL_ASSERT

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
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
