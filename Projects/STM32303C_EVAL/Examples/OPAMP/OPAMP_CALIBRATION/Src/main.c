/**
  ******************************************************************************
  * @file    OPAMP/OPAMP_CALIBRATION/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to calibrate
  *          the OPAMP peripheral and generate several signals.
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

/** @addtogroup OPAMP_CALIBRATION
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

uint32_t factorytrimmingvaluep = 0, factorytrimmingvaluen = 0;
uint32_t oldtrimmingvaluep = 0, oldtrimmingvaluen = 0; 
uint32_t newtrimmingvaluep = 0, newtrimmingvaluen = 0; 

uint32_t ledblinkingduration = 0; 

static DAC_ChannelConfTypeDef sConfig;

const uint16_t Sine12bit[32] = {510, 610, 700, 790, 840, 870, 980, 1010, 1020,
                                1010, 980, 930, 870, 790, 700, 610, 510, 410,
                                310, 220, 140, 140, 40, 0, 00, 10, 40, 90, 150, 230,
                                320, 41};

/* Private function prototypes -----------------------------------------------*/
static void DAC_Config(void);
static void TIM_Config(void);
static void OPAMP_Calibrate_Before_Run (void);
static void OPAMP_Calibrate_After_Run(void);
static void OPAMP_Config(void);
void SystemClock_Config(void);
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

  /* Configure LED1 & LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* Configure Key push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Configure the TIM to control the DAC */
  TIM_Config();

  /* Configure the DAC to generator sine wave */
  DAC_Config();

  /*##-1- Configure OPAMP    #################################################*/
  /* Set OPAMP instance */
  OpampHandle.Instance = OPAMP1; 
  

  /* Configure the OPAMPx */
  OPAMP_Config();

  /*##-2- Calibrate OPAMP    #################################################*/
  
  /* Press user button to launch calibration */ 
  /*  printf("\n\n\r Press user button to launch calibration \n\r"); */
  
  while(UserButtonStatus != 1);

  HAL_Delay(100); 
  UserButtonStatus = 0;
  
  OPAMP_Calibrate_Before_Run();
 
  /*  printf("\n\r The LED blinks rapidly if ");
  printf("\n\r Calibrated trimming are different from Factory Trimming \n\r");
  
  printf("\n\r The LED blinks slowly if ");
  printf("\n\r Calibrated trimming same as Factory Trimming \n\r"); */
  
  /*##-3- Start OPAMP    #####################################################*/

  /* Press user button to launch OPAMP */ 
  /*  printf("\n\n\r Press user button to launch OPAMP, gain = 2\n\r"); */
  while(UserButtonStatus != 1)
  {
    BSP_LED_Toggle (LED1);
    HAL_Delay(ledblinkingduration); 
  }
  BSP_LED_Off(LED1);
  
  UserButtonStatus = 0;

  /* Enable OPAMP */
  HAL_OPAMP_Start(&OpampHandle);
  
  /*##-3- Modify OPAMP setting while OPAMP ON ################################*/
  
  /* Press user button to change OPAMP setting on the fly (dummy change,      */
  /* without application purpose in this example: change non-inverting input  */
  /* source).                                                                 */ 
  /* printf("\n\n\r Press user button to modify OPAMP non-inverting input source \n\r"); */
  /* printf("       i.e. non-inverting input source is changed on the fly \n\r"); */

  while(UserButtonStatus != 1);
  HAL_Delay(500);
  UserButtonStatus = 0;
  /* Change the OPAMP input selection */
  OpampHandle.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO0;
  /* Update OMAP config */
  HAL_OPAMP_Init(&OpampHandle); 

  
  /*##-4- Stop OPAMP #########################################################*/
  
  /* Press user button to stop OPAMP */ 
  /* printf("\n\n\r Press user button to stop OPAMP \n\r"); */
  
  while(UserButtonStatus != 1);
  HAL_Delay(500);
  UserButtonStatus = 0;

  /* Disable OPAMP */
  HAL_OPAMP_Stop(&OpampHandle);
  
  /*##-5- Calibrate OPAMP    #################################################*/
  /* Press user button to launch calibration */ 
  /* printf("\n\n\r Press user button to launch calibration \n\r"); */
  
  while(UserButtonStatus != 1);
  HAL_Delay(500);
  UserButtonStatus = 0;

  OPAMP_Calibrate_After_Run();
   
  /* printf("\n\r The LED blinks rapidly if: ");
  printf("\n\r New calibrated trimming are different from ones calibrated before run \n\r");
  
  printf("\n\r The LED blinks slowly if: ");
  printf("\n\r New calibrated trimming are same as ones calibrated before run\n\r"); */
         
   while(UserButtonStatus != 1)
  {
    BSP_LED_Toggle (LED1);
    HAL_Delay(ledblinkingduration); 
  }
  BSP_LED_Off(LED1);
  
  /*##-4- End of tests       #################################################*/
  
  /* printf("\n\n\r End of example \n\r"); */
  while (1);
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

static void DAC_Config(void)
{
  /* Configure the DAC peripheral instance */
  DacHandle.Instance = DACx;

  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-1- DAC channel Configuration ###########################################*/
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNELa) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }

  /*##-2- Enable DAC Channel and associeted DMA ##############################*/
  if(HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNELa, (uint32_t*)Sine12bit, 
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
  * @brief  OPAMP Calibration before OPAMP runs
  * @note   
  * @note   
  * @param  None
  * @retval None
  */
void OPAMP_Calibrate_Before_Run(void)
{

  /* Retrieve Factory Trimming */
   factorytrimmingvaluep = HAL_OPAMP_GetTrimOffset(&OpampHandle, OPAMP_FACTORYTRIMMING_P);
   factorytrimmingvaluen = HAL_OPAMP_GetTrimOffset(&OpampHandle, OPAMP_FACTORYTRIMMING_N);
    
   /* Run OPAMP calibration */
   HAL_OPAMP_SelfCalibrate(&OpampHandle);
   
   /* Store trimming value */
   oldtrimmingvaluep = OpampHandle.Init.TrimmingValueP;
   oldtrimmingvaluen = OpampHandle.Init.TrimmingValueN;

   /* Are just calibrated trimming same or different from Factory Trimming */
   if  ((oldtrimmingvaluep != factorytrimmingvaluep) || (oldtrimmingvaluen != factorytrimmingvaluen))
   {
     /* Calibrated trimming are different from Factory Trimming */
     /* printf("....... Calibrated trimming are different from Factory Trimming \n\r"); */
     /* LED blinks quickly */
     ledblinkingduration = 250;       
   }
   
   else
   {
     /* Calibrated trimming same as Factory Trimming */  
     /* printf("....... Calibrated trimming same as Factory Trimming \n\r"); */
    /* LED blinks slowly */
     ledblinkingduration = 1500;
   }
  
   /* With use temperature sensor   */
   /* Calibration */
  }

/**             
  * @brief  OPAMP Calibration after OPAMP runs
  * @note   
  * @note   
  * @param  None
  * @retval None
  */
void OPAMP_Calibrate_After_Run(void)
  {
   /* Run OPAMP calibration */
   HAL_OPAMP_SelfCalibrate(&OpampHandle);
   
   /* Store trimming value */
   newtrimmingvaluep = OpampHandle.Init.TrimmingValueP;
   newtrimmingvaluen = OpampHandle.Init.TrimmingValueN;

   /* Are just calibrated trimming same or different from Factory Trimming */
   if  ((oldtrimmingvaluep != newtrimmingvaluep) || (oldtrimmingvaluen != newtrimmingvaluen))
   {
     /* New calibrated trimming are different from ones calibrated before run */
      /* printf("....... New calibrated trimming are different from ones calibrated before run \n\r"); */
     /* LED blinks quickly */
     ledblinkingduration = 250;
   }
   
   else
   {
     /* New calibrated trimming are same as ones calibrated before run */
     /* printf("....... New calibrated trimming are same as ones calibrated before run \n\r"); */
     /* LED blinks slowly */
     ledblinkingduration = 1500;
   } 
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
  /* Select FOLLOWER_MODE */
  OpampHandle.Init.Mode = OPAMP_FOLLOWER_MODE;  	
  	
  /* Select VPx as input for OPAMP */
  OpampHandle.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO3;

  /* Timer controlled Mux mode is not used */
  OpampHandle.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;

  /* Inverting pin in PGA mode is not connected  */
  OpampHandle.Init.PgaConnect = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
  

  /* Select the user trimming (just calibrated) */
  OpampHandle.Init.UserTrimming = OPAMP_TRIMMING_USER;
  
  /* Init */
  HAL_OPAMP_Init(&OpampHandle);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
