/**
  ******************************************************************************
  * @file    I2S/I2S_Audio/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
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

/** @addtogroup I2S_Audio
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   "    STM32F373xx     "
#define MESSAGE2   " Device running on  " 
#define MESSAGE3   "   STM32373C-EVAL   "


extern const uint16_t AUDIO_SAMPLE[];

/* Audio file size and start address are defined here since the Audio file is 
   stored in Flash memory as a constant table of 16-bit data */

#define AUDIO_START_OFFSET_ADDRESS    44     /* Offset relative to audio file header size */
#define AUDIO_FILE_SIZE       99112          /* Audio file size in 16-bit sample          */
#define AUDIO_FILE_ADDRESS    AUDIO_SAMPLE   /* Audio file address                        */  

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t uwCommand = AUDIO_PAUSE;
__IO uint32_t uwVolume = 70;

uint32_t AudioTotalSize = 0xFFFF; /* This variable holds the total size of the audio file */
uint32_t AudioRemSize   = 0xFFFF; /* This variable holds the remaining data in audio file */
uint16_t* CurrentPos;              /* This variable holds the current position address of audio data */

/* LCD display can't be used at the same time in background loop and thru an
   interruption handling (non re-entering API). Therefore variable uwInterruptIgnore
   is used to ignore interrupts handling processing when the LCD is being updated
   in the background loop. Basically, this means that pushing the joystick buttons
   at that time has no effect. */
uint32_t uwInterruptIgnore = 0;

/* Variable used to indicate when it is required to erase the additional information
   which is sometimes indicated on the LCD line 9. */
uint32_t uwEraseLCDLine9 = 0;

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
  
  /* Initialize LEDs, Key Button, LCD available on EVAL board *****************/
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);


  /* Initialize the Key Push button.
     Key button is used for Pause/Resume,
     checked in polling mode.
   */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO); 
    
  /* Initialize the LCD */
  BSP_LCD_Init();
  
  /* Display message on EVAL LCD **********************************************/
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_BLUE);  

  /* Set the LCD Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAtLine(0, (uint8_t *)MESSAGE1);
  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)MESSAGE2);
  BSP_LCD_DisplayStringAtLine(2, (uint8_t *)MESSAGE3);

  /* Turn on LEDs available on EVAL *******************************************/
  BSP_LED_On(LED1);
  BSP_LED_On(LED2);
  BSP_LED_On(LED3);
  BSP_LED_On(LED4);

  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOExpander, IOs...) */  
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, uwVolume, I2S_AUDIOFREQ_8K) == 0)
  {
    BSP_LCD_DisplayStringAtLine(3, (uint8_t *)"====================");
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"Key   : Play/Pause ");
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"Up    : Vol+       ");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"Down  : Vol-       ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"====================");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"  AUDIO CODEC   OK  ");    
  }
  else
  {
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"  AUDIO CODEC  FAIL ");
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *)" Try to reset board ");
  }
  


  
  /* 
  Normal mode description:
      Start playing the audio file (using DMA channel).
      Using this mode, the application can run other tasks in parallel since 
      the DMA is handling the Audio Transfer instead of the CPU.
      The only task remaining for the CPU will be the management of the DMA 
      Transfer Complete interrupt or the Half Transfer Complete interrupt in 
      order to load again the buffer and to calculate the remaining data.  
  Circular mode description:
     Start playing the file from a circular buffer, once the DMA is enabled it 
     always run. User has to fill periodically the buffer with the audio data 
     using Transfer complete and/or half transfer complete interrupts callbacks 
     (EVAL_AUDIO_TransferComplete_CallBack() or EVAL_AUDIO_HalfTransfer_CallBack()...
     In this case the audio data file is smaller than the DMA max buffer 
     size 65535 so there is no need to load buffer continuously or manage the 
     transfer complete or Half transfer interrupts callbacks. */
     
  /* Set the total number of data to be played (count in half-word) */
  /* Mono channel here, in case of stereo, AudioTotalSize should have been
     divided by half */
  AudioTotalSize = (AUDIO_FILE_SIZE - AUDIO_START_OFFSET_ADDRESS);   
  
  /* Set the current audio pointer position */
  CurrentPos = (uint16_t*)(AUDIO_FILE_ADDRESS + AUDIO_START_OFFSET_ADDRESS);
  /* Update the remaining number of data to be played */
  AudioRemSize = AudioTotalSize - DMA_MAX(AudioTotalSize);  
  /* Start the audio player */
  BSP_AUDIO_OUT_Play((uint16_t*)CurrentPos, (uint32_t)(AUDIO_FILE_SIZE - AUDIO_START_OFFSET_ADDRESS)); 

  
  /* Display the state on the screen */
  BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"       PLAYING      ");
  
  
  /* Configure JOY STICK in interrupt mode
     Up Button for Volume High 
     Down Button for Volume Low      
   */
  BSP_JOY_Init(JOY_MODE_EXTI);
  
  
  /* Infinite loop */
  while (1)
  {
    
    /* Check on the Pause/Resume button */
    if(BSP_PB_GetState(BUTTON_KEY) == RESET)
    {
      /* Wait to avoid rebound */
      while (BSP_PB_GetState(BUTTON_KEY) == RESET);
      
      if(uwCommand == AUDIO_PAUSE)
      {
        BSP_AUDIO_OUT_Pause();
        /* Display the current state of the player */
        uwInterruptIgnore = 1;
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"       PAUSED       ");
        uwInterruptIgnore = 0;
        
        /* Next time Resume command should be processed */
        uwCommand = AUDIO_RESUME;
      }
      else
      {
        BSP_AUDIO_OUT_Resume();
        /* Display the current state of the player */
        uwInterruptIgnore = 1;
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"       PLAYING      ");
        uwInterruptIgnore = 0;
        /* Next time Pause command should be processed */
        uwCommand = AUDIO_PAUSE;
      }
    }
   
        
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);
    
    /* Insert 100 ms delay */
    HAL_Delay(100);
    
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
    
    /* Insert 100 ms delay */
    HAL_Delay(100);
    /* Erase volume change or player state information if required */
    if (uwEraseLCDLine9 == 1)
    {
       /* Insert 400 ms additional delay*/
      HAL_Delay(400);
      uwInterruptIgnore = 1;
      BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"                  ");
      uwInterruptIgnore = 0;
      uwEraseLCDLine9 = 0;
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

/*------------------------------------------------------------------------------
       Callbacks implementation:
           the callbacks API are defined __weak in the stm32303c_eval_audio.c file
           and their implementation should be done the user code if they are needed.
           Below some examples of callback implementations.
  ----------------------------------------------------------------------------*/
/**
  * @brief  Manages the full Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
  /* Calculate the remaining audio data in the file and the new size 
     for the DMA transfer. If the Audio files size is less than the DMA max 
     data transfer size, so there is no calculation to be done, just restart 
     from the beginning of the file ... */
  /* Check if the end of file has been reached */
  if(AudioRemSize > 0)
  { 
    /* Replay from the current position */
    BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)CurrentPos, DMA_MAX(AudioRemSize));
    
    /* Update the current pointer position */
    CurrentPos += DMA_MAX(AudioRemSize);        
    
    /* Update the remaining number of data to be played */
    AudioRemSize -= DMA_MAX(AudioRemSize);  
  }
  else
  {
    /* Set the current audio pointer position */
    CurrentPos = (uint16_t*)(AUDIO_FILE_ADDRESS + AUDIO_START_OFFSET_ADDRESS);
    /* Replay from the beginning */
    BSP_AUDIO_OUT_Play((uint16_t*)CurrentPos,  (uint32_t)(AUDIO_FILE_SIZE - AUDIO_START_OFFSET_ADDRESS));
    /* Update the remaining number of data to be played */
    AudioRemSize = AudioTotalSize - DMA_MAX(AudioTotalSize);  
    /* Update the current audio pointer position */
    CurrentPos += DMA_MAX(AudioTotalSize);
  }

}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{  
  /* Generally this interrupt routine is used to load the buffer when 
  a streaming scheme is used: When first Half buffer is already transferred load 
  the new data to the first half of buffer while DMA is transferring data from 
  the second half. And when Transfer complete occurs, load the second half of 
  the buffer while the DMA is transferring from the first half ... */
  /* 
    ...........
                   */
}

/**
  * @brief  Manages the DMA FIFO error event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
  /* Display message on the LCD screen */
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"     DMA  ERROR     ");
  
  /* Stop the program with an infinite loop */
  while (1)
  {}
  
  /* could also generate a system reset to recover from the error */
  /* .... */
}


/**
  * @brief  EXTI line detection callbacks.
  *         EXTI interruptions are triggered by volume change requests  
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* if LCD is not being updated ... */
  if (uwInterruptIgnore != 1)
  {
    if(HAL_GPIO_ReadPin(UP_JOY_GPIO_PORT, UP_JOY_PIN) != GPIO_PIN_RESET)
    {
      /* additional information will be indicated on LCD line 9,
        will be erased after some delay */
      uwEraseLCDLine9 = 1;

      /* Increase volume by 5% */
      if (uwVolume < 95)
        uwVolume += 5; 
      else
        uwVolume = 100; 
      
      /* Apply the new volume to the codec */
      BSP_AUDIO_OUT_SetVolume(uwVolume);
      BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"       VOL:   +     "); 
    }
    else if(HAL_GPIO_ReadPin(DOWN_JOY_GPIO_PORT, DOWN_JOY_PIN) != GPIO_PIN_RESET)
    {
        /* additional information will be indicated on LCD line 9,
        will be erased after some delay */
       uwEraseLCDLine9 = 1;

      /* Decrease volume by 5% */
      if (uwVolume > 5)
        uwVolume -= 5; 
      else
        uwVolume = 0; 
        
      /* Apply the new volume to the codec */
      BSP_AUDIO_OUT_SetVolume(uwVolume);
      BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"       VOL:   -     ");
    } 
  } /* if (uwInterruptIgnore != 1) */
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
