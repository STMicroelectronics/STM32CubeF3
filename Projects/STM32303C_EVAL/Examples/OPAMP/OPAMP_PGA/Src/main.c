/**
  ******************************************************************************
  * @file    OPAMP/OPAMP_PGA/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the DAC
  *          peripheral to generate several signals.
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

/** @addtogroup OPAMP_PGA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef    DacHandle;
OPAMP_HandleTypeDef  OpampHandle;
TIM_HandleTypeDef    htim;

__IO uint32_t UserButtonStatus = 0;  /* set to 1 after Key push-button interrupt  */

static DAC_ChannelConfTypeDef sConfig;

const uint16_t Sine12bit[32] = {511, 611, 707, 796, 847, 874, 984, 1014, 1023,
                                1014, 984, 937, 874, 796, 707, 611, 511, 411,
                                315, 227, 149, 149, 38, 9, 0, 9, 38, 86, 149, 227,
                                315, 411};

/* Private function prototypes -----------------------------------------------*/
static void DAC_Config(void);
static void TIM_Config(void);
static void OPAMP_Config(void);
static void SystemClock_Config(void);
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

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* Configure the system clock to 72 MHz */
  SystemClock_Config();

  /* Initialize the Key push-button.
     It is used for change the gain */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Configure the TIM to control the DAC */
  TIM_Config();

  /* Configure the DAC to generator sine wave */
  DAC_Config();

  /* Configure the OPAMP1 in PGA mode : gain = 2 */
  OPAMP_Config();

  while (1)
  {
    /* Test if user an action occurred on the Key push-button */
    if (UserButtonStatus == 1)
    {
      UserButtonStatus = 0;
      /* Change the gain */
      if (OpampHandle.Init.PgaGain == OPAMP_PGA_GAIN_2)
      {
        OpampHandle.Init.PgaGain = OPAMP_PGA_GAIN_4;
      }
      else
      {
        OpampHandle.Init.PgaGain = OPAMP_PGA_GAIN_2;
      }
      /* Update OMAP config */
      HAL_OPAMP_Init(&OpampHandle);
    }
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
static void SystemClock_Config(void)
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
static void Error_Handler(void)
{
    /* Turn LED3 on */
    BSP_LED_On(LED3);
  while (1)
  {
  }
}

static void DAC_Config(void)
{
  /* Configure the DAC peripheral instance */
  DacHandle.Instance = DAC1;

  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initiliazation Error */
    Error_Handler();
  }

  /*##-1- DAC channel Configuration ###########################################*/
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }

  /*##-2- Enable DAC Channel and associated DMA ##############################*/
  if(HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_2, (uint32_t*)Sine12bit, 
                       sizeof (Sine12bit) / sizeof (uint32_t), 
                       DAC_ALIGN_12B_R) != HAL_OK)
  {
    /* Start DMA Error */
    Error_Handler();
  }
}

/**
  * @brief  TIM Configuration
  * @note   TIM configuration is based on APB1 frequency
  * @note   TIM Update event occurs each TIMxCLK/65535   
  * @param  None
  * @retval None
  */
void TIM_Config(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;
  
  /*##-1- Configure the TIM peripheral #######################################*/

  /* Time base configuration */
  htim.Instance = TIM2;
  
  htim.Init.Period = 0xFFFF;          
  htim.Init.Prescaler = 0;       
  htim.Init.ClockDivision = 0;    
  htim.Init.CounterMode = TIM_COUNTERMODE_UP; 
  htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&htim);

  /* TIM2 TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

  /*##-2- Enable TIM peripheral counter ######################################*/
  HAL_TIM_Base_Start(&htim);
  }

/**             
  * @brief  OPAMP Configuration
  * @note   
  * @note   
  * @param  None
  * @retval None
  */
void OPAMP_Config(void)
  {
  /* Set OPAMP instance */
  OpampHandle.Instance = OPAMP1; 

  /*##-1- Configure OPAMP    ##################################################*/
  /* Select PGAMode */
  OpampHandle.Init.Mode = OPAMP_PGA_MODE;  

  /* Select VPx as input for OPAMP */
  OpampHandle.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO3;

  /* Timer controlled Mux mode is not used */
  OpampHandle.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;

  /* Inverting pin in PGA mode is not connected  */
  OpampHandle.Init.PgaConnect = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
  
  /* Select the factory trimming */
  OpampHandle.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;
  
  /* Configure the gain */
  OpampHandle.Init.PgaGain = OPAMP_PGA_GAIN_2;

  /* Init */
  HAL_OPAMP_Init(&OpampHandle);

  /*##-2- Start OPAMP    ######################################################*/
  /* Enable OPAMP */
  HAL_OPAMP_Start(&OpampHandle);
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == KEY_BUTTON_PIN)
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
