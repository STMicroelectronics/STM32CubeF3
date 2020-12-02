/**
  ******************************************************************************
  * @file    Demonstrations/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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

/** @addtogroup STM32F3-Discovery_Demo STM32F3-Discovery_Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t UserButtonPressed = 0x00;
__IO uint32_t PressCount = 0;
USBD_HandleTypeDef USBD_Device;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Demo(void);
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

  /* Initialize LEDs and User_Button on STM32F3-Discovery ------------------*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED7);
  BSP_LED_Init(LED9);
  BSP_LED_Init(LED10);
  BSP_LED_Init(LED8);
  BSP_LED_Init(LED6);

  /* Configure the system clock to 72 Mhz */
  SystemClock_Config();

    /* Init USB Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);

  /* Register the USB HID class */
  USBD_RegisterClass(&USBD_Device, &USBD_HID);

  /* Start Device Process */
  USBD_Start(&USBD_Device);

  /* Delay 1s to select USB Test Program or to go directly througt the demo*/
  HAL_Delay(1000);

  if(BSP_PB_GetState(BUTTON_USER) == KEY_PRESSED)
  {
    /* Wait for User button is released */
    while(BSP_PB_GetState(BUTTON_USER) != KEY_NOT_PRESSED)
    {
    }
    /* Wait for User button to be pressed to switch to USB Test
    the cursor move in square path and led On corresponding to such direction */
    USB_Test();
  }

  /* Gyroscope Mems Init -----------------------------------------------------*/
  if(BSP_GYRO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Accelerometer Mems Init -------------------------------------------------*/
  if(BSP_ACCELERO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Infinite loop */
  while (1)
  {
    Demo();
  }
}

/**
* @brief  Demo.
* @param  None
* @retval None
*/
static void Demo(void)
{
  /* 0. Wait for User button to be pressed -------------------------------------*/
  while (BSP_PB_GetState(BUTTON_USER) != KEY_PRESSED)
  {
    /* Toggle LED4 */
    BSP_LED_Toggle(LED4);
    HAL_Delay(20);
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);
    HAL_Delay(20);
    /* Toggle LED5 */
    BSP_LED_Toggle(LED5);
    HAL_Delay(20);
    /* Toggle LED7 */
    BSP_LED_Toggle(LED7);
    HAL_Delay(20);
    /* Toggle LED9 */
    BSP_LED_Toggle(LED9);
    HAL_Delay(20);
    /* Toggle LED10 */
    BSP_LED_Toggle(LED10);
    HAL_Delay(20);
    /* Toggle LED8 */
    BSP_LED_Toggle(LED8);
    HAL_Delay(20);
    /* Toggle LED6 */
    BSP_LED_Toggle(LED6);
    HAL_Delay(20);
  }
  /* Wait for User button is released */
  while (BSP_PB_GetState(BUTTON_USER) != KEY_NOT_PRESSED)
  {}

  /* 1. Gyroscope Mems Test -----------------------------------------------*/

  /* Wait for User button to be pressed */
  while (BSP_PB_GetState(BUTTON_USER) != KEY_PRESSED)
  {
    /* Move discovery kit to detect negative and positive acceleration values
    on X, Y axis for GYROSCOPE MEMS*/
    GYRO_MEMS_Test();
  }

  /* Wait for User button is released */
  while (BSP_PB_GetState(BUTTON_USER) != KEY_NOT_PRESSED)
  {}

  /* 2. Wait for User button to be pressed -------------------------------------*/
  while (BSP_PB_GetState(BUTTON_USER) != KEY_PRESSED)
  {
    /* Toggle LED4 */
    BSP_LED_Toggle(LED4);
    HAL_Delay(20);
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);
    HAL_Delay(20);
    /* Toggle LED5 */
    BSP_LED_Toggle(LED5);
    HAL_Delay(20);
    /* Toggle LED7 */
    BSP_LED_Toggle(LED7);
    HAL_Delay(20);
    /* Toggle LED9 */
    BSP_LED_Toggle(LED9);
    HAL_Delay(20);
    /* Toggle LED10 */
    BSP_LED_Toggle(LED10);
    HAL_Delay(20);
    /* Toggle LED8 */
    BSP_LED_Toggle(LED8);
    HAL_Delay(20);
    /* Toggle LED6 */
    BSP_LED_Toggle(LED6);
    HAL_Delay(20);
  }
  /* Wait for User button is released */
  while (BSP_PB_GetState(BUTTON_USER) != KEY_NOT_PRESSED)
  {}

  /* 3. Accelerometer Mems Test --------------------------------------------*/

  /* Wait for User button to be pressed */
  while (BSP_PB_GetState(BUTTON_USER) != KEY_PRESSED)
  {
    /* Move discovery kit to detect negative and positive acceleration values
    on X, Y and Z axis for ACCELERATOR MEMS*/
    ACCELERO_MEMS_Test();
  }
  /* Wait for User button is released */
  while (BSP_PB_GetState(BUTTON_USER) != KEY_NOT_PRESSED)
  {}

  /* 4. Wait for User button to be pressed -------------------------------------*/
  while (BSP_PB_GetState(BUTTON_USER) != KEY_PRESSED)
  {
    /* Toggle LED4 */
    BSP_LED_Toggle(LED4);
    HAL_Delay(20);
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);
    HAL_Delay(20);
    /* Toggle LED5 */
    BSP_LED_Toggle(LED5);
    HAL_Delay(20);
    /* Toggle LED7 */
    BSP_LED_Toggle(LED7);
    HAL_Delay(20);
    /* Toggle LED9 */
    BSP_LED_Toggle(LED9);
    HAL_Delay(20);
    /* Toggle LED10 */
    BSP_LED_Toggle(LED10);
    HAL_Delay(20);
    /* Toggle LED8 */
    BSP_LED_Toggle(LED8);
    HAL_Delay(20);
    /* Toggle LED6 */
    BSP_LED_Toggle(LED6);
    HAL_Delay(20);
  }

  /* Force The Leds to be Off before entring to the USB demo  */
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED7);
  BSP_LED_Off(LED9);
  BSP_LED_Off(LED10);
  BSP_LED_Off(LED8);
  BSP_LED_Off(LED6);

  /* 5. Wait for User button is released */
  while (BSP_PB_GetState(BUTTON_USER) != KEY_NOT_PRESSED)
  {}

  /* 6. USB Test -------------------------------------------------------------*/

  /* Wait for User button to be pressed */
  while (BSP_PB_GetState(BUTTON_USER) != KEY_PRESSED)
  {
    /* Wait for User button to be pressed to switch to USB demo
    Mouse cursor moving corresponding to board move direction  */
    USB_Demo();
  }

  /* Wait for User button is released */
  while (BSP_PB_GetState(BUTTON_USER) != KEY_NOT_PRESSED)
  {}

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
  RCC_PeriphCLKInitTypeDef  RCC_PeriphClkInit;

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

  /* Configures the USB clock */
  HAL_RCCEx_GetPeriphCLKConfig(&RCC_PeriphClkInit);
  RCC_PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);


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
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==USER_BUTTON_PIN)
  {
   UserButtonPressed++;
   if (UserButtonPressed > 0x2)
    {
      UserButtonPressed = 0x0;
    }
  }
}

/**
  * @brief  This function handles the test program fail.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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
  {}
}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
