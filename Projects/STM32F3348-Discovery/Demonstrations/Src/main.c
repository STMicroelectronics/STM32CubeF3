/**
  ******************************************************************************
  * @file    Demonstrations/Src/main.c 
  * @author  MCD Application Team
  * @brief   Buck LED Demonstration of STM32F3348-DISCO board
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

/** @addtogroup STM32F3348-Discovery_Demo
* @{
*/ 

/* Private typedef -----------------------------------------------------------*/ 
  
/* Private define ------------------------------------------------------------*/
#define MIN_INTENSITY        625
#define MAX_INTENSITY        0
#define MAX_INTENSITY_DIMMER 419

/* Test mode */
#define ADC_TRIGGER_VALUE 1000  /* ADC trigger Event time where noise is not present */
#define BLANK_TIME_TIMA   500   /* Blank Time between alternate BuckBoost Timer A initialization */
#define MAX_VOLTAGE       5500
#define MIN_TIME_TIMA     500   /* Min time corresponding to DutyCycle Max = 97% */
#define MIN_VOLTAGE       4500
#define SET_PERIOD        18432 /* Period equivalent to 250kHz frequency */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t       TriangCmp          = 0x100;
__IO uint8_t        TriangleGeneration = 0;
HRTIM_HandleTypeDef HrtimHandle;

static COMP_HandleTypeDef CompHandle;
static DAC_HandleTypeDef  DacHandle;
static DMA_HandleTypeDef  DmaHandle;
static __IO  uint8_t      Converter_Mode_Change = TRUE;
static __IO  uint8_t      Current_Mode          = BUCK;
static __IO  uint8_t      DownUp                = FALSE; /* UP DOWN LED intensity */
static __IO  uint8_t      NoWait                = TRUE;
static __IO  uint8_t      StateMachine          = STATE_OFF;
static __IO  uint8_t      TestMode              = FALSE;
static const uint16_t     DitherTab[8][8]       = {{0,0,0,0,0,0,0,0},
                                                   {0,0,0,0,0,0,0,1},
                                                   {0,0,0,1,0,0,0,1},
                                                   {0,0,1,0,0,1,0,1},
                                                   {0,1,0,1,0,1,0,1},
                                                   {0,1,0,1,1,0,1,1},
                                                   {0,1,1,1,0,1,1,1},
                                                   {0,1,1,1,1,1,1,1}};
static __IO  uint16_t     Keypressed            = FALSE; 
static __IO  uint32_t     CurrentSenseTab[5]    = {280, 240, 200, 360, 320}; /* Current table centered around ~250mA in the Power LED */
static __IO  float        fIntensity            = MIN_INTENSITY;

/* Test mode */
ADC_HandleTypeDef   AdcHandle;

static       uint16_t AdcReferenceConvertedValue      = 0;
static __IO  uint32_t Adc_Channel1_ConvertedValue_In  = 0;
static __IO  uint32_t Adc_Channel1_ConvertedValue_Out = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void ADC_Config(void);
static void BuckInit(void);
static void COMP_Config(void);
static void DAC_Config(void);
static void DMA_Config(void);
static void Error_Handler(void);
static void HRTIM_Config(void);
static void HRTIM_SetBurstCompare(float BurstCompare);
static void HRTIM_Unselect_OutputTIMx(void);
static void SetHRTIM_BoostMode(void);
static void SetHRTIM_BuckMode(void);
static void SystemClock_Config(void);
static void TestProgram(void);
static void TriangleGenerationInit(void);

/**
* @brief   Main program
* @param  None
* @retval None
*/
int main(void)
{
  uint8_t  toggle      = FALSE;
  uint32_t sensetab[5] = {0,0,0,0,0}; /* mA */
  uint32_t i;
  uint32_t tempo       = 400;
  double   midvalue    = MIN_INTENSITY;
  float    y;
  
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  files (startup_stm32f334x8.s) before to branch to application main. 
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f3xx.c file
  */  
  
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

  /* Configure the system clock to have a system clock = 72 Mhz */
  SystemClock_Config();

  /*Configure the SysTick to have interrupt in 10탎 time basis*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/100000);

  /* Allows temporization */
  NoWait = FALSE;
  
  /* Initialize LEDs available on STM32F3348-DISCO */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);
  
  /* Initialize User_Button on STM32F3348-DISCO */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);   
  
  /* Check if TestProgram is called by User button at startup*/
  if((BSP_PB_GetState(BUTTON_USER) == SET))
  {
    while((BSP_PB_GetState(BUTTON_USER) == SET))
    {
      /* Toggle LED5/LED6 to show TestProgram selection until key pressed */
      BSP_LED_On(LED6);
      HAL_Delay(70);
      BSP_LED_Off(LED6);
      BSP_LED_On(LED5);
      HAL_Delay(70);
      BSP_LED_Off(LED5);       
    } 
    BSP_LED_Off(LED6);   
    BSP_LED_Off(LED5);
    /* Jump to TestProgram function */
    TestMode = TRUE;
    TestProgram();
  }
  
  /* Initializes HRTIM, DAC, DMA and COMP to drive Buck LED*/
  BuckInit();
  
  /* Initializes HRTIM to generate a triangle signal on PB14 */
  TriangleGenerationInit();
  
  /* Infinite loop */
  while (1)
  { 
    switch (StateMachine)
    {
    case STATE_MAN:
      /* Disable HRTIM Master Burst period interrupt */
      __HAL_HRTIM_DISABLE_IT(&HrtimHandle, HRTIM_IT_BMPER);

      /* Reset all signal LEDs */
      BSP_LED_Off(LED3);
      BSP_LED_Off(LED4);
      BSP_LED_Off(LED5);
      BSP_LED_Off(LED6); 
      
      TriangleGeneration = 0;

      /* Disable the TD1 output */
      if(HAL_HRTIM_WaveformOutputStop(&HrtimHandle, HRTIM_OUTPUT_TD1) != HAL_OK)
      {
        Error_Handler();
      }
      
      /* By keeping pressed User push-button, intensity can be adjusted manually */
      /* Alternatively the LED intensity is increased or decreased */
      /* Always starts with low intensity value for first time */ 
      /* Decrease LED intensity */
      while((BSP_PB_GetState(BUTTON_USER) == SET) && (DownUp==TRUE))
      {
        /* Display Decrease LED */
        BSP_LED_On(LED6);
        BSP_LED_Off(LED3);
        
        /* Temporization active */
        NoWait = FALSE;
        HRTIM_SetBurstCompare(midvalue);
        midvalue++;
        HAL_Delay(10);
        /* Check if min. intensity reached */
        if (midvalue >= MIN_INTENSITY)
        {
          midvalue = MIN_INTENSITY;
          /* Then blink display LED min. intensity */
          BSP_LED_Off(LED6);
          HAL_Delay(100);
        }
      }
      
      /* Increase LED intensity */
      while((BSP_PB_GetState(BUTTON_USER) == SET) && (DownUp==FALSE))
      { 
        /* Display Increase LED */
        BSP_LED_Off(LED6);
        BSP_LED_On(LED3);        
        
        /* Temporization active */
        NoWait = FALSE;
        HRTIM_SetBurstCompare(midvalue);
        midvalue--;
        HAL_Delay(10);
        
        /* Check if max. intensity reached */
        if (midvalue <= MAX_INTENSITY)
        {
          midvalue = MAX_INTENSITY;
          /* Then blink display LED max. intensity */
          BSP_LED_Off(LED3);
          HAL_Delay(100);
        }
      }
      
      break; 
      
    case STATE_OFF:
      /* This mode disables LED dimmer or flash */
      /* Disable HRTIM Master Burst period interrupt */
      __HAL_HRTIM_DISABLE_IT(&HrtimHandle, HRTIM_IT_BMPER);
      DownUp = TRUE;
      midvalue = MIN_INTENSITY;

      /* BUCK LED is OFF, only signal LEDs are blinking */
      HRTIM_SetBurstCompare(MIN_INTENSITY);

      /* Define initial blinking time between two LEDs 400ms */
      NoWait = FALSE;

      /* Red LED ON */
      BSP_LED_On(LED3);
      HAL_Delay(tempo);
      BSP_LED_Off(LED3);

      /* Green LED ON */
      BSP_LED_On(LED5);
      HAL_Delay(tempo);
      BSP_LED_Off(LED5);

      /* Blue LED ON */
      BSP_LED_On(LED6);
      HAL_Delay(tempo);    
      BSP_LED_Off(LED6);

      /* Orange LED ON */
      BSP_LED_On(LED4);
      HAL_Delay(tempo);
      BSP_LED_Off(LED4);
      
      TriangleGeneration = 1;

      /* Enable the TD1 output */
      if(HAL_HRTIM_WaveformOutputStart(&HrtimHandle, HRTIM_OUTPUT_TD1) != HAL_OK)
      {
        Error_Handler();
      }
      
      /* Decrease time between blinking sequence */
      tempo = tempo / 2;
      if(tempo == 1)
      {
        tempo = 400;
      }
      break; 
      
    case STATE_DIMMER:
      
      /* The LED intensity is automatically controlled */
      /* Levels of current thresholds are progressively set */
      /* Intensity variation and flicker removal are enhanced thanks to dither sequences 
         that allows greater precision by steps */
      DownUp = TRUE;
      midvalue = MIN_INTENSITY;

      /* Reset all signal LEDs */
      BSP_LED_Off(LED3);
      BSP_LED_Off(LED4);
      BSP_LED_Off(LED5);
      BSP_LED_Off(LED6); 
      
      TriangleGeneration = 0;

      /* Disable the TD1 output */
      if(HAL_HRTIM_WaveformOutputStop(&HrtimHandle, HRTIM_OUTPUT_TD1) != HAL_OK)
      {
        Error_Handler();
      }
      
      NoWait = FALSE;

      /* Set signal LEDs to rising mode */
      BSP_LED_On(LED3);
      BSP_LED_Off(LED6);

      /* Start with current thresholds set to minimum */
      sensetab[0] = 80;
      sensetab[1] = 40;
      sensetab[2] = 0;
      sensetab[3] = 160;
      sensetab[4] = 120;
      CurrentSenseTab[0] = 0;
      CurrentSenseTab[1] = 0;
      CurrentSenseTab[2] = 0;
      CurrentSenseTab[3] = 0;
      CurrentSenseTab[4] = 0;

      /* Set fIntensity at minimum intensity */
      fIntensity = MIN_INTENSITY;

      /* Enable HRTIM Master Burst period interrupt */
      /* Each time the interrupt is serviced, the LED is driven */
      __HAL_HRTIM_ENABLE_IT(&HrtimHandle, HRTIM_IT_BMPER);

      /* Intensity changes from Min to Max */
      for(fIntensity = MIN_INTENSITY; (fIntensity >= MAX_INTENSITY_DIMMER) && (NoWait == FALSE); fIntensity -= 0.1f)
      {
        /* Increment progressively all thresholds up to the setting value (centered value is ~250mA)*/
        if(fIntensity > (605 + 0.1))
        {
          for(i = 0; i < 5; i++)
          {
            sensetab[i]++;
            CurrentSenseTab[i] = (sensetab[i] * 4096) / 3300;
          }
        }

        /* Delay between 2 steps is decreased exponentially */
        y = ((fIntensity / 100) * 1.1f);
        y = exp((double)y);
        HAL_Delay((uint32_t)(y / 100));
      }
      
      /* Set signal LEDs to falling mode */
      /* Intensity falling phase */
      BSP_LED_On(LED6);
      BSP_LED_Off(LED3);

      /* Intensity changes from Max to Min */
      for(fIntensity = MAX_INTENSITY_DIMMER; (fIntensity <= MIN_INTENSITY) && (NoWait == FALSE); fIntensity += 0.1f)
      {
        /* Decrement progressively all thresholds down to 0*/
        if(fIntensity > 605)
        {
          for (i = 0; i < 5; i++)
          {
            sensetab[i]--;
            CurrentSenseTab[i] = (sensetab[i] * 4096) / 3300;
          }
        }
        /* Delay between 2 steps is increased exponentially */
        y = ((fIntensity / 100) * 1.1f);
        y = exp((double)y);
        HAL_Delay((uint32_t)(y / 100));
      }      
      break;
      
    case STATE_FLASH:
      /* Flash mode consists to drive the LED @10Hz frequency (20% time ON) at maximum intensity */
      /* Disable HRTIM Master Burst period interrupt */      
      __HAL_HRTIM_DISABLE_IT(&HrtimHandle, HRTIM_IT_BMPER);

      /* (Re)Initialization of Current thresholds */
      DownUp = TRUE;
      midvalue = MIN_INTENSITY;

      /* Reset all signal LEDs except green*/       
      BSP_LED_Off(LED3);
      BSP_LED_Off(LED6);
      /* Alternatively signals LED4 and LED5 are toggled */ 
      if (toggle != FALSE)
      {
        BSP_LED_On(LED4);
        BSP_LED_Off(LED5);
      }
      else
      {
        BSP_LED_Off(LED4);
        BSP_LED_On(LED5);
      }
      
      TriangleGeneration = 0;

      /* Disable the TD1 output */
      if(HAL_HRTIM_WaveformOutputStop(&HrtimHandle, HRTIM_OUTPUT_TD1) != HAL_OK)
      {
        Error_Handler();
      }
      
      NoWait = FALSE; 

      /* freq= 10Hz tON = 20ms; tOFF = 80ms */
      /* BUCK LED ON */
      HRTIM_SetBurstCompare(MAX_INTENSITY);
      /* tON = 20ms */
      HAL_Delay(20);

      /* Alternatively signals LED4 and LED5 are toggled */ 
      if (toggle != FALSE)
      {
        BSP_LED_Off(LED4);
        BSP_LED_On(LED5);
      }
      else
      {
        BSP_LED_On(LED4);
        BSP_LED_Off(LED5);
      }     
      
      /* BUCK LED OFF */
      HRTIM_SetBurstCompare(MIN_INTENSITY);
      /* tOFF = 80 ms */
      HAL_Delay(80);
      toggle = (!toggle);
      
      break;
      
    default :
      Error_Handler();
    }
  }
}


/**
* @brief  Configures Buck
* @param  None
* @retval None
*/
static void BuckInit(void)
{  
  uint8_t index;

  /* Convert current sense value thresholds for DAC */
  for (index =0; index < 5; index++)
  {
    CurrentSenseTab[index] = (CurrentSenseTab[index] * 4096) / 3300; 
  }
  
  /* DMA Configuration */
  DMA_Config();
  
  /* DAC Configuration */
  DAC_Config();
  
  /* COMP Configuration */
  COMP_Config();
  
  /* HRTIM Configuration */
  HRTIM_Config(); 
}

/**
* @brief  DMA configuration
* @param  None
* @retval None
*/
static void DMA_Config(void)
{
  /* Enable DMA1 clock -------------------------------------------------------*/
  __HAL_RCC_DMA1_CLK_ENABLE();
  
  /* Configure the DMA1 CH5 IRQ Channel */
  DmaHandle.Init.Direction           = DMA_MEMORY_TO_PERIPH; /* M2P transfer mode                 */           
  DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;     /* Peripheral increment mode Disable */                 
  DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;      /* Memory increment mode Enable      */                   
  DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;  /* Peripheral data alignment : Word  */    
  DmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;  /* memory data alignment : Word      */     
  DmaHandle.Init.Mode                = DMA_CIRCULAR;         /* Circular Normal DMA mode          */  
  DmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;    /* priority level : high             */  

  DmaHandle.Instance                 = DMA1_Channel5;

  if(HAL_DMA_Init(&DmaHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
* @brief  DAC channel1 configuration
* @param  None
* @retval None
*/
static void DAC_Config(void)
{
  DAC_ChannelConfTypeDef DAC_ConfigStructure;

#ifdef DEBUG  
  GPIO_InitTypeDef       GPIO_InitStructure;

  /* Configure DAC1 OUT1 ******************************************************/ 
  /* GPIO Periph clock enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /* Configure PA4 (DAC1_OUT1) as analog */
  GPIO_InitStructure.Pin  =  GPIO_PIN_4;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
  
  /* DAC1 Periph clock enable */
  __HAL_RCC_DAC1_CLK_ENABLE();
  
  /* DAC1 deinitialize */
  DacHandle.Instance = DAC1;
  if(HAL_DAC_DeInit(&DacHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* DAC Channel1 Init */
  if(HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Fill DAC ConfigStructure */
  DAC_ConfigStructure.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
  DAC_ConfigStructure.DAC_Trigger      = DAC_TRIGGER_NONE;
  if(HAL_DAC_ConfigChannel(&DacHandle, &DAC_ConfigStructure, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Enable DAC Channel1 */
  if(HAL_DAC_Start(&DacHandle, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Set DAC Channel1 DHR register: DAC1_OUT */
  if(HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, CurrentSenseTab[0]) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
* @brief  COMP configuration
* @param  None
* @retval None
*/
static void COMP_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable SYSCFG clock */
//  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* GPIOB Peripheral clock enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure PB0 in analog mode: PB0 is connected to COMP4 non inverting input */
  GPIO_InitStructure.Pin  =  GPIO_PIN_0;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  
#ifdef DEBUG  
  /* COMP4 output config: PB1 for debug */
  GPIO_InitStructure.Pin       = GPIO_PIN_1;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  /* Alternate function configuration : Comparator4 Out2 / PB1 */ 
  GPIO_InitStructure.Alternate = GPIO_AF8_GPCOMP4;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
  
  /*  COMP4 deinitialize */
  CompHandle.Instance = COMP4;
  if(HAL_COMP_DeInit(&CompHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* COMP4 config */
  CompHandle.Init.InvertingInput    = COMP_INVERTINGINPUT_DAC1;
  CompHandle.Init.NonInvertingInput = 0;
  CompHandle.Init.Output            = COMP_OUTPUT_NONE;
  CompHandle.Init.OutputPol         = COMP_OUTPUTPOL_NONINVERTED;
  CompHandle.Init.Hysteresis        = COMP_HYSTERESIS_NONE;
  CompHandle.Init.Mode              = 0;
  CompHandle.Init.BlankingSrce      = COMP_BLANKINGSRCE_NONE;
  CompHandle.Init.WindowMode        = COMP_WINDOWMODE_DISABLE;
  CompHandle.Init.TriggerMode       = COMP_TRIGGERMODE_NONE;
  if(HAL_COMP_Init(&CompHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Enable COMP4 */
  if(HAL_COMP_Start(&CompHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
* @brief  HRTIM TIMC configuration
* @param  None
* @retval None
*/
static void HRTIM_Config(void)
{
  GPIO_InitTypeDef          GPIO_InitStructure;
  HRTIM_BurstModeCfgTypeDef HRTIM_BurstStructure; 
  HRTIM_CompareCfgTypeDef   HRTIM_CompareStructure;
  HRTIM_EventCfgTypeDef     HRTIM_ExternalEventStructure;
  HRTIM_OutputCfgTypeDef    HRTIM_OutputStructure;
  HRTIM_TimeBaseCfgTypeDef  HRTIM_TimeBaseStructure;
  HRTIM_TimerCfgTypeDef     HRTIM_TimerWaveStructure;

  /* Configure HRTIM TIM C ****************************************************/  
  /* HRTIM output channel configuration : HRTIM_CHC1 (Buck drive) / PB12 */
  GPIO_InitStructure.Pin       = GPIO_PIN_12;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  /* Alternate function configuration : HRTIM_CHC1 / PB12 */
  GPIO_InitStructure.Alternate = GPIO_AF13_HRTIM1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Use the PLLx2 clock for HRTIM */
  __HAL_RCC_HRTIM1_CONFIG(RCC_HRTIM1CLK_PLLCLK);
  __HAL_RCC_HRTIM1_CLK_ENABLE();

  HrtimHandle.Instance = HRTIM1;
  HAL_HRTIM_Init(&HrtimHandle);
  
  /* DMA Configuration */
  __HAL_LINKDMA(&HrtimHandle, hdmaTimerC, DmaHandle);
  
  /* HRTIM initialization startup */  
  if(HAL_HRTIM_DLLCalibrationStart(&HrtimHandle, HRTIM_CALIBRATIONRATE_14) != HAL_OK)
  {
    Error_Handler();
  }
  
  if(HAL_HRTIM_PollForDLLCalibration(&HrtimHandle, HAL_MAX_DELAY) != HAL_OK)
  {
    Error_Handler();
  }
   
  /* Configure the output features */  
  HRTIM_OutputStructure.Polarity              = HRTIM_OUTPUTPOLARITY_HIGH; 
  HRTIM_OutputStructure.SetSource             = HRTIM_OUTPUTSET_TIMPER;  
  HRTIM_OutputStructure.ResetSource           = HRTIM_OUTPUTRESET_EEV_2; 
  HRTIM_OutputStructure.IdleMode              = HRTIM_OUTPUTIDLEMODE_IDLE;  
  HRTIM_OutputStructure.IdleLevel             = HRTIM_OUTPUTIDLELEVEL_INACTIVE;          
  HRTIM_OutputStructure.FaultLevel            = HRTIM_OUTPUTFAULTLEVEL_NONE;          
  HRTIM_OutputStructure.ChopperModeEnable     = HRTIM_OUTPUTCHOPPERMODE_DISABLED;        
  HRTIM_OutputStructure.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_C, 
                                    HRTIM_OUTPUT_TC1, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }

  /* Configure HRTIM1_TIMC Deadtime */
  HRTIM_TimerWaveStructure.InterruptRequests     = (HRTIM_MASTER_IT_NONE | HRTIM_TIM_IT_NONE);
  HRTIM_TimerWaveStructure.DMARequests           = (HRTIM_TIM_DMA_REP | HRTIM_TIM_DMA_CMP1 | HRTIM_TIM_DMA_CMP2 |
                                                    HRTIM_TIM_DMA_CMP3 | HRTIM_TIM_DMA_CMP4);
  HRTIM_TimerWaveStructure.DMASrcAddress         = (uint32_t)CurrentSenseTab;
  HRTIM_TimerWaveStructure.DMADstAddress         = (uint32_t)(&((&DacHandle)->Instance->DHR12R1));
  HRTIM_TimerWaveStructure.DMASize               = 5;
  HRTIM_TimerWaveStructure.HalfModeEnable        = HRTIM_HALFMODE_DISABLED;
  HRTIM_TimerWaveStructure.StartOnSync           = HRTIM_SYNCSTART_DISABLED;
  HRTIM_TimerWaveStructure.ResetOnSync           = HRTIM_SYNCRESET_DISABLED;
  HRTIM_TimerWaveStructure.DACSynchro            = HRTIM_DACSYNC_NONE;
  HRTIM_TimerWaveStructure.PreloadEnable         = HRTIM_PRELOAD_DISABLED;
  HRTIM_TimerWaveStructure.UpdateGating          = HRTIM_UPDATEGATING_INDEPENDENT;
  HRTIM_TimerWaveStructure.BurstMode             = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  HRTIM_TimerWaveStructure.RepetitionUpdate      = HRTIM_UPDATEONREPETITION_DISABLED;
  HRTIM_TimerWaveStructure.PushPull              = HRTIM_TIMPUSHPULLMODE_DISABLED;
  HRTIM_TimerWaveStructure.FaultEnable           = HRTIM_TIMFAULTENABLE_NONE;
  HRTIM_TimerWaveStructure.FaultLock             = HRTIM_TIMFAULTLOCK_READWRITE;
  HRTIM_TimerWaveStructure.DeadTimeInsertion     = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  HRTIM_TimerWaveStructure.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  HRTIM_TimerWaveStructure.UpdateTrigger         = HRTIM_TIMUPDATETRIGGER_TIMER_C; 
  HRTIM_TimerWaveStructure.ResetTrigger          = HRTIM_TIMRESETTRIGGER_NONE; 
  HRTIM_TimerWaveStructure.ResetUpdate           = HRTIM_TIMUPDATEONRESET_DISABLED;
  
  if(HAL_HRTIM_WaveformTimerConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_C, &HRTIM_TimerWaveStructure) != HAL_OK)
  {
    Error_Handler();
  }
  
  HRTIM_CompareStructure.AutoDelayedMode    = HRTIM_AUTODELAYEDMODE_REGULAR;
  HRTIM_CompareStructure.AutoDelayedTimeout = 0; 
  HRTIM_CompareStructure.CompareValue       = 3686; /* 20% time */
  
  if(HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_C, 
                                     HRTIM_COMPAREUNIT_1, &HRTIM_CompareStructure) != HAL_OK)
  {
    Error_Handler();
  }
  
  HRTIM_CompareStructure.AutoDelayedMode    = HRTIM_AUTODELAYEDMODE_REGULAR;
  HRTIM_CompareStructure.AutoDelayedTimeout = 0; 
  HRTIM_CompareStructure.CompareValue       = 7373; /* 40% time */

  if(HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_C, 
                                     HRTIM_COMPAREUNIT_2, &HRTIM_CompareStructure) != HAL_OK)
  {
    Error_Handler();
  }
  
  HRTIM_CompareStructure.AutoDelayedMode    = HRTIM_AUTODELAYEDMODE_REGULAR;
  HRTIM_CompareStructure.AutoDelayedTimeout = 0; 
  HRTIM_CompareStructure.CompareValue       = 11059; /* 60% time */

  if(HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_C, 
                                     HRTIM_COMPAREUNIT_3, &HRTIM_CompareStructure) != HAL_OK)
  {
    Error_Handler();
  }
  
  HRTIM_CompareStructure.AutoDelayedMode    = HRTIM_AUTODELAYEDMODE_REGULAR;
  HRTIM_CompareStructure.AutoDelayedTimeout = 0; 
  HRTIM_CompareStructure.CompareValue       = 14746; /* 80% time */

  if(HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_C, 
                                     HRTIM_COMPAREUNIT_4, &HRTIM_CompareStructure) != HAL_OK)
  {
    Error_Handler();
  }
  
  HRTIM_TimeBaseStructure.Period            = 18432; /* 1 period = 4 탎 = 100% time */
  HRTIM_TimeBaseStructure.RepetitionCounter = 0x00;
  HRTIM_TimeBaseStructure.PrescalerRatio    = HRTIM_PRESCALERRATIO_MUL32; 
  HRTIM_TimeBaseStructure.Mode              = HRTIM_MODE_CONTINUOUS;          
  
  if(HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_C, &HRTIM_TimeBaseStructure) != HAL_OK)
  {
    Error_Handler();
  }
    
  /* Configure External Event Source 2 */
  HRTIM_ExternalEventStructure.Source      = HRTIM_EVENTSRC_2; 
  HRTIM_ExternalEventStructure.Polarity    = HRTIM_EVENTPOLARITY_HIGH; 
  HRTIM_ExternalEventStructure.Sensitivity = HRTIM_EVENTSENSITIVITY_LEVEL;
  HRTIM_ExternalEventStructure.FastMode    = HRTIM_EVENTFASTMODE_ENABLE;
  HRTIM_ExternalEventStructure.Filter      = HRTIM_EVENTFILTER_NONE;
  
  if(HAL_HRTIM_EventConfig(&HrtimHandle, HRTIM_EVENT_2, &HRTIM_ExternalEventStructure) != HAL_OK)
  {
    Error_Handler();
  }
    
  /* Burst Mode configuration */
  HRTIM_BurstStructure.Mode          = HRTIM_BURSTMODE_CONTINOUS;
  HRTIM_BurstStructure.ClockSource   = HRTIM_BURSTMODECLOCKSOURCE_TIMER_C; 
  HRTIM_BurstStructure.Prescaler     = HRTIM_BURSTMODEPRESCALER_DIV1;
  HRTIM_BurstStructure.PreloadEnable = HRIM_BURSTMODEPRELOAD_ENABLED;
  HRTIM_BurstStructure.Trigger       = HRTIM_BURSTMODETRIGGER_NONE;
  HRTIM_BurstStructure.IdleDuration  = MIN_INTENSITY;
  HRTIM_BurstStructure.Period        = MIN_INTENSITY;  
  
  if(HAL_HRTIM_BurstModeConfig(&HrtimHandle, &HRTIM_BurstStructure) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Enable the TC1 output */
  if(HAL_HRTIM_WaveformOutputStart(&HrtimHandle, HRTIM_OUTPUT_TC1) != HAL_OK)
  {
    Error_Handler();
  }
  
  if(HAL_HRTIM_BurstModeCtl(&HrtimHandle, HRTIM_BURSTMODECTL_ENABLED) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start slave*/
  if(HAL_HRTIM_WaveformCountStart_DMA(&HrtimHandle, HRTIM_TIMERID_TIMER_C) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Configure and enable HRTIM interrupt */
  HAL_NVIC_SetPriority(HRTIM1_Master_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(HRTIM1_Master_IRQn);

  /* Enable Burst mode period completed interrupt */
  HrtimHandle.Init.HRTIMInterruptResquests = HRTIM_IT_BMPER;

  /* Select Burst Trigger */
  if(HAL_HRTIM_BurstModeSoftwareTrigger(&HrtimHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
* @brief  Set HRTIM to generate a triangle on channel D1
* @param  None
* @retval None
*/
static void TriangleGenerationInit(void)
{
  GPIO_InitTypeDef         GPIO_InitStructure;
  HRTIM_OutputCfgTypeDef   HRTIM_OutputStructure;
  HRTIM_TimeBaseCfgTypeDef HRTIM_TimeBaseStructure;
  HRTIM_TimerCfgTypeDef    HRTIM_TimerWaveStructure;
  HRTIM_CompareCfgTypeDef  HRTIM_CompareStructure;
  
  /* GPIOB Peripheral clock enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /* HRTIM output channel configuration : HRTIM_CHD1 / PB14 */
  GPIO_InitStructure.Pin       = GPIO_PIN_14;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  /* Alternate function configuration : HRTIM_CHD1 / PB14 */
  GPIO_InitStructure.Alternate = GPIO_AF13_HRTIM1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure the output features */  
  HRTIM_OutputStructure.Polarity              = HRTIM_OUTPUTPOLARITY_HIGH; 
  HRTIM_OutputStructure.SetSource             = HRTIM_OUTPUTSET_TIMPER;  
  HRTIM_OutputStructure.ResetSource           = HRTIM_OUTPUTRESET_TIMCMP1; 
  HRTIM_OutputStructure.IdleMode              = HRTIM_OUTPUTIDLEMODE_NONE;  
  HRTIM_OutputStructure.IdleLevel             = HRTIM_OUTPUTIDLELEVEL_INACTIVE;          
  HRTIM_OutputStructure.FaultLevel            = HRTIM_OUTPUTFAULTLEVEL_NONE;          
  HRTIM_OutputStructure.ChopperModeEnable     = HRTIM_OUTPUTCHOPPERMODE_DISABLED;        
  HRTIM_OutputStructure.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, 
                                    HRTIM_OUTPUT_TD1, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }

  /* Configure HRTIM1_TIMD Deadtime */
  HRTIM_TimerWaveStructure.InterruptRequests     = (HRTIM_MASTER_IT_NONE | HRTIM_TIM_IT_NONE);
  HRTIM_TimerWaveStructure.DMARequests           = (HRTIM_MASTER_DMA_NONE | HRTIM_TIM_DMA_NONE);
  HRTIM_TimerWaveStructure.DMASrcAddress         = 0;
  HRTIM_TimerWaveStructure.DMADstAddress         = 0;
  HRTIM_TimerWaveStructure.DMASize               = 0;
  HRTIM_TimerWaveStructure.HalfModeEnable        = HRTIM_HALFMODE_DISABLED;
  HRTIM_TimerWaveStructure.StartOnSync           = HRTIM_SYNCSTART_DISABLED;
  HRTIM_TimerWaveStructure.ResetOnSync           = HRTIM_SYNCRESET_DISABLED;
  HRTIM_TimerWaveStructure.DACSynchro            = HRTIM_DACSYNC_NONE;
  HRTIM_TimerWaveStructure.PreloadEnable         = HRTIM_PRELOAD_DISABLED;
  HRTIM_TimerWaveStructure.UpdateGating          = HRTIM_UPDATEGATING_INDEPENDENT;
  HRTIM_TimerWaveStructure.BurstMode             = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  HRTIM_TimerWaveStructure.RepetitionUpdate      = HRTIM_UPDATEONREPETITION_DISABLED;
  HRTIM_TimerWaveStructure.PushPull              = HRTIM_TIMPUSHPULLMODE_DISABLED;
  HRTIM_TimerWaveStructure.FaultEnable           = HRTIM_TIMFAULTENABLE_NONE;
  HRTIM_TimerWaveStructure.FaultLock             = HRTIM_TIMFAULTLOCK_READWRITE;
  HRTIM_TimerWaveStructure.DeadTimeInsertion     = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
  HRTIM_TimerWaveStructure.DelayedProtectionMode = HRTIM_TIMER_D_E_DELAYEDPROTECTION_DISABLED;
  HRTIM_TimerWaveStructure.UpdateTrigger         = HRTIM_TIMUPDATETRIGGER_TIMER_D; 
  HRTIM_TimerWaveStructure.ResetTrigger          = HRTIM_TIMRESETTRIGGER_NONE; 
  HRTIM_TimerWaveStructure.ResetUpdate           = HRTIM_TIMUPDATEONRESET_DISABLED;
  
  if(HAL_HRTIM_WaveformTimerConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, &HRTIM_TimerWaveStructure) != HAL_OK)
  {
    Error_Handler();
  }

  HRTIM_TimeBaseStructure.Period            = 0x900; /* 2MHz => 2304 = 0x900 (timer input clock freq is 144MHz x 32)*/
  HRTIM_TimeBaseStructure.RepetitionCounter = 0x00;
  HRTIM_TimeBaseStructure.PrescalerRatio    = HRTIM_PRESCALERRATIO_MUL32; 
  HRTIM_TimeBaseStructure.Mode              = HRTIM_MODE_CONTINUOUS;          
  
  if(HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, &HRTIM_TimeBaseStructure) != HAL_OK)
  {
    Error_Handler();
  }
  
  HRTIM_CompareStructure.AutoDelayedMode    = HRTIM_AUTODELAYEDMODE_REGULAR;
  HRTIM_CompareStructure.AutoDelayedTimeout = 0; /* meaningless in regular mode */
  HRTIM_CompareStructure.CompareValue       = TriangCmp;
  
  if(HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_D, 
                                     HRTIM_COMPAREUNIT_1, &HRTIM_CompareStructure) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable the TD1 output */
  if(HAL_HRTIM_WaveformOutputStart(&HrtimHandle, HRTIM_OUTPUT_TD1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Start slave timer D*/
  if(HAL_HRTIM_WaveformCountStart(&HrtimHandle, HRTIM_TIMERID_TIMER_D) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
* @brief  Set Burst Compare value
* @param  BurstCompare: Burst Compare value, it can be a value between 0x0 and 0xFFFF 
* @retval None
*/
static void HRTIM_SetBurstCompare(float BurstCompare)
{
  /* Set Burst Compare value */
  HrtimHandle.Instance->sCommonRegs.BMCMPR = (uint16_t)BurstCompare;
}

/**
* @brief  Checks the Discovery Kit features.
* @param  None
* @retval None
*/
static void TestProgram(void)
{  
  uint16_t refint_cal      = (uint16_t) *(__IO uint32_t *)((uint32_t)0x1FFFF7BA);
  uint32_t duty_cycle_timA = 2500; /*Buck Boost duty cycle Timer A initialization */
  uint32_t vin = 0;
  uint32_t vout = 0;
  uint32_t vout_target     = 5000; /* Default Vout target set in mV */
  float    ratio           = 2.0;

  GPIO_InitTypeDef         GPIO_InitStructure;
  HRTIM_CompareCfgTypeDef  HRTIM_CompareStructure;
  HRTIM_DeadTimeCfgTypeDef HRTIM_DeadTimeStructure;
  HRTIM_OutputCfgTypeDef   HRTIM_OutputStructure;
  HRTIM_TimeBaseCfgTypeDef HRTIM_TimeBaseStructure;
  HRTIM_TimerCfgTypeDef    HRTIM_TimerWaveStructure;
   
  /* Use the PLLx2 clock for HRTIM */
  __HAL_RCC_HRTIM1_CONFIG(RCC_HRTIM1CLK_PLLCLK);
  
  /* Enable the HRTIM, SYSCFG and GPIOs (port A, B, C) Clocks */
  __HAL_RCC_HRTIM1_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  /* Configure H bridge BuckBoost converter N and P MOS output config */
  GPIO_InitStructure.Pin       = (GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11);
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  /* Configure HRTIM drivers PA8/PA9/PA10/PA11 HRTIM_CHA1/A2/B1/B2 */
  /* Alternate function configuration : HRTIM_CHA1/A2/B1/B2 / PA8-9-10-11 */
  GPIO_InitStructure.Alternate = GPIO_AF13_HRTIM1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  
#ifdef Debug  
  /* For Debug only */
  GPIO_InitStructure.Pin   = GPIO_PIN_4;
  GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
  
  /* Initialize User_Button on STM32F3348-DISCO */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
    
  /* HRTIM intialisation startup */  
  HrtimHandle.Instance = HRTIM1;
  if(HAL_HRTIM_DLLCalibrationStart(&HrtimHandle, HRTIM_CALIBRATIONRATE_14) != HAL_OK)
  {
    Error_Handler();
  }
  
  if(HAL_HRTIM_PollForDLLCalibration(&HrtimHandle, HAL_MAX_DELAY) != HAL_OK)
  {
    Error_Handler();
  }
   
  /* Configure the output features */  
  HRTIM_OutputStructure.Polarity              = HRTIM_OUTPUTPOLARITY_HIGH; 
  HRTIM_OutputStructure.SetSource             = HRTIM_OUTPUTSET_TIMPER;  
  HRTIM_OutputStructure.ResetSource           = HRTIM_OUTPUTRESET_TIMCMP1; 
  HRTIM_OutputStructure.IdleMode              = HRTIM_OUTPUTIDLEMODE_NONE;  
  HRTIM_OutputStructure.IdleLevel             = HRTIM_OUTPUTIDLELEVEL_INACTIVE;          
  HRTIM_OutputStructure.FaultLevel            = HRTIM_OUTPUTFAULTLEVEL_NONE;          
  HRTIM_OutputStructure.ChopperModeEnable     = HRTIM_OUTPUTCHOPPERMODE_DISABLED;        
  HRTIM_OutputStructure.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                    HRTIM_OUTPUT_TA1, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }
  
  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, 
                                    HRTIM_OUTPUT_TB1, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }

  /* Initialize HRTIM1_TIMA and HRTIM1_TIMB */
  /* HRTIM1_TIMA and HRTIM1_TIMB Deadtime enable */
  HRTIM_TimerWaveStructure.InterruptRequests     = (HRTIM_MASTER_IT_NONE | HRTIM_TIM_IT_NONE);
  HRTIM_TimerWaveStructure.DMARequests           = (HRTIM_MASTER_DMA_NONE | HRTIM_TIM_DMA_NONE);
  HRTIM_TimerWaveStructure.DMASrcAddress         = 0;
  HRTIM_TimerWaveStructure.DMADstAddress         = 0;
  HRTIM_TimerWaveStructure.DMASize               = 0;
  HRTIM_TimerWaveStructure.HalfModeEnable        = HRTIM_HALFMODE_DISABLED;
  HRTIM_TimerWaveStructure.StartOnSync           = HRTIM_SYNCSTART_DISABLED;
  HRTIM_TimerWaveStructure.ResetOnSync           = HRTIM_SYNCRESET_DISABLED;
  HRTIM_TimerWaveStructure.DACSynchro            = HRTIM_DACSYNC_NONE;
  HRTIM_TimerWaveStructure.PreloadEnable         = HRTIM_PRELOAD_ENABLED;
  HRTIM_TimerWaveStructure.UpdateGating          = HRTIM_UPDATEGATING_INDEPENDENT;
  HRTIM_TimerWaveStructure.BurstMode             = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
  HRTIM_TimerWaveStructure.RepetitionUpdate      = HRTIM_UPDATEONREPETITION_ENABLED;
  HRTIM_TimerWaveStructure.PushPull              = HRTIM_TIMPUSHPULLMODE_DISABLED;
  HRTIM_TimerWaveStructure.FaultEnable           = HRTIM_TIMFAULTENABLE_NONE;
  HRTIM_TimerWaveStructure.FaultLock             = HRTIM_TIMFAULTLOCK_READWRITE;
  HRTIM_TimerWaveStructure.DeadTimeInsertion     = HRTIM_TIMDEADTIMEINSERTION_ENABLED;
  HRTIM_TimerWaveStructure.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
  HRTIM_TimerWaveStructure.UpdateTrigger         = HRTIM_TIMUPDATETRIGGER_NONE; 
  HRTIM_TimerWaveStructure.ResetTrigger          = HRTIM_TIMRESETTRIGGER_NONE; 
  HRTIM_TimerWaveStructure.ResetUpdate           = HRTIM_TIMUPDATEONRESET_DISABLED;
  
  if(HAL_HRTIM_WaveformTimerConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, &HRTIM_TimerWaveStructure) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_HRTIM_WaveformTimerConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, &HRTIM_TimerWaveStructure) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure HRTIM1_TIMA Deadtime */
  HRTIM_DeadTimeStructure.FallingLock     = HRTIM_TIMDEADTIME_FALLINGLOCK_WRITE;
  HRTIM_DeadTimeStructure.FallingSign     = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE;
  HRTIM_DeadTimeStructure.FallingSignLock = HRTIM_TIMDEADTIME_FALLINGSIGNLOCK_WRITE;
  HRTIM_DeadTimeStructure.FallingValue    = QB_ON_DEADTIME;
  HRTIM_DeadTimeStructure.Prescaler       = HRTIM_TIMDEADTIME_PRESCALERRATIO_MUL8;
  HRTIM_DeadTimeStructure.RisingLock      = HRTIM_TIMDEADTIME_RISINGLOCK_WRITE;
  HRTIM_DeadTimeStructure.RisingSign      = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE;
  HRTIM_DeadTimeStructure.RisingSignLock  = HRTIM_TIMDEADTIME_RISINGSIGNLOCK_WRITE;
  HRTIM_DeadTimeStructure.RisingValue     = QB_OFF_DEADTIME;
  
  if(HAL_HRTIM_DeadTimeConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, &HRTIM_DeadTimeStructure) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Configure HRTIM1_TIMB Deadtime */
  HRTIM_DeadTimeStructure.FallingLock     = HRTIM_TIMDEADTIME_FALLINGLOCK_WRITE;
  HRTIM_DeadTimeStructure.FallingSign     = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE;
  HRTIM_DeadTimeStructure.FallingSignLock = HRTIM_TIMDEADTIME_FALLINGSIGNLOCK_WRITE;
  HRTIM_DeadTimeStructure.FallingValue    = QD_ON_DEADTIME;
  HRTIM_DeadTimeStructure.Prescaler       = HRTIM_TIMDEADTIME_PRESCALERRATIO_MUL8;
  HRTIM_DeadTimeStructure.RisingLock      = HRTIM_TIMDEADTIME_RISINGLOCK_WRITE;
  HRTIM_DeadTimeStructure.RisingSign      = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE;
  HRTIM_DeadTimeStructure.RisingSignLock  = HRTIM_TIMDEADTIME_RISINGSIGNLOCK_WRITE;
  HRTIM_DeadTimeStructure.RisingValue     = QD_OFF_DEADTIME;
  
  if(HAL_HRTIM_DeadTimeConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, &HRTIM_DeadTimeStructure) != HAL_OK)
  {
    Error_Handler();
  }

  /*HRTIM 250KHz configuration */
  HRTIM_TimeBaseStructure.Period            = SET_PERIOD; /* 1 period = 4 탎 = 100% time */
  HRTIM_TimeBaseStructure.RepetitionCounter = 10;
  HRTIM_TimeBaseStructure.PrescalerRatio    = HRTIM_PRESCALERRATIO_MUL32; 
  HRTIM_TimeBaseStructure.Mode              = HRTIM_MODE_CONTINUOUS;          
  
  if(HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, &HRTIM_TimeBaseStructure) != HAL_OK)
  {
    Error_Handler();
  }

  HRTIM_CompareStructure.AutoDelayedMode    = HRTIM_AUTODELAYEDMODE_REGULAR;
  HRTIM_CompareStructure.AutoDelayedTimeout = duty_cycle_timA; 
  HRTIM_CompareStructure.CompareValue       = duty_cycle_timA; 
  
  if(HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                     HRTIM_COMPAREUNIT_1, &HRTIM_CompareStructure) != HAL_OK)
  {
    Error_Handler();
  }

  HRTIM_CompareStructure.AutoDelayedMode    = HRTIM_AUTODELAYEDMODE_REGULAR;
  HRTIM_CompareStructure.AutoDelayedTimeout = duty_cycle_timA + BLANK_TIME_TIMA; 
  HRTIM_CompareStructure.CompareValue       = duty_cycle_timA + BLANK_TIME_TIMA;

  if(HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                     HRTIM_COMPAREUNIT_2, &HRTIM_CompareStructure) != HAL_OK)
  {
    Error_Handler();
  }
  
  HRTIM_CompareStructure.AutoDelayedMode    = HRTIM_AUTODELAYEDMODE_REGULAR;
  HRTIM_CompareStructure.AutoDelayedTimeout = (uint32_t)ratio*duty_cycle_timA + BLANK_TIME_TIMA; 
  HRTIM_CompareStructure.CompareValue       = (uint32_t)ratio*duty_cycle_timA + BLANK_TIME_TIMA;
  
  if(HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                     HRTIM_COMPAREUNIT_4, &HRTIM_CompareStructure) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_HRTIM_TimeBaseConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, &HRTIM_TimeBaseStructure) != HAL_OK)
  {
    Error_Handler();
  }
  
  HRTIM_CompareStructure.AutoDelayedMode    = HRTIM_AUTODELAYEDMODE_REGULAR;
  HRTIM_CompareStructure.AutoDelayedTimeout = ADC_TRIGGER_VALUE; 
  HRTIM_CompareStructure.CompareValue       = ADC_TRIGGER_VALUE;

  if(HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, 
                                     HRTIM_COMPAREUNIT_2, &HRTIM_CompareStructure) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* End HRTIM 250KHz configuration */
  
  /* Configure and enable HRTIM interrupt */
  HAL_NVIC_SetPriority(HRTIM1_TIMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(HRTIM1_TIMA_IRQn);
  
  /* Enable the TA1 output */
  if(HAL_HRTIM_WaveformOutputStart(&HrtimHandle, HRTIM_OUTPUT_TA1) != HAL_OK)
  {
    Error_Handler();
  }
  /* Enable the TA2 output */
  if(HAL_HRTIM_WaveformOutputStart(&HrtimHandle, HRTIM_OUTPUT_TA2) != HAL_OK)
  {
    Error_Handler();
  }
  /* Enable the TB1 output */
  if(HAL_HRTIM_WaveformOutputStart(&HrtimHandle, HRTIM_OUTPUT_TB1) != HAL_OK)
  {
    Error_Handler();
  }
  /* Enable the TB2 output */
  if(HAL_HRTIM_WaveformOutputStart(&HrtimHandle, HRTIM_OUTPUT_TB2) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Start H bridge converter in Buck Mode */
  SetHRTIM_BuckMode();
  
  if(HAL_HRTIM_SimpleBaseStart_IT(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A) != HAL_OK)
  {
    Error_Handler();
  }

  /* ADC Configuration */
  ADC_Config();
  
  HAL_Delay(100); 
  while(1)
  {
    uint32_t timeout = 0 ;
    
    /* Check if VIN connected to 5V and wait until voltage is present and included in the expected range */
    /* Range is 4.5V to 5.5V */
    while ((vin < MIN_VOLTAGE) || (vin > MAX_VOLTAGE))
    {
      /* Enable next trigger on ADC during data process*/
      if(HAL_ADCEx_InjectedStart_IT(&AdcHandle) != HAL_OK)
      {
        Error_Handler();
      }

      /* Compute the voltage (Voltages have to be multiplied by ~5 as bridge divider is ~20%) */
      Adc_Channel1_ConvertedValue_In = (Adc_Channel1_ConvertedValue_In * refint_cal) / AdcReferenceConvertedValue;
      vin = (Adc_Channel1_ConvertedValue_In * 3300) / 0xFFF;

      /* VIN bridge conversion is 4.97 (6.8K / 6.8K + 27K) */
      vin = (497 * vin ) / 100;
      timeout++;
      if (timeout >= 100)
      {
        while(1)
        {   
          /* Orange LED is blinking to alert on missing input voltage */
          BSP_LED_On(LED4);
          HAL_Delay(100);
          BSP_LED_Off(LED4);
          HAL_Delay(100);
        }
      }
    }

    /* Input voltage connected with expected value: Green LED is ON */
    BSP_LED_On(LED5);

    /* Set Boost Mode and Vout target value to 10V  */
    vout_target = 10000;
    Current_Mode = BOOST;
    Converter_Mode_Change = TRUE; 
    HAL_Delay(100);

    /* Enable next trigger on ADC during data process*/
    if(HAL_ADCEx_InjectedStart_IT(&AdcHandle) != HAL_OK)
    {
      Error_Handler();
    }
    
    while((vout < (vout_target - 5)) || (vout > (vout_target + 5)))
    {
      /* VIN and VOUT ADC sampling */
      Adc_Channel1_ConvertedValue_In = (Adc_Channel1_ConvertedValue_In * refint_cal) / AdcReferenceConvertedValue;
      Adc_Channel1_ConvertedValue_Out = (Adc_Channel1_ConvertedValue_Out * refint_cal) / AdcReferenceConvertedValue;
      
      vin = (Adc_Channel1_ConvertedValue_In * 3300) / 0xFFF;
      vout = (Adc_Channel1_ConvertedValue_Out * 3300) / 0xFFF;
      vin = (497 * vin )/100;

      /* VOUT bridge conversion is 5.03 (3.3K / 3.3K + 13.3K) */
      vout = (503 * vout)/100;

      /* Control Timer A Duty Cycle to reach the VOUT target */ 
      if(vout <= vout_target)
      {
        duty_cycle_timA += 10;
        if (duty_cycle_timA >= (SET_PERIOD - MIN_TIME_TIMA))
        {
          duty_cycle_timA = (SET_PERIOD - MIN_TIME_TIMA);
        }
      }
      else
      {
        duty_cycle_timA -= 10;
        if (duty_cycle_timA <= MIN_TIME_TIMA)
        {
          duty_cycle_timA = MIN_TIME_TIMA;
        }  
      }
      timeout++;

      HRTIM_CompareStructure.AutoDelayedMode    = HRTIM_AUTODELAYEDMODE_REGULAR;
      HRTIM_CompareStructure.AutoDelayedTimeout = duty_cycle_timA; 
      HRTIM_CompareStructure.CompareValue       = duty_cycle_timA; 
  
      if(HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                         HRTIM_COMPAREUNIT_1, &HRTIM_CompareStructure) != HAL_OK)
      {
        Error_Handler();
      }

      HAL_Delay(1);

      /* Enable next trigger on ADC during data process*/
      if(HAL_ADCEx_InjectedStart_IT(&AdcHandle) != HAL_OK)
      {
        Error_Handler();
      }

      /* Verify if timeout not reached OR VIN not in expected range */
      if ((timeout > 10000) || (vin < MIN_VOLTAGE) || (vin >  MAX_VOLTAGE))
      {
        BSP_LED_Off(LED5);        
        while (1)
        {
          /* Red LED is set to alert on Buck boost function failure */
          BSP_LED_On(LED3);
          HAL_Delay(100);
          BSP_LED_Off(LED3);
          HAL_Delay(100);
        }
      }     
    }

    while(Keypressed == FALSE)
    {
      /* Boost mode successful */
      /* Blue LED is toggling */
      BSP_LED_On(LED6);
      HAL_Delay(100);
      BSP_LED_Off(LED6);
      HAL_Delay(100);
    }
    Keypressed = FALSE;
    
    /* Set Buck Mode and Vout target value to 2V  */
    vout_target = 2000;
    Current_Mode = BUCK;
    Converter_Mode_Change = TRUE;
    HAL_Delay(10); 
    timeout = 0;
    
    BSP_LED_On(LED6);
    BSP_LED_Off(LED5);
    
    while((vout < (vout_target - 5)) || (vout > (vout_target + 5)))
    {
      /* VIN and VOUT ADC sampling */
      Adc_Channel1_ConvertedValue_In = (Adc_Channel1_ConvertedValue_In * refint_cal) / AdcReferenceConvertedValue;
      Adc_Channel1_ConvertedValue_Out = (Adc_Channel1_ConvertedValue_Out * refint_cal) / AdcReferenceConvertedValue;
      
      vin = (Adc_Channel1_ConvertedValue_In * 3300) / 0xFFF;
      vout = (Adc_Channel1_ConvertedValue_Out * 3300) / 0xFFF;
      vin = (497 * vin )/100;
      vout = (503 * vout)/100;  

      /* Control Timer A Duty Cycle to reach the VOUT target */       
      if(vout <= vout_target)
      {
        duty_cycle_timA += 10;
        if (duty_cycle_timA >= (SET_PERIOD - MIN_TIME_TIMA))
        {
          duty_cycle_timA = (SET_PERIOD - MIN_TIME_TIMA);
        }
      }
      else
      {
        duty_cycle_timA -= 10;
        if (duty_cycle_timA <= MIN_TIME_TIMA)
        {
          duty_cycle_timA = MIN_TIME_TIMA;
        }  
      }
      timeout++;

      HRTIM_CompareStructure.AutoDelayedMode    = HRTIM_AUTODELAYEDMODE_REGULAR;
      HRTIM_CompareStructure.AutoDelayedTimeout = duty_cycle_timA; 
      HRTIM_CompareStructure.CompareValue       = duty_cycle_timA; 
  
      if(HAL_HRTIM_WaveformCompareConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                         HRTIM_COMPAREUNIT_1, &HRTIM_CompareStructure) != HAL_OK)
      {
        Error_Handler();
      }
      
      HAL_Delay(1);

      /* Enable next trigger on ADC during data process*/
      if(HAL_ADCEx_InjectedStart_IT(&AdcHandle) != HAL_OK)
      {
        Error_Handler();
      }

      /* Verify if timeout not reached OR VIN not in expected range */      
      if ((timeout > 10000) || (vin < MIN_VOLTAGE) || (vin >  MAX_VOLTAGE))
      {
        BSP_LED_Off(LED6);     
        while (1)
        {
          /* Red LED is set to alert on Buck boost function failure */
          BSP_LED_On(LED3);
          HAL_Delay(100);
          BSP_LED_Off(LED3);
          HAL_Delay(100);
        }
      } 
    }
    
    while(Keypressed == FALSE)
    {
      /* Buck mode successful */
      /* Green LED is toggling */
      BSP_LED_On(LED5);
      HAL_Delay(100);
      BSP_LED_Off(LED5);
      HAL_Delay(100);
    } 

    /* Test complete */
    while (1)
    {
      /* All LEDs are toggling : TEST SUCCESSFUL */
      BSP_LED_On(LED3);  
      BSP_LED_On(LED4);
      BSP_LED_On(LED5);          
      BSP_LED_On(LED6);
      HAL_Delay(150);
      BSP_LED_Off(LED3);
      BSP_LED_Off(LED4);
      BSP_LED_Off(LED5);
      BSP_LED_Off(LED6);
      HAL_Delay(150);
    }
  }  
}


/**
* @brief  ADC configuration
* @param  None
* @retval None
*/
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef     ChannelConfig;
  ADC_InjectionConfTypeDef   InjectedConfig;
  ADC_MultiModeTypeDef       MultiModeConfig;
  GPIO_InitTypeDef           GPIO_InitStructure;
  HRTIM_ADCTriggerCfgTypeDef ADCTriggerConfig;
  
  /* ADC configuration */
  /* ADC is first set in single channel to record VREF */
  /* then turned into injected mode to monitor VIN and VOUT voltages */
  
  /* Configure the ADC clock */
  __HAL_RCC_ADC12_CONFIG(RCC_ADC12PLLCLK_DIV2);
  
  /* Enable ADC1 clock */
  __HAL_RCC_ADC12_CLK_ENABLE();
  
  /* ADC Channel configuration */
  /* Configure ADC Channel2 as analog input / PA1/VIN */
  GPIO_InitStructure.Pin  =  GPIO_PIN_1;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure ADC Channel4 as analog input / PA3/VOUT */
  GPIO_InitStructure.Pin  =  GPIO_PIN_3;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Insert delay equal to 100 탎 */
  HAL_Delay(1);
  
  AdcHandle.Instance = ADC1;

  /* Start single channel voltage reference measurement */
  MultiModeConfig.DMAAccessMode    = ADC_DMAACCESSMODE_DISABLED;
  MultiModeConfig.Mode             = ADC_MODE_INDEPENDENT;
  MultiModeConfig.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_1CYCLE;

  if(HAL_ADCEx_MultiModeConfigChannel(&AdcHandle, &MultiModeConfig) != HAL_OK)
  {
    Error_Handler();
  }

  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4; /* AHB clock / 4 = 16MHz : 12-bit resolution */
  AdcHandle.Init.ContinuousConvMode    = DISABLE;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGINJECCONV_EDGE_NONE;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
  AdcHandle.Init.NbrOfConversion       = 1;
  AdcHandle.Init.NbrOfDiscConversion   = 1;
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_PRESERVED;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.ScanConvMode          = DISABLE;
  
  if(HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Calibration procedure */ 
  if(HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* 61.5 clock cycles @16MHz = 3.84us */
  ChannelConfig.Channel      = ADC_CHANNEL_18;
  ChannelConfig.Offset       = 0;
  ChannelConfig.OffsetNumber = ADC_OFFSET_NONE;
  ChannelConfig.Rank         = ADC_REGULAR_RANK_1;
  ChannelConfig.SamplingTime = ADC_SAMPLETIME_61CYCLES_5;
  ChannelConfig.SingleDiff   = ADC_SINGLE_ENDED;

  if(HAL_ADC_ConfigChannel(&AdcHandle, &ChannelConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  HAL_Delay(1);
  
  /* Start ADC1 Software Conversion */ 
  if(HAL_ADC_Start(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  if(HAL_ADC_PollForConversion(&AdcHandle, HAL_MAX_DELAY) != HAL_OK)
  {
    Error_Handler();
  }

  /* Get ADC converted data */
  AdcReferenceConvertedValue = HAL_ADC_GetValue(&AdcHandle);
  
  /* check and wait ADC is disabled */
  if(HAL_ADC_Stop(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_Delay(1);
  
  /* Switch to ADC injected mode after VREFINT calibration */
  MultiModeConfig.DMAAccessMode    = ADC_DMAACCESSMODE_DISABLED;
  MultiModeConfig.Mode             = ADC_DUALMODE_INJECSIMULT;
  MultiModeConfig.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_1CYCLE;

  if(HAL_ADCEx_MultiModeConfigChannel(&AdcHandle, &MultiModeConfig) != HAL_OK)
  {
    Error_Handler();
  }

  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
  AdcHandle.Init.ContinuousConvMode    = DISABLE;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  AdcHandle.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
  AdcHandle.Init.NbrOfConversion       = 1;
  AdcHandle.Init.NbrOfDiscConversion   = 1;
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.ScanConvMode          = ENABLE;
  
  if(HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* ADC1 channel1 configuration */ 
  /* Example VIN Tconv = Tsamp (7.5) + 12.5 ADC clock cycles = 20 ADC clock samples = 278ns @72MHz */
  InjectedConfig.AutoInjectedConv              = DISABLE;
  InjectedConfig.ExternalTrigInjecConv         = ADC_EXTERNALTRIGINJECCONV_HRTIM_TRG2;
  InjectedConfig.ExternalTrigInjecConvEdge     = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
  InjectedConfig.InjectedChannel               = ADC_CHANNEL_2; /* corresponds to PA1 VIN */
  InjectedConfig.InjectedDiscontinuousConvMode = ENABLE;
  InjectedConfig.InjectedNbrOfConversion       = 2;
  InjectedConfig.InjectedOffset                = 0;
  InjectedConfig.InjectedOffsetNumber          = ADC_OFFSET_NONE;
  InjectedConfig.InjectedRank                  = ADC_INJECTED_RANK_1;
  InjectedConfig.InjectedSamplingTime          = ADC_SAMPLETIME_7CYCLES_5;
  InjectedConfig.InjectedSingleDiff            = ADC_SINGLE_ENDED;
  InjectedConfig.QueueInjectedContext          = DISABLE;
  
  if(HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectedConfig) != HAL_OK)
  {
    Error_Handler();
  }

  InjectedConfig.InjectedChannel               = ADC_CHANNEL_4; /* corresponds to PA3 VOUT */
  InjectedConfig.InjectedRank                  = ADC_INJECTED_RANK_2;
  InjectedConfig.InjectedSamplingTime          = ADC_SAMPLETIME_19CYCLES_5;

  if(HAL_ADCEx_InjectedConfigChannel(&AdcHandle, &InjectedConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure and enable ADC1 interrupt */
  HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
  
  /*HRTIM trigger configuration for ADC1 */
  ADCTriggerConfig.Trigger      = HRTIM_ADCTRIGGEREVENT24_TIMERB_CMP2;
  ADCTriggerConfig.UpdateSource = HRTIM_ADCTRIGGERUPDATE_TIMER_B;

  if(HAL_HRTIM_ADCTriggerConfig(&HrtimHandle, HRTIM_ADCTRIGGER_2, &ADCTriggerConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start ADC1 Software Conversion */ 
  if(HAL_ADCEx_InjectedStart_IT(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
* @brief  Configure HRTIM for Buck Mode configuration
* @param  None
* @retval None
*/
static void SetHRTIM_BuckMode(void)
{
  HRTIM_OutputCfgTypeDef HRTIM_OutputStructure;
  
  HRTIM_Unselect_OutputTIMx();
  
  HRTIM_OutputStructure.Polarity              = HRTIM_OUTPUTPOLARITY_HIGH; 
  HRTIM_OutputStructure.IdleMode              = HRTIM_OUTPUTIDLEMODE_IDLE;  
  HRTIM_OutputStructure.IdleLevel             = HRTIM_OUTPUTIDLELEVEL_INACTIVE;          
  HRTIM_OutputStructure.FaultLevel            = HRTIM_OUTPUTFAULTLEVEL_NONE;          
  HRTIM_OutputStructure.ChopperModeEnable     = HRTIM_OUTPUTCHOPPERMODE_DISABLED;        
  HRTIM_OutputStructure.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;

  /* Set TIMA A1(PA8) and A2(PA9) opposite PWM ouputs */
  HRTIM_OutputStructure.SetSource   = HRTIM_OUTPUTSET_TIMPER;  
  HRTIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_TIMCMP1; 

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                    HRTIM_OUTPUT_TA1, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }

  /* Reset Output B1(PA10) to open T12 NMOS and set B2 (PA11) to close T5 PMOS bridge, operated as I/O */
  HRTIM_OutputStructure.SetSource   = HRTIM_OUTPUTSET_NONE;  
  HRTIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_TIMPER; 

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, 
                                    HRTIM_OUTPUT_TB1, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }
  
  HRTIM_OutputStructure.SetSource   = HRTIM_OUTPUTSET_TIMPER;  
  HRTIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_NONE; 

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, 
                                    HRTIM_OUTPUT_TB2, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }

  /* Start both TIMA and TIMB */
  if(HAL_HRTIM_WaveformCountStart(&HrtimHandle, HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B) != HAL_OK)
  {
    Error_Handler();
  }

  Converter_Mode_Change = FALSE;
}

/**
* @brief  Configure HRTIM for Boost Mode configuration
* @param  None
* @retval None
*/
void SetHRTIM_BoostMode(void)
{
  HRTIM_OutputCfgTypeDef HRTIM_OutputStructure;

  HRTIM_Unselect_OutputTIMx();

  HRTIM_OutputStructure.Polarity              = HRTIM_OUTPUTPOLARITY_HIGH; 
  HRTIM_OutputStructure.IdleMode              = HRTIM_OUTPUTIDLEMODE_IDLE;  
  HRTIM_OutputStructure.IdleLevel             = HRTIM_OUTPUTIDLELEVEL_INACTIVE;          
  HRTIM_OutputStructure.FaultLevel            = HRTIM_OUTPUTFAULTLEVEL_NONE;          
  HRTIM_OutputStructure.ChopperModeEnable     = HRTIM_OUTPUTCHOPPERMODE_DISABLED;        
  HRTIM_OutputStructure.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;

  /* Set TIMA B1(PA10) and B2(PA11) opposite PWM ouputs */
  HRTIM_OutputStructure.SetSource   = HRTIM_OUTPUTSET_TIMPER;  
  HRTIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_TIMEV_1; 

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, 
                                    HRTIM_OUTPUT_TB1, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }
  
  /* Set Output A1(PA8) to open T4 PMOS and reset A2(PA9) to close T5 NMOS bridge, operated as I/O */
  HRTIM_OutputStructure.SetSource   = HRTIM_OUTPUTSET_NONE;  
  HRTIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_TIMPER; 

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                    HRTIM_OUTPUT_TA2, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }
  
  HRTIM_OutputStructure.SetSource   = HRTIM_OUTPUTSET_TIMPER;  
  HRTIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_NONE; 

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                    HRTIM_OUTPUT_TA1, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }

  /* Start both TIMA and TIMB */
  if(HAL_HRTIM_WaveformCountStart(&HrtimHandle, HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B) != HAL_OK)
  {
    Error_Handler();
  }

  Converter_Mode_Change = FALSE;
}

/**
* @brief  Reset ALL HRTIM Setx and Rstx registers Output Disable 
* @param  None
* @retval None
*/
static void HRTIM_Unselect_OutputTIMx(void)
{  
  HRTIM_OutputCfgTypeDef HRTIM_OutputStructure;

  HRTIM_OutputStructure.Polarity              = HRTIM_OUTPUTPOLARITY_HIGH; 
  HRTIM_OutputStructure.IdleMode              = HRTIM_OUTPUTIDLEMODE_IDLE;  
  HRTIM_OutputStructure.IdleLevel             = HRTIM_OUTPUTIDLELEVEL_INACTIVE;          
  HRTIM_OutputStructure.FaultLevel            = HRTIM_OUTPUTFAULTLEVEL_NONE;          
  HRTIM_OutputStructure.ChopperModeEnable     = HRTIM_OUTPUTCHOPPERMODE_DISABLED;        
  HRTIM_OutputStructure.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
  HRTIM_OutputStructure.SetSource             = HRTIM_OUTPUTSET_NONE;  
  HRTIM_OutputStructure.ResetSource           = HRTIM_OUTPUTRESET_NONE; 

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                    HRTIM_OUTPUT_TA1, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_A, 
                                    HRTIM_OUTPUT_TA2, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, 
                                    HRTIM_OUTPUT_TB1, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }

  if(HAL_HRTIM_WaveformOutputConfig(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, 
                                    HRTIM_OUTPUT_TB2, &HRTIM_OutputStructure) != HAL_OK)  
  {
    Error_Handler();
  }
}

/**
  * @brief  Injected conversion complete callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if( __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_JEOS))
  {   
    __HAL_HRTIM_TIMER_CLEAR_IT(&HrtimHandle, HRTIM_TIMERINDEX_TIMER_B, HRTIM_TIM_IT_CMP2);

    /* Get ADC converted data */
    Adc_Channel1_ConvertedValue_In = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
    Adc_Channel1_ConvertedValue_Out = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_2);
  }

  /* Disable next trigger on ADC during data process*/
  HAL_ADCEx_InjectedStop_IT(hadc);
}

/**
  * @brief  EXTI line detection callbacks.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint32_t i=0;

  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  Keypressed = TRUE;

  if(TestMode == FALSE)
  {
    NoWait = TRUE;

    /* Change state machine mode */
    StateMachine++;

    /* Wait until USER button released */
    while((BSP_PB_GetState(BUTTON_USER) == SET))
    {
      i++;
      if(i == 1000000)
      {
        StateMachine = STATE_MAN;
        DownUp = (!DownUp);
        break;
      }
    }

    /* Reinitialize thresholds */
    CurrentSenseTab[0] = 280;
    CurrentSenseTab[1] = 240;
    CurrentSenseTab[2] = 200;
    CurrentSenseTab[3] = 360;
    CurrentSenseTab[4] = 320;

    /* Convert current sense value thresholds for DAC */
    for (i = 0; i < 5; i++)
    {
      CurrentSenseTab[i] = (CurrentSenseTab[i] * 4096) / 3300; 
    }
    
    if (StateMachine >= MAX_STATE)
    {
      StateMachine = STATE_OFF;
    }
  }
}

/**
  * @brief  Callback function invoked when timer repetition period has elapsed
  * @param  hhrtim: pointer to HAL HRTIM handle  
  * @param  TimerIdx: Timer index
  *                   This parameter can be one of the following values:
  *                   @arg HRTIM_TIMERINDEX_MASTER  for master timer
  *                   @arg HRTIM_TIMERINDEX_TIMER_A for timer A
  *                   @arg HRTIM_TIMERINDEX_TIMER_B for timer B
  *                   @arg HRTIM_TIMERINDEX_TIMER_C for timer C
  *                   @arg HRTIM_TIMERINDEX_TIMER_D for timer D
  *                   @arg HRTIM_TIMERINDEX_TIMER_E for timer E
  * @retval None
  */
void HAL_HRTIM_RepetitionEventCallback(HRTIM_HandleTypeDef * hhrtim,
                                              uint32_t TimerIdx)
{
  if (TimerIdx == HRTIM_TIMERINDEX_TIMER_A)
  {
    /* Converter mode is changed during interrupt to synchronize HRTIM output changes */
    if(Converter_Mode_Change == TRUE)
    {
      switch(Current_Mode)
      {
      case BUCK:
        SetHRTIM_BuckMode();
        break;
      
      case BOOST:
        SetHRTIM_BoostMode();
        break;
        
      default:
        Error_Handler();
      }
    }
  }
}

/**
  * @brief  Callback function invoked when the end of the burst mode period is reached
  * @param  hhrtim: pointer to HAL HRTIM handle  
  * @retval None
  */
void HAL_HRTIM_BurstModePeriodCallback(HRTIM_HandleTypeDef * hhrtim)
{
  static uint8_t index1 =0;
  uint32_t i_intensity = 0;
  float index2;

  /* Each Master Burst mode period interrupt, the burst compare is updated */
  i_intensity = (uint32_t) fIntensity;
  index2 = fIntensity - i_intensity;
  index2 *= 8;
  
  if (DitherTab[(uint32_t)index2][index1] == 0)
  { 
    /* If Dither table value is zero then standard drive ...*/
    HRTIM_SetBurstCompare(i_intensity);
  }
  else
  {
    /* or extra period is added */ 
    HRTIM_SetBurstCompare(i_intensity + 1);
  }

  /* Each interrupt the table data are rolling from 1 to 7 */
  index1++;
  if (index1 > 7)
  {
    index1 = 0;
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
  while(1)
  {
    /* All LEDs are ON */
    BSP_LED_On(LED3);  
    BSP_LED_On(LED4);
    BSP_LED_On(LED5);          
    BSP_LED_On(LED6);
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
