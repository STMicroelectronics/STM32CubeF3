/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_Snippets/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to generate basic PWM waveforms with the 
  * HRTIM, as per HRTIM Cookbook basic examples.
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

/** @addtogroup HRTIM_Snippets
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
#define HRTIM_INPUT_CLOCK       ((uint64_t)144000000)   /* Value in Hz */

/* Formula below works down to 70.3kHz (with presc ratio = 1) */ 
#define _200KHz_PERIOD ((uint16_t)((HRTIM_INPUT_CLOCK * 32) / 200000))

/* Formula below works down to 70.3kHz (with presc ratio = 1) */ 
#define _100KHz_PERIOD ((uint16_t)((HRTIM_INPUT_CLOCK * 32) / 100000))

/* Formula below works down to 70.3kHz (with presc ratio = 1) */ 
#define _100KHz_Plus_PERIOD ((uint16_t)((HRTIM_INPUT_CLOCK * 32) / 100001))

/* Formula below works down to 17.6kHz (with presc ratio = 4) */ 
#define _33KHz_PERIOD ((uint16_t)((HRTIM_INPUT_CLOCK * 8) / 33333))

/* Define here the snippet to be executed */
#define HRTIM_CHECK
//#define SINGLE_PWM
//#define MULTIPLE_PWM
//#define PWM_MASTER
//#define ARBITRARY_WAVEFORM

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

static void HRTIM_Minimal_Config(void);

static void GPIO_HRTIM_outputs_Config(void);
  
/* Private functions ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32F3xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to have a system clock = 72 MHz */
  SystemClock_Config();

  /* Initialize STM32F3348-DISCO LEDs */
  BSP_LED_Init(LED3);   /* Indicates Error */
  BSP_LED_Init(LED5);   /* Indicates MCU is active */

  HRTIM_Minimal_Config();       /* Initialize the HRTIM clock and DLL */

/* ---------------------------------------------------------------------------*/
/* ------------------------ SNIPPET 1: HRTIM_CHECK ---------------------------*/
/* ---------------------------------------------------------------------------*/
#ifdef HRTIM_CHECK

  HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TD1OEN; /* Enable TD1 output */

  GPIO_HRTIM_outputs_Config();  /* Initialize HRTIM outputs */

  BSP_LED_On(LED5);
  
  while(1)
  {
    /* Set and reset TD1 by software */
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx1R = HRTIM_SET1R_SST;
    HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx1R = HRTIM_RST1R_SRT;
    
  }

#endif /* HRTIM_CHECK */

/* ---------------------------------------------------------------------------*/
/* ------------------------ SNIPPET 2: SINGLE PWM ----------------------------*/
/* ---------------------------------------------------------------------------*/
#ifdef SINGLE_PWM
  
  /* TIMD counter operating in continuous mode */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].TIMxCR = HRTIM_TIMCR_CONT;

  /* Set period to 100kHz and duty cycle (CMP1) to 50% */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].PERxR = _100KHz_PERIOD;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP1xR = _100KHz_PERIOD/2;

  /* TD1 output set on TIMD period and reset on TIMD CMP1 event*/
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx1R = HRTIM_SET1R_PER;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx1R = HRTIM_RST1R_CMP1;
 
  HRTIM1->sMasterRegs.MCR = HRTIM_MCR_TDCEN;    /* Start Timer D */
  HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TD1OEN; /* Enable TD1 output */

  GPIO_HRTIM_outputs_Config();  /* Initialize HRTIM GPIO outputs */

  while(1)
  {
    BSP_LED_Toggle(LED5);
    HAL_Delay(100);
  }

#endif /* SINGLE_PWM */
  
/* ---------------------------------------------------------------------------*/
/* ------------------------ SNIPPET 3: MULTIPLE PWM --------------------------*/
/* ---------------------------------------------------------------------------*/
#ifdef MULTIPLE_PWM
  
  /* --------------------- Timer D initialization --------------------------- */
  /* TIMD counter operating in continuous mode, preload enabled on REP event */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].TIMxCR = HRTIM_TIMCR_CONT
                                                       + HRTIM_TIMCR_PREEN
                                                       + HRTIM_TIMCR_TREPU;

  /* Set period to 100kHz, CMP1 to 25% and CMP2 to 75% of period */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].PERxR = _100KHz_PERIOD;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP1xR = _100KHz_PERIOD/4;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP2xR = (3*_100KHz_PERIOD)/4;

  /* TD1 output set on TIMD period and reset on TIMD CMP1 event*/
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx1R = HRTIM_SET1R_PER;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx1R = HRTIM_RST1R_CMP1;
 
  /* TD2 output set on TIMD CMP2 and reset on TIMD period event*/
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx2R = HRTIM_SET2R_CMP2;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx2R = HRTIM_RST2R_PER;

  /* --------------------- Timer A initialization --------------------------- */
  /* TIMA counter operating in continuous mode with prescaler = 010b (div. by 4) */
  /* Preload enabled on REP event*/
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].TIMxCR = HRTIM_TIMCR_CONT
                                                       + HRTIM_TIMCR_PREEN
                                                       + HRTIM_TIMCR_TREPU
                                                       + HRTIM_TIMCR_CK_PSC_1;
  /* Set period to 33kHz and duty cycles to 25% */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].PERxR = _33KHz_PERIOD;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = _33KHz_PERIOD/4;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP2xR = _33KHz_PERIOD/2;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP3xR = (3*_33KHz_PERIOD)/4;

  /* TA1 output set on TIMA period and reset on TIMA CMP1 event*/
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].SETx1R = HRTIM_SET1R_PER;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].RSTx1R = HRTIM_RST1R_CMP1;

  /* TA2 output set on TIMA CMP2 and reset on TIMA CMP3 event*/
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].SETx2R = HRTIM_SET2R_CMP2;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].RSTx2R = HRTIM_RST2R_CMP3;

  /* Enable TA1, TA2, TD1 and TD2 outputs */
  HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TA1OEN + HRTIM_OENR_TA2OEN + HRTIM_OENR_TD1OEN + HRTIM_OENR_TD2OEN; 

  GPIO_HRTIM_outputs_Config();  /* Initialize HRTIM GPIO outputs */

  /* Start Timer A and Timer D */
  HRTIM1->sMasterRegs.MCR = HRTIM_MCR_TACEN + HRTIM_MCR_TDCEN;

  while(1)
  {
    BSP_LED_Toggle(LED5);
    HAL_Delay(100);
  }

#endif /* MULTIPLE_PWM */

/* ---------------------------------------------------------------------------*/
/* -------------------- SNIPPET 4: PWM with MASTER TIMER ---------------------*/
/* ---------------------------------------------------------------------------*/
#ifdef PWM_MASTER
  
  /* --------------------- Master Timer initialization ---------------------- */
  /* Master counter operating in continuous mode, Preload enabled on REP event*/
  HRTIM1->sMasterRegs.MCR = HRTIM_MCR_CONT + HRTIM_MCR_PREEN + HRTIM_MCR_MREPU;
  /* Set period to 100kHz+ and duty cycle to 50% */
  /* The frequency difference is very low to show that the 2 signals are not synchronous*/
  HRTIM1->sMasterRegs.MPER = _100KHz_Plus_PERIOD;
  HRTIM1->sMasterRegs.MCMP1R = _100KHz_Plus_PERIOD/2;

  /* --------------------- Timer D initialization --------------------------- */
  /* TIMD counter operating in continuous mode, preload enabled on REP event */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].TIMxCR = HRTIM_TIMCR_CONT
                                                       + HRTIM_TIMCR_PREEN
                                                       + HRTIM_TIMCR_TREPU;

  /* Set period to 100kHz and duty cycle (CMP1) to 25% */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].PERxR = _100KHz_PERIOD;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP1xR = _100KHz_PERIOD/4;

  /* TD1 output set on TIMD period and reset on TIMD CMP1 event*/
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx1R = HRTIM_SET1R_PER;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx1R = HRTIM_RST1R_CMP1;
 
  /* TD2 output set on TIMD CMP2 and reset on TIMD period event*/
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx2R = HRTIM_SET2R_MSTPER;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx2R = HRTIM_RST2R_MSTCMP1;

  /* Enable TD1 and TD2 outputs */
  HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TD1OEN + HRTIM_OENR_TD2OEN; 
  GPIO_HRTIM_outputs_Config();  /* Initialize HRTIM GPIO outputs */

  /* Start Master Timer and Timer D */
  HRTIM1->sMasterRegs.MCR |= HRTIM_MCR_MCEN + HRTIM_MCR_TDCEN;

  while(1)
  {
    BSP_LED_Toggle(LED5);
    HAL_Delay(100);
  }

#endif /* MULTIPLE_PWM */

/* ---------------------------------------------------------------------------*/
/* --------------------- SNIPPET 5: ARBITRARY_WAVEFORM -----------------------*/
/* ---------------------------------------------------------------------------*/
#ifdef ARBITRARY_WAVEFORM
  
  /* --------------------- Timer D initialization --------------------------- */
  /* TIMD counter operating in continuous mode, preload enabled on REP event */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].TIMxCR = HRTIM_TIMCR_CONT
                                                       + HRTIM_TIMCR_PREEN
                                                       + HRTIM_TIMCR_TREPU;

  /* Set period to 100kHz and edge timings */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].PERxR = _100KHz_PERIOD;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP1xR = _100KHz_PERIOD/4;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP2xR = (3*_100KHz_PERIOD)/8;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP3xR = _100KHz_PERIOD/2;

  /* TD1 toggles on TIMD period, CMP1 and CMP2 event */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx1R = HRTIM_SET1R_PER + HRTIM_SET1R_CMP1 + HRTIM_SET1R_CMP2;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx1R = HRTIM_RST1R_PER + HRTIM_RST1R_CMP1 + HRTIM_RST1R_CMP2;
 
  /* TD2 output set on TIMD PER and CMP2 and reset on TIMD CMP1 and CMP3 event */
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].SETx2R = HRTIM_SET2R_PER + HRTIM_SET2R_CMP2;
  HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].RSTx2R = HRTIM_RST2R_CMP1 + HRTIM_RST2R_CMP3;

  /* Enable TD1 and TD2 outputs */
  HRTIM1->sCommonRegs.OENR = HRTIM_OENR_TD1OEN + HRTIM_OENR_TD2OEN; 
  GPIO_HRTIM_outputs_Config();  /* Initialize HRTIM GPIO outputs */

  /* Start Timer D */
  HRTIM1->sMasterRegs.MCR = HRTIM_MCR_TDCEN;

  while(1)
  {
    BSP_LED_Toggle(LED5);
    HAL_Delay(100);
  }

#endif /* ARBITRARY_WAVEFORM */

}


/**
* @brief  Configure GPIO outputs for the HRTIM
* @param  None
* @retval None
*/
static void GPIO_HRTIM_outputs_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable GPIOA clock for timer A outputs */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure HRTIM output: TA1 (PA8) and TA2 (PA9)*/
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9; 
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;;  
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;;  
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Enable GPIOB clock for timer D outputs */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure HRTIM output: TD1 (PB14) and TD2 (PB15)*/
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15; 
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;;  
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;;  
  GPIO_InitStruct.Alternate = GPIO_AF13_HRTIM1;
  
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/**
* @brief  HRTIM minimal configuration to have snippets running
*         HRTIM is clocked and DLL is started
* @param  None
* @retval None
*/
static void HRTIM_Minimal_Config(void)
{

  /* Use the PLLx2 clock for HRTIM */
  __HAL_RCC_HRTIM1_CONFIG(RCC_HRTIM1CLK_PLLCLK);

  /* Enable HRTIM clock*/
  __HAL_RCC_HRTIM1_CLK_ENABLE();

  /* DLL calibration: periodic calibration enabled, period set to 14µs */
  HRTIM1->sCommonRegs.DLLCR = HRTIM_CALIBRATIONRATE_14| HRTIM_DLLCR_CALEN;
    
  /* Check DLL end of calibration flag */
  while((HRTIM1->sCommonRegs.ISR & HRTIM_IT_DLLRDY) == RESET);

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
  /* Turn LED3 (RED) on */
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
