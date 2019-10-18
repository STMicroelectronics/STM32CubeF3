/**
  ******************************************************************************
  * @file    PWR/PWR_CurrentConsumption/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F3xx PWR HAL API to enter
  *          and exit the sleep/stop/standby modes, to measure power consumption 
  *          in these different low power modes.  
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

/** @addtogroup PWR_CurrentConsumption
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User Button interrupt  */
__IO uint32_t uwWakeUpIntFlag  = 0;  /* set to 1 after RTC alarm interrupt    */
__IO uint32_t uwStandByOutFlag = 0;  /* set to 1 after exit from standby mode */
/* Private function prototypes -----------------------------------------------*/

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
  
  /* Configure Green, Red and Orange LEDs */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);
  BSP_LED_Init(LED_ORANGE);    
  
  /* Configure the system clock to 72 Mhz */
  SystemClock_Config();
 
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Check and handle if the system was resumed from StandBy mode */ 
  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

    /* Turn on the Orange LED */
    BSP_LED_On(LED_ORANGE);
    uwStandByOutFlag = 1;
    
    /* Configure User push-button */
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

    /* Wait that user release the User push-button */
    while(BSP_PB_GetState(BUTTON_USER) == SET){}    
  }
 
 /* infinite loop */
  while(1)
  {

    /* Configure User Button */
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
    UserButtonStatus = 0;
    
    /* Wait until User button is pressed to enter the Low Power mode.
       In the meantime, LED_GREEN is blinking */
    while(UserButtonStatus == 0)
    {
      BSP_LED_Toggle(LED_GREEN); 
      HAL_Delay(100);
      
      /* if exiting from stand-by mode, 
         keep LED_ORANGE ON for about 3 sec. */
      if (uwStandByOutFlag > 0)
      {
        uwStandByOutFlag++;
        if (uwStandByOutFlag == 30)
        {
          BSP_LED_Off(LED_ORANGE);
          uwStandByOutFlag = 0; 
        }
      }
      /* if exiting from stop mode thru RTC alarm
        interrupt, keep LED_ORANGE ON for about 3 sec. */      
      if (uwWakeUpIntFlag > 0)
      {
        uwWakeUpIntFlag++;
        if (uwWakeUpIntFlag == 30)
        {
          BSP_LED_Off(LED_BLUE);
          uwWakeUpIntFlag = 0; 
        }
      }
    }
    
    /* Loop while Key button is maintained pressed */
    while(BSP_PB_GetState(BUTTON_USER) == SET) {} 
    
    
    /* Make sure LED_GREEN is turned off to 
      reduce low power mode consumption */
    BSP_LED_Off(LED_GREEN);
    
#if defined (SLEEP_MODE)
    /* Sleep Mode Entry 
    - System Running at PLL (72 MHz)
    - Flash 2 wait state
    - Instruction and Data caches ON
    - Prefetch ON
    - Code running from Internal FLASH
    - All peripherals disabled.
    - Wakeup using EXTI Line (User Button PA.00)
    */
    SleepMode_Measure();
#elif defined (STOP_MODE)
    /* STOP Mode Entry 
    - RTC Clocked by LSI
    - Regulator in LP mode
    - HSI, HSE OFF and LSI OFF if not used as RTC Clock source  
    - No IWDG
    - Wakeup using EXTI Line (User Button PA.00)
    */
    StopMode_Measure();      
#elif defined (STOP_RTC_MODE)
    /* STOP Mode Entry 
    - RTC Clocked by LSI
    - Regulator in LP mode
    - HSI, HSE OFF and LSI OFF if not used as RTC Clock source  
    - No IWDG
    - Automatic Wakeup using RTC clocked by LSI (after ~20s)
    */
    StopRTCMode_Measure();  
#elif defined (STANDBY_MODE)
    /* STANDBY Mode Entry 
    - Backup SRAM and RTC OFF
    - IWDG and LSI OFF
    - Wakeup using WakeUp Pin (User Button PA.00)
    */
    StandbyMode_Measure();
#elif defined (STANDBY_RTC_MODE)
    /* STANDBY Mode with RTC on LSI Entry 
    - RTC Clocked by LSI
    - IWDG OFF and LSI OFF if not used as RTC Clock source
    - Automatic Wakeup using RTC clocked by LSI (after ~20s)
    */
    StandbyRTCMode_Measure();
#endif    
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

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Error if LED_RED is ON  */
  BSP_LED_Init(LED_RED);
  BSP_LED_On(LED_RED);
  while(1)
  {
  }
}

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_IncTick();
}


/**
  * @brief  Wake Up Timer callback
  * @param  hrtc : hrtc handle
  * @retval None
  */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{ 
  /* NOTE : add the specific code to handle the RTC wake up interrupt */
  
  /* Initialize Green LED */
  BSP_LED_Init(LED_GREEN);
  /* On The Green LED */
  BSP_LED_On(LED_GREEN);
  
  /* Initialize Blue LED */
  BSP_LED_Init(LED_BLUE);
  /* On The Blue LED */
  BSP_LED_On(LED_BLUE);
  uwWakeUpIntFlag = 1;
  
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == USER_BUTTON_PIN)
  {  
    UserButtonStatus = 1;
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
