/**
  ******************************************************************************
  * @file    ADC/ADC_Sequencer/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the ADC
  *          peripheral with sequencer, to convert several channels.
  *          Channels converted are 1 channel on external pin and 2 internal 
  *          channels (VrefInt and temperature sensor).
  *          Moreover, voltage and temperature are then computed.
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

/** @addtogroup ADC_Sequencer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VDD_APPLI                      ((uint32_t)3300)   /* Value of analog voltage supply Vdda (unit: mV) */
#define RANGE_12BITS                   ((uint32_t)4095)   /* Max value for a full range of 12 bits (4096 values) */
#define USERBUTTON_CLICK_COUNT_MAX     ((uint32_t)   4)   /* Maximum value of variable "UserButtonClickCount" */
#define TIMER_FREQUENCY_KHZ            ((uint32_t)   1)   /* Timer frequency (unit: kHz) */

#define ADCCONVERTEDVALUES_BUFFER_SIZE ((uint32_t)   3)   /* Size of array aADCxConvertedValues[]: set to ADC sequencer number of ranks converted, to have a rank in each address */

/* Internal temperature sensor: constants data used for indicative values in  */
/* this example. Refer to device datasheet for min/typ/max values.            */
/* For more accurate values, device should be calibrated on offset and slope  */
/* for application temperature range.                                         */
#define INTERNAL_TEMPSENSOR_V25        ((uint32_t)1430)         /* Internal temperature sensor, parameter V25 (unit: mV). Refer to device datasheet for min/typ/max values. */
#define INTERNAL_TEMPSENSOR_AVGSLOPE   ((uint32_t)4300)         /* Internal temperature sensor, parameter Avg_Slope (unit: uV/DegCelsius). Refer to device datasheet for min/typ/max values. */
#define TEMP110_CAL_ADDR  ((uint16_t*) ((uint32_t)0x1FFFF7C2))  /* Internal temperature sensor, parameter TS_CAL1: TS ADC raw data acquired at a temperature of  30 DegC (+-5 DegC), VDDA = 3.3 V (+-10 mV) */
#define TEMP30_CAL_ADDR   ((uint16_t*) ((uint32_t)0x1FFFF7B8))  /* Internal temperature sensor, parameter TS_CAL2: TS ADC raw data acquired at a temperature of 110 DegC (+-5 DegC), VDDA = 3.3 V (+-10 mV) */

/* Internal voltage reference */
#define VREFINT_CAL       ((uint16_t*) ((uint32_t)0x1FFFF7BA))  /* Internal temperature sensor, parameter VREFINT_CAL: Raw data acquired at a temperature of 30 DegC (+-5 DegC), VDDA = 3.3 V (+-10 mV) */

/* Private macro -------------------------------------------------------------*/
/**
  * @brief  Computation of temperature (unit: degree Celsius) from the internal
  *         temperature sensor measurement by ADC.
  *         Computation is using temperature sensor calibration values done
  *         in production.
  *         Computation formula:
  *         Temperature = (TS_ADC_DATA - TS_CAL1)/Avg_Slope + 30
  *         with TS_ADC_DATA = temperature sensor raw data measured by ADC
  *              Avg_Slope = (TS_CAL2 - TS_CAL1) / (110 - 30)
  *              TS_CAL1 = TS_ADC_DATA @30degC (calibrated in factory, vdda 3.3V)
  *              TS_CAL2 = TS_ADC_DATA @110degC (calibrated in factory, vdda 3.3V)
  *         Calculation validity conditioned to settings: 
  *          - ADC resolution 12 bits (need to scale value if using a different 
  *            resolution).
  *          - Power supply of analog voltage Vdda 3.3V (need to scale value 
  *            if using a different analog voltage supply value).
  * @param TS_ADC_DATA: Temperature sensor digital value measured by ADC
  * @retval None
  */
#define COMPUTATION_TEMPERATURE(TS_ADC_DATA)                                   \
  (((( ((int32_t)(TS_ADC_DATA) - (int32_t) *TEMP30_CAL_ADDR)                   \
     ) * (int32_t)(110 - 30)                                                   \
    ) / (int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR)                        \
   ) + 30                                                                      \
  )

/**
  * @brief  Computation of temperature (unit: degree Celsius) from the internal
  *         temperature sensor measurement by ADC.
  *         Computation is using temperature sensor standard parameters (refer
  *         to device datasheet).
  *         Computation formula:
  *         Temperature = (VTS - V25)/Avg_Slope + 25
  *         with VTS = temperature sensor voltage
  *              Avg_Slope = temperature sensor slope (unit: uV/DegCelsius)
  *              V25 = temperature sensor @25degC and Vdda 3.3V (unit: mV)
  *         Calculation validity conditioned to settings: 
  *          - ADC resolution 12 bits (need to scale value if using a different 
  *            resolution).
  *          - Power supply of analog voltage Vdda 3.3V (need to scale value 
  *            if using a different analog voltage supply value).
  * @param TS_ADC_DATA: Temperature sensor digital value measured by ADC
  * @retval None
  */
#define COMPUTATION_TEMPERATURE_STD_PARAMS(TS_ADC_DATA)                        \
  ((((INTERNAL_TEMPSENSOR_V25 - (((TS_ADC_DATA) * VDD_APPLI) / RANGE_12BITS)   \
     ) * 1000                                                                  \
    ) / INTERNAL_TEMPSENSOR_AVGSLOPE                                           \
   ) + 25                                                                      \
  )

/**
  * @brief  Computation of voltage (unit: mV) from ADC measurement.
  *         Calculation validity conditioned to settings: 
  *          - ADC resolution 12 bits (need to scale value if using a different 
  *            resolution).
  *          - Power supply of analog voltage Vdda 3.3V (need to scale value 
  *            if using a different analog voltage supply value).
  * @param ADC_DATA: Digital value measured by ADC
  * @retval None
  */
#define COMPUTATION_VOLTAGE(ADC_DATA)                                          \
  ( (ADC_DATA) * VDD_APPLI / RANGE_12BITS)
  
/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

/* DAC handler declaration */
DAC_HandleTypeDef    DacHandle;

/* Variable containing ADC conversions results */
__IO uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];

/* Variables for ADC conversions results computation to physical values */
uint16_t   uhADCChannelToDAC_mVolt = 0;
uint16_t   uhVrefInt_mVolt = 0;
 int32_t   wTemperature_DegreeCelsius = 0;
          
/* Variables to manage push button on board: interface between ExtLine interruption and main program */
uint8_t         ubUserButtonClickCount = 0;      /* Count number of clicks: Incremented after User Button interrupt */
uint8_t         ubUserButtonClickEvent = RESET;  /* Event detection: Set after User Button interrupt */

/* Variable to report ADC sequencer status */
uint8_t         ubSequenceCompleted = RESET;     /* Set when all ranks of the sequence have been converted */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void ADC_Config(void);
static void DAC_Config(void);

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

  /* Initialize LEDs on board */
  BSP_LED_Init(LED2);

  /* Configure the system clock to 64 MHz */
  SystemClock_Config();
  
  /* Configure User push-button in Interrupt mode */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Configure the ADC peripheral */
  ADC_Config();
    
  /* Run the ADC calibration in single-ended mode */  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }


  /* Configuration of DAC peripheral */
  DAC_Config();
  

  /*## Enable peripherals ####################################################*/
  
  /* Set DAC Channel data register: channel corresponding to ADC channel CHANNELa */
  /* Set DAC output to 1/2 of full range (4095 <=> Vdda=3.3V): 2048 <=> 1.65V              */
  if (HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL_TO_ADCx_CHANNELa, DAC_ALIGN_12B_R, RANGE_12BITS/2) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }
  
  /* Enable DAC Channel: channel corresponding to ADC channel CHANNELa */
  if (HAL_DAC_Start(&DacHandle, DACx_CHANNEL_TO_ADCx_CHANNELa) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
    
  /*## Start ADC conversions #################################################*/
  
  /* Start ADC conversion on regular group with transfer by DMA */
  if (HAL_ADC_Start_DMA(&AdcHandle,
                        (uint32_t *)aADCxConvertedValues,
                        ADCCONVERTEDVALUES_BUFFER_SIZE
                       ) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  
  
  /* Infinite loop */
  while (1)
  {  
    /* Wait for event on push button to perform following actions */
    while ((ubUserButtonClickEvent) == RESET)
    {
      __NOP();
    }
    /* Reset variable for loop iteration */
    ubUserButtonClickEvent = RESET;

    /* Set DAC voltage on channel corresponding to ADCx_CHANNELa              */
    /* in function of user button clicks count.                               */
    /* Set DAC output successively to:                                        */
    /*  - minimum of full range (0 <=> ground 0V)                             */
    /*  - 1/4 of full range (4095 <=> Vdda=3.3V): 1023 <=> 0.825V             */
    /*  - 1/2 of full range (4095 <=> Vdda=3.3V): 2048 <=> 1.65V              */
    /*  - 3/4 of full range (4095 <=> Vdda=3.3V): 3071 <=> 2.475V             */
    /*  - maximum of full range (4095 <=> Vdda=3.3V)                          */
    if (HAL_DAC_SetValue(&DacHandle,
                         DACx_CHANNEL_TO_ADCx_CHANNELa,
                         DAC_ALIGN_12B_R,
                         (RANGE_12BITS * ubUserButtonClickCount / USERBUTTON_CLICK_COUNT_MAX)
                        ) != HAL_OK)
    {
      /* Start Error */
      Error_Handler();
    }

    /* Wait for DAC settling time */
    HAL_Delay(1);
    
    /* Start ADC conversion */
    /* Since sequencer is enabled in discontinuous mode, this will perform    */
    /* the conversion of the next rank in sequencer.                          */
    /* Note: For this example, conversion is triggered by software start,     */
    /*       therefore "HAL_ADC_Start()" must be called for each conversion.  */
    /*       Since DMA transfer has been initiated previously by function     */
    /*       "HAL_ADC_Start_DMA()", this function will keep DMA transfer      */
    /*       active.                                                          */
    HAL_ADC_Start(&AdcHandle);
      
    /* Wait for conversion completion before conditional check hereafter */
    HAL_ADC_PollForConversion(&AdcHandle, 1);
    
    
    /* Turn-on/off LED2 in function of ADC sequencer status */
    /*  - Turn-off LED2 if sequencer has not yet converted all ranks */    
    /*  - Turn-on LED2 if sequencer has converted all ranks */
    if (ubSequenceCompleted == RESET)
    {
      BSP_LED_Off(LED2);
    }
    else
    {
      BSP_LED_On(LED2);
      
      /* Computation of ADC conversions raw data to physical values */
      /* Note: ADC results are transferred into array "aADCxConvertedValues"  */
      /*       in the order of their rank in ADC sequencer.                   */
      uhADCChannelToDAC_mVolt    = COMPUTATION_VOLTAGE(aADCxConvertedValues[0]);
      uhVrefInt_mVolt            = COMPUTATION_VOLTAGE(aADCxConvertedValues[2]);
      wTemperature_DegreeCelsius = COMPUTATION_TEMPERATURE(aADCxConvertedValues[1]);
      
      /* Reset variable for next loop iteration */
      ubSequenceCompleted = RESET;
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
/**
  * @brief  ADC configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef sConfig;

  /* Configuration of ADCx init structure: ADC parameters and regular group */
  AdcHandle.Instance = ADCx;

  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode          = ENABLE;                        /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 sequence converted at each conversion trig, and because discontinuous mode is enabled */
  AdcHandle.Init.NbrOfConversion       = 3;                             /* Sequencer of regular group will convert the 3 first ranks: rank1, rank2, rank3 */
  AdcHandle.Init.DiscontinuousConvMode = ENABLE;                        /* Sequencer of regular group will convert the sequence in several sub-divided sequences */
  AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Sequencer of regular group will convert ranks one by one, at each conversion trig */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Trig of conversion start done manually by software, without external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.DMAContinuousRequests = ENABLE;
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
 
  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  /* Note: Considering IT occurring after each ADC conversion (IT by DMA end  */
  /*       of transfer), select sampling time and ADC clock with sufficient   */
  /*       duration to not create an overhead situation in IRQHandler.        */
  /* Note: Set long sampling time due to internal channels (VrefInt,          */
  /*       temperature sensor) constraints.                                   */
  /*       For example, sampling time of temperature sensor must be higher    */
  /*       than 2.2us. Refer to device datasheet for min/typ/max values.      */
  sConfig.Channel      = ADCx_CHANNELa;
  sConfig.Rank         = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset       = 0;
  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
  /* Configuration of channel on ADCx regular group on sequencer rank 2 */
  /* Replicate previous rank settings, change only channel and rank */
  sConfig.Channel      = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank         = ADC_REGULAR_RANK_2;
  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
  /* Configuration of channel on ADCx regular group on sequencer rank 3 */
  /* Replicate previous rank settings, change only channel and rank */
  sConfig.Channel      = ADC_CHANNEL_VREFINT;
  sConfig.Rank         = ADC_REGULAR_RANK_3;
  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
}

/**
  * @brief  DAC configuration
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
  static DAC_ChannelConfTypeDef sConfig;

  /* Configuration of DACx peripheral */
  DacHandle.Instance = DACx;

  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* DAC initialization error */
    Error_Handler();
  }

  /* Configuration of DACx channel 1 */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL_TO_ADCx_CHANNELa) != HAL_OK)
  {
    /* Channel configuration error */
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
  if (GPIO_Pin == USER_BUTTON_PIN)
  {
    /* Set variable to report push button event to main program */
    ubUserButtonClickEvent = SET;
  
    /* Manage ubUserButtonClickCount to increment it circularly from 0 to     */
    /* maximum value defined                                                  */
    if (ubUserButtonClickCount < USERBUTTON_CLICK_COUNT_MAX)
    {
      ubUserButtonClickCount++;
    }      
    else
    {
      ubUserButtonClickCount=0;
    }
    
  }
}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
  /* Report to main program that ADC sequencer has reached its end */
  ubSequenceCompleted = SET;
}

/**
  * @brief  Conversion DMA half-transfer callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{

}

/**
  * @brief  ADC error callback in non blocking mode
  *        (ADC conversion with interruption or transfer by DMA)
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* In case of ADC error, call main error handler */
  Error_Handler();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with a potential error */
  
  /* In case of error, LED2 is toggling at a frequency of 1Hz */
  while(1)
  {
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
    HAL_Delay(500);
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
