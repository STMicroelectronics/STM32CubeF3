/**
  ******************************************************************************
  * @file    Examples_LL/HRTIM/HRTIM_BuckBoost/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to control the STMicroelectronics STM32F3348-Discovery RevB
  *          kit's buck-boost converter with the HRTIM
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

/** @addtogroup STM32F3xx_LL_Examples
  * @{
  */

/** @addtogroup HRTIM_BuckBoost
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* BUCK cannot re-start until Vout capacitor is de-energized below this value */
#define DE_ENERGIZING_THRESHOLD         ((uint16_t)2500)

/* BOOST cannot be started if VIN value is above this threshold */
#define OVER_VOLTAGE_PROTECTION         ((uint16_t)5000)

/* This is the VDDA supply in mV */
#define VDDA                            ((uint16_t)3300)

/* Deadtime values for buck and boost stages */
#define DT_RISING       ((uint16_t)230) // 200ns
#define DT_FALLING      ((uint16_t)230) // 200ns

/* Definitions of ADC hardware constraints delays */
/* Timeout values for ADC calibration, enable settling time, disable        */
/* settling time.                                                           */
/* Values defined to be higher than worst cases: low clock frequency,       */
/* maximum prescalers.                                                      */
/* Example of profile very low frequency : ADC clock frequency 0.14MHz      */
/* prescaler 256, sampling time 601.5 ADC clock cycles, resolution 12 bits. */
/*  - ADC calibration time: On STM32F3 ADC, maximum delay is 112/fADC,      */
/*    resulting in a maximum delay of 800us                                 */
/*    (refer to device datasheet, parameter "tCAL")                         */
/*  - ADC enable time: maximum delay is 1us.                                */
/*    (refer to device datasheet, parameter "tSTAB")                        */
/*  - ADC disable time: maximum delay should be a few ADC clock cycles      */
/*  - ADC stop conversion time: maximum delay should be a few ADC clock     */
/*    cycles                                                                */
/*  - ADC conversion time: with this hypothesis of clock settings, maximum  */
/*    delay will be 1123ms.                                                 */
/*    (refer to device reference manual, section "Timing")                  */
/* Unit: ms                                                                 */
#define ADC_CALIBRATION_TIMEOUT_MS       ((uint32_t)   1)
#define ADC_ENABLE_TIMEOUT_MS            ((uint32_t)   1)
#define ADC_DISABLE_TIMEOUT_MS           ((uint32_t)   1)
#define ADC_STOP_CONVERSION_TIMEOUT_MS   ((uint32_t)   1)
#define ADC_CONVERSION_TIMEOUT_MS        ((uint32_t)1150)


/* Delay between ADC end of calibration and ADC enable.                     */
/* Delay estimation in CPU cycles: Case of ADC enable done                  */
/* immediately after ADC calibration, ADC clock setting slow                */
/* (LL_ADC_CLOCK_ASYNC_DIV32). Use a higher delay if ratio                  */
/* (CPU clock / ADC clock) is above 32.                                     */
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)

#define HRTIM1_DLL_CALIBRATION_TIMEOUT_MS 10

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
__IO uint32_t Vin, Vout; /* volatile is here for debug convenience */

/* Exported variables --------------------------------------------------------*/
DemoState_t DemoState = DEMO_STATE_BUCK;

/* Private function prototypes -----------------------------------------------*/
static void UserButton_Init(void);
static uint32_t UserButton_GetState(void);

static void SystemClock_Config(void);

static void GPIO_BUCK_BOOST_Outputs_Config(void);
static void HRTIM_Config(void);
static void ADC_Config(void);
static void ADC_Activate(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 72 MHz */
  SystemClock_Config();

  /* Initialize LEDs */
  LED_Init(LED_RED);
  LED_Init(LED_BLUE);
  LED_Init(LED_ORANGE);
  LED_Init(LED_GREEN);

  /* Set LEDs Off */
  LED_Off(LED_RED);
  LED_Off(LED_BLUE);
  LED_Off(LED_ORANGE);
  LED_Off(LED_GREEN);

  /* Initialize User push-button in GPIO mode */
  UserButton_Init();
  
  /* Initialize ADC to be triggered by the HRTIMER */
  ADC_Config();
  ADC_Activate();
  
  /* Start ADC group regular conversion */
  if ((LL_ADC_IsEnabled(ADC1) == 1)               &&
      (LL_ADC_IsDisableOngoing(ADC1) == 0)        &&
      (LL_ADC_INJ_IsConversionOngoing(ADC1) == 0)   )
  {
    LL_ADC_INJ_StartConversion(ADC1);
  }
  
  /* Demo starts by de-energizing output capacitor */
  DemoState = DEMO_STATE_DE_ENERGIZE;
  
  /* Initialize HRTIM and related inputs */
  HRTIM_Config();

  /* Initialize BUCK-BOOST outputs (it has to be done after HRTIM init) */
  GPIO_BUCK_BOOST_Outputs_Config();
  
  /* HRTIM start-up */
  /* Enable HRTIM's outputs TA1, TA2, TB1 and TB2 */
  LL_HRTIM_EnableOutput(HRTIM1,
                        LL_HRTIM_OUTPUT_TA1 |
                        LL_HRTIM_OUTPUT_TA2 |
                        LL_HRTIM_OUTPUT_TB1 |
                        LL_HRTIM_OUTPUT_TB2);
  
  /* Start HRTIM's TIMER A and B */
  LL_HRTIM_TIM_CounterEnable(HRTIM1, LL_HRTIM_TIMER_A | LL_HRTIM_TIMER_B);

  /* Infinite loop */
  while (1)
  {
    /* ---------------- */
    /* Fault management */
    /* ---------------- */
    if (LL_HRTIM_IsActiveFlag_FLT1(HRTIM1))
    {
      
      DemoState = DEMO_STATE_FAULT;
    }
    
    /* -------------------------------------------------------------------------*/
    /* Input and output voltages measures (can be displayed in a watch window)  */
    /* -------------------------------------------------------------------------*/
    Vin = (LL_ADC_INJ_ReadConversionData32(ADC1, LL_ADC_INJ_RANK_1) * VDDA)/0xFFF;
    /* VIN bridge conversion is 4.97 (6.8K / 6.8K + 27K) */
    Vin = (497 * Vin )/100; /* Values in mV (for debug convenience) */
 
    Vout = (LL_ADC_INJ_ReadConversionData32(ADC1, LL_ADC_INJ_RANK_2) * VDDA)/0xFFF;
    /* VOUT bridge conversion is 5.03 (3.3K / 3.3K + 13.3K) */
    Vout = (503 * Vout)/100; /* Values in mV (for debug convenience) */

    /* ---------------- */
    /* Normal operation */
    /* ---------------- */
    switch(DemoState)
    {
    case DEMO_STATE_BUCK:
      /* Duty cycle is modified in the TIMA IRQ */
      /* BOOST cannot be started if VIN value is too high (Vout must not exceed 15V)*/
      if ((UserButton_GetState() == SET) && (Vin < OVER_VOLTAGE_PROTECTION))
      {
        DemoState = DEMO_STATE_BOOST;
        while(UserButton_GetState() == SET);
      }
      LED_Toggle(LED5);
      LED_Off(LED6);
      break;
 
    case DEMO_STATE_BOOST:
      /* Vout capacitor must be de-energized before switching to BUCK mode
      otherwise causing current to flow back into voltage source */
      if (UserButton_GetState() == SET)
      {
        DemoState = DEMO_STATE_DE_ENERGIZE;
        while(UserButton_GetState() == SET);
      }
      LED_Toggle(LED6);
      LED_Off(LED5);
      break;

    case DEMO_STATE_DE_ENERGIZE:
      /* Once Vout is below a safe level (below Vin value), BUCK operation can be started */
      if (Vout < DE_ENERGIZING_THRESHOLD)
      {
        DemoState = DEMO_STATE_BUCK;
      }
      LED_Off(LED6);
      LED_Off(LED3);
      break;

    case DEMO_STATE_FAULT:
      /* If fault is triggered, operation can be re-started with the push-button */
      if((UserButton_GetState() == SET))
      {
        LL_HRTIM_ClearFlag_FLT1(HRTIM1);
        LL_HRTIM_EnableOutput(HRTIM1,
                              LL_HRTIM_OUTPUT_TA1 |
                              LL_HRTIM_OUTPUT_TA2 |
                              LL_HRTIM_OUTPUT_TB1 |
                              LL_HRTIM_OUTPUT_TB2);
        DemoState = DEMO_STATE_DE_ENERGIZE;
        while(UserButton_GetState() == SET);
      }
      LED_Toggle(LED3);
      LED_Off(LED5);
      LED_Off(LED6);
      break;

    default:
        LL_HRTIM_DisableOutput(HRTIM1,
                               LL_HRTIM_OUTPUT_TA1 |
                               LL_HRTIM_OUTPUT_TA2 |
                               LL_HRTIM_OUTPUT_TB1 |
                               LL_HRTIM_OUTPUT_TB2);
      break;
    }

    /* This delay defines the main loop time and LED blinking rate*/
    LL_mDelay(50);
  }
}

/**
* @brief  Configure GPIO outputs for the BUCK-BOOST converter
* @param  None
* @retval None
*/
static void GPIO_BUCK_BOOST_Outputs_Config(void)
{
  /*## Configuration of GPIO used by HRTIM1_CHA1 #############################*/
  
  /* HRTIM1 Timer A - output 1 is mapped on GPIO pin PA8 */ 
  
  /* Enable GPIO Clock */
  HRTIM1_CHA1_GPIO_CLK_ENABLE();

  /* Configure PA8 as alternate function for HRTIM1_CHA1 */
  LL_GPIO_SetPinMode(HRTIM1_CHA1_GPIO_PORT, HRTIM1_CHA1_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(HRTIM1_CHA1_GPIO_PORT, HRTIM1_CHA1_PIN, LL_GPIO_PULL_NO);
  LL_GPIO_SetAFPin_8_15(HRTIM1_CHA1_GPIO_PORT, HRTIM1_CHA1_PIN, LL_GPIO_AF_13);


  /*## Configuration of GPIO used by HRTIM1_CHA2 #############################*/

  /* HRTIM1 Timer A - output 2 is mapped on GPIO pin PA9 */ 
  
  /* Enable GPIO Clock */
  HRTIM1_CHA2_GPIO_CLK_ENABLE();

  /* Configure PA9 as alternate function for HRTIM1_CHA2 */
  LL_GPIO_SetPinMode(HRTIM1_CHA2_GPIO_PORT, HRTIM1_CHA2_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(HRTIM1_CHA2_GPIO_PORT, HRTIM1_CHA2_PIN, LL_GPIO_PULL_NO);
  LL_GPIO_SetAFPin_8_15(HRTIM1_CHA2_GPIO_PORT, HRTIM1_CHA2_PIN, LL_GPIO_AF_13);


  /*## Configuration of GPIO used by HRTIM1_CHB1 #############################*/

  /* HRTIM1 Timer B - output 1 is mapped on GPIO pin PA10 */ 
  
  /* Enable GPIO Clock */
  HRTIM1_CHB1_GPIO_CLK_ENABLE();

  /* Configure PA10 as alternate function for HRTIM1_CHB1 */
  LL_GPIO_SetPinMode(HRTIM1_CHB1_GPIO_PORT, HRTIM1_CHB1_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(HRTIM1_CHB1_GPIO_PORT, HRTIM1_CHB1_PIN, LL_GPIO_PULL_NO);
  LL_GPIO_SetAFPin_8_15(HRTIM1_CHB1_GPIO_PORT, HRTIM1_CHB1_PIN, LL_GPIO_AF_13);


  /*## Configuration of GPIO used by HRTIM1_CHB2 #############################*/

  /* HRTIM1 Timer B - output 2 is mapped on GPIO pin PA11 */ 
  
  /* Enable GPIO Clock */
  HRTIM1_CHB2_GPIO_CLK_ENABLE();

  /* Configure PA11 as alternate function for HRTIM1_CHB2 */
  LL_GPIO_SetPinMode(HRTIM1_CHB2_GPIO_PORT, HRTIM1_CHB2_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(HRTIM1_CHB2_GPIO_PORT, HRTIM1_CHB2_PIN, LL_GPIO_PULL_NO);
  LL_GPIO_SetAFPin_8_15(HRTIM1_CHB2_GPIO_PORT, HRTIM1_CHB2_PIN, LL_GPIO_AF_13);
}

/**
* @brief  HRTIM configuration
* @param  None
* @retval None
*/
static void HRTIM_Config(void)
{
#if (USE_TIMEOUT == 1)
  uint32_t Timeout = 0; /* Variable used for timeout management */
#endif /* USE_TIMEOUT */


  /*## Configuration of GPIO used by HRTIM1_FLT1 #############################*/
  
  /* HRTIM1 fault channel 1 is mapped on GPIO pin PA12 */ 
  
  /* Enable GPIO Clock */
  HRTIM1_FLT1_GPIO_CLK_ENABLE();

  /* Configure PA12 as alternate function for HRTIM1_FLT1 */
  LL_GPIO_SetPinMode(HRTIM1_FLT1_GPIO_PORT, HRTIM1_FLT1_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(HRTIM1_FLT1_GPIO_PORT, HRTIM1_FLT1_PIN, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetAFPin_8_15(HRTIM1_FLT1_GPIO_PORT, HRTIM1_FLT1_PIN, LL_GPIO_AF_13);

 
  /*## HRTIM clock source selection and enabling #############################*/
  
  /* Use the PLLx2 clock for HRTIM */
  LL_RCC_SetHRTIMClockSource(LL_RCC_HRTIM1_CLKSOURCE_PLL);
  
  /* Enable HRTIM clock*/
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_HRTIM1);
  
  

  /*## DLL Calibration #######################################################*/
  
  /* DLL calibration: periodic calibration enabled, period set to 14µs */
  LL_HRTIM_ConfigDLLCalibration(HRTIM1, 
                                LL_HRTIM_DLLCALIBRATION_MODE_CONTINUOUS,
                                LL_HRTIM_DLLCALIBRATION_RATE_14);
    
  /* Poll for DLL end of calibration */
#if (USE_TIMEOUT == 1)
  Timeout = HRTIM1_DLL_CALIBRATION_TIMEOUT_MS;
#endif /* USE_TIMEOUT */
    
  while((LL_HRTIM_IsActiveFlag_DLLRDY(HRTIM1)) == RESET)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      if(Timeout-- == 0)
      {
        /* Time-out occurred. Set LED to blinking mode */
        LED_Blinking(LED_BLINK_ERROR);
      }
    }
#endif /* USE_TIMEOUT */
  }
  

  /*## TIMER A - Configuration ###############################################*/
  
  /* TIMER A - 250kHz switching frequency */
  LL_HRTIM_TIM_SetPeriod(HRTIM1,
                         LL_HRTIM_TIMER_A,
                         BUCK_PWM_PERIOD);
  
  /* TIMER A - 1 repetition event every 32 PWM periods */ 
  LL_HRTIM_TIM_SetRepetition(HRTIM1,
                             LL_HRTIM_TIMER_A, 31);
  
  /* TIMER A - set clock prescaling ratio to acheive the highest possible resolution */
  LL_HRTIM_TIM_SetPrescaler(HRTIM1,
                            LL_HRTIM_TIMER_A,
                            LL_HRTIM_PRESCALERRATIO_MUL32);
  
  /* TIMER A - Operates in continuous (free-running) mode */
  LL_HRTIM_TIM_SetCounterMode(HRTIM1,
                              LL_HRTIM_TIMER_A,
                              LL_HRTIM_MODE_CONTINUOUS);
  
  /* TIMER A - Enable preload mechanism*/
  LL_HRTIM_TIM_EnablePreload(HRTIM1,
                             LL_HRTIM_TIMER_A);
  
  /* TIMER A - Update on repetition is enabled */
  LL_HRTIM_TIM_SetUpdateTrig(HRTIM1,
                             LL_HRTIM_TIMER_A,
                             LL_HRTIM_UPDATETRIG_REPETITION);
  
  /* TIMER A - Fault1 is active and can disable HRTIM outputs */
  LL_HRTIM_TIM_EnableFault(HRTIM1,
                           LL_HRTIM_TIMER_A,
                           LL_HRTIM_FAULT_1);

  /* TIMER A - Dead time insertion is enabled */
  LL_HRTIM_TIM_EnableDeadTime(HRTIM1,
                              LL_HRTIM_TIMER_A);
  
  /* TIMER A - Configure compare unit 1 (Start with minimal duty cycle) */
  LL_HRTIM_TIM_SetCompare1(HRTIM1,
                           LL_HRTIM_TIMER_A,
                           BUCK_PWM_PERIOD - 0x60);

  /*## TIMER B - Time Base configuration #####################################*/
  
  /* TIMER B - 250kHz switching frequency */
  LL_HRTIM_TIM_SetPeriod(HRTIM1,
                         LL_HRTIM_TIMER_B,
                         BUCK_PWM_PERIOD);
  
  /* TIMER B - 1 repetion event every 32 PWM periods */ 
  LL_HRTIM_TIM_SetRepetition(HRTIM1,
                             LL_HRTIM_TIMER_B, 31);
  
  /* TIMER B - Set clock prescaling ratio to acheive the highest possible resolution */
  LL_HRTIM_TIM_SetPrescaler(HRTIM1,
                            LL_HRTIM_TIMER_B,
                            LL_HRTIM_PRESCALERRATIO_MUL32);
  
  /* TIMER B -Operates in continuous (free-running) mode */
  LL_HRTIM_TIM_SetCounterMode(HRTIM1,
                              LL_HRTIM_TIMER_B,
                              LL_HRTIM_MODE_CONTINUOUS);
  
  /* TIMER B - Enable preload mechanism*/
  LL_HRTIM_TIM_EnablePreload(HRTIM1,
                             LL_HRTIM_TIMER_B);
  
  /* TIMER B - Update on repetition is enabled */
  LL_HRTIM_TIM_SetUpdateTrig(HRTIM1,
                             LL_HRTIM_TIMER_B,
                             LL_HRTIM_UPDATETRIG_REPETITION);
  
  /* TIMER B - Dead time insertion is enabled */
  LL_HRTIM_TIM_EnableDeadTime(HRTIM1,
                              LL_HRTIM_TIMER_B);
  
  /* TIMER B - Configure compare unit 1 (0% duty cycle) */
  LL_HRTIM_TIM_SetCompare1(HRTIM1,
                           LL_HRTIM_TIMER_B,
                           BUCK_PWM_PERIOD + 1);


  
  /*## TA1 configuration #####################################################*/
  
  /* TA1 - Timer A CMP1 event forces TA1 to its active state */
  LL_HRTIM_OUT_SetOutputSetSrc(HRTIM1,
                               LL_HRTIM_OUTPUT_TA1,
                               LL_HRTIM_CROSSBAR_TIMCMP1);
    
  /* TA1 - Timer A period event forces TA1 to its inactive state */
  LL_HRTIM_OUT_SetOutputResetSrc(HRTIM1,
                                 LL_HRTIM_OUTPUT_TA1,
                                 LL_HRTIM_CROSSBAR_TIMPER);
  
  /* TA1 - After a fault event TA1 is set to its inactive state */
  LL_HRTIM_OUT_SetFaultState(HRTIM1,
                             LL_HRTIM_OUTPUT_TA1,
                             LL_HRTIM_OUT_FAULTSTATE_INACTIVE);
  

  /*## TA2 configuration #####################################################*/
  
  /* TA2 - Timer A CMP1 event forces TA1 to its active state */
  LL_HRTIM_OUT_SetOutputSetSrc(HRTIM1,
                               LL_HRTIM_OUTPUT_TA2,
                               LL_HRTIM_CROSSBAR_TIMCMP1);
    
  /* TA2 - Timer A period event forces TA1 to its inactive state */
  LL_HRTIM_OUT_SetOutputResetSrc(HRTIM1,
                                 LL_HRTIM_OUTPUT_TA2,
                                 LL_HRTIM_CROSSBAR_TIMPER);
  
  /* TA2 - After a fault event TA1 is set to its inactive state */
  LL_HRTIM_OUT_SetFaultState(HRTIM1,
                             LL_HRTIM_OUTPUT_TA2,
                             LL_HRTIM_OUT_FAULTSTATE_INACTIVE);
  

  /*## TB1 configuration #####################################################*/

  /* TB1 - Timer B CMP1 event forces TB1 to its active state */
  LL_HRTIM_OUT_SetOutputSetSrc(HRTIM1,
                               LL_HRTIM_OUTPUT_TB1,
                               LL_HRTIM_CROSSBAR_TIMCMP1);
    
  /* TB1 - Timer B period event forces TB1 to its inactive state */
  LL_HRTIM_OUT_SetOutputResetSrc(HRTIM1,
                                 LL_HRTIM_OUTPUT_TB1,
                                 LL_HRTIM_CROSSBAR_TIMPER);
  
  /* TB1 - After a fault event TB1 is set to its inactive state */
  LL_HRTIM_OUT_SetFaultState(HRTIM1,
                             LL_HRTIM_OUTPUT_TB1,
                             LL_HRTIM_OUT_FAULTSTATE_INACTIVE);  


  /*## TB2 configuration #####################################################*/

  /* TB2 - Timer B CMP1 event forces TB2 to its active state */
  LL_HRTIM_OUT_SetOutputSetSrc(HRTIM1,
                               LL_HRTIM_OUTPUT_TB2,
                               LL_HRTIM_CROSSBAR_TIMCMP1);
    
  /* TB2 - Timer B period event forces TB2 to its inactive state */
  LL_HRTIM_OUT_SetOutputResetSrc(HRTIM1,
                                 LL_HRTIM_OUTPUT_TB2,
                                 LL_HRTIM_CROSSBAR_TIMPER);
  
  /* TB2 - After a fault event TB2 is set to its inactive state */
  LL_HRTIM_OUT_SetFaultState(HRTIM1,
                             LL_HRTIM_OUTPUT_TB2,
                             LL_HRTIM_OUT_FAULTSTATE_INACTIVE);  
  

  /*## Deadtime insertion configuration #####################################*/

  /* TIMER A - Deadtime insertion configuration */
  LL_HRTIM_DT_SetRisingValue(HRTIM1, LL_HRTIM_TIMER_A, DT_RISING);
  LL_HRTIM_DT_SetFallingValue(HRTIM1, LL_HRTIM_TIMER_A, DT_FALLING);
  LL_HRTIM_DT_LockRisingSign(HRTIM1, LL_HRTIM_TIMER_A);
  LL_HRTIM_DT_LockFallingSign(HRTIM1, LL_HRTIM_TIMER_A);

  /* TIMER B - Deadtime insertion configuration */
  LL_HRTIM_DT_SetRisingValue(HRTIM1, LL_HRTIM_TIMER_B, DT_RISING);
  LL_HRTIM_DT_SetFallingValue(HRTIM1, LL_HRTIM_TIMER_B, DT_FALLING);
  LL_HRTIM_DT_LockRisingSign(HRTIM1, LL_HRTIM_TIMER_B);
  LL_HRTIM_DT_LockFallingSign(HRTIM1, LL_HRTIM_TIMER_B);


  /*## ADC trigger configuration #############################################*/

  /* TIMER A - Configure compare unit 2 (CMP2 event used as ADC trigger) */
  LL_HRTIM_TIM_SetCompare2(HRTIM1,
                           LL_HRTIM_TIMER_A,
                           BUCK_PWM_PERIOD/2);
  
  /* ADC trigger initialization */
  LL_HRTIM_ConfigADCTrig(HRTIM1,
                         LL_HRTIM_ADCTRIG_2,
                         LL_HRTIM_ADCTRIG_UPDATE_TIMER_A,
                         LL_HRTIM_ADCTRIG_SRC24_TIMACMP2);
  

  /*## Fault input 1 configuration #########################################*/

  /* Fault 1 input is HRTIM1_FLT1 input pin and it is active low */
  LL_HRTIM_FLT_Config(HRTIM1,
                      LL_HRTIM_FAULT_1,
                      LL_HRTIM_FLT_SRC_DIGITALINPUT | LL_HRTIM_FLT_POLARITY_LOW);
  
  /* Enable Fault input 1 circuitry */
  LL_HRTIM_FLT_Enable(HRTIM1, LL_HRTIM_FAULT_1);
  

  /*## TIMER A - interrupts configuration ####################################*/
  
  /* Timer A - Enable repetition interrupt */
  LL_HRTIM_EnableIT_REP(HRTIM1, LL_HRTIM_TIMER_A);
  
  /* Configure and enable HRTIM TIMERA interrupt channel in NVIC */
  NVIC_EnableIRQ(HRTIM1_TIMA_IRQn);
  NVIC_SetPriority(HRTIM1_TIMA_IRQn, 0);
}

/**
  * @brief  Configure ADC1 for being used with HRTIM
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  /*## Configuration of GPIO used by ADC channels ############################*/
  
  /* ADC1 channel 2 is mapped on GPIO pin PA.01 */ 
  
  /* Enable GPIO Clock */
  ADC1_IN2_GPIO_CLK_ENABLE();
  
  /* Configure GPIO in analog mode to be used as ADC input */
  LL_GPIO_SetPinMode(ADC1_IN2_GPIO_PORT, ADC1_IN2_PIN, LL_GPIO_MODE_ANALOG);
  
  /* ADC1 channel 4 is mapped on GPIO pin PA.03 */ 

  /* Enable GPIO Clock */
  ADC1_IN4_GPIO_CLK_ENABLE();
  
  /* Configure GPIO in analog mode to be used as ADC input */
  LL_GPIO_SetPinMode(ADC1_IN4_GPIO_PORT, ADC1_IN4_PIN, LL_GPIO_MODE_ANALOG);
  
  /*## Configuration of ADC ##################################################*/
  
  /*## Configuration of ADC hierarchical scope: common to several ADC ########*/
  
  /* Enable ADC clock (core clock) */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_ADC12);
  
  /* Enable ADC clock (conversion clock) */
  LL_RCC_SetADCClockSource(LL_RCC_ADC12_CLKSRC_PLL_DIV_1);
  
  
  if(__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
  {
    /* Set ADC clock (conversion clock) common to several ADC instances */
    LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_ASYNC_DIV1);    
  }
    
  /*## Configuration of ADC hierarchical scope: ADC group injected ###########*/
  
  if ((LL_ADC_IsEnabled(ADC1) == 0) ||
      (LL_ADC_INJ_IsConversionOngoing(ADC1) == 0))
  {
    /* ADC group injected conversion trigger source: HRTIM_ADCTRG2 event */
    /* Hardware trigger detection on the rising edge */
    /* Injected sequencer sequence length: 2 concersions */
    /* ADC channel 2 is assigned as the 1st in the injected conversion sequence */
    /* ADC channel 4 is assigned as the 2nd in the injected conversion sequence */
    LL_ADC_INJ_ConfigQueueContext(ADC1,
                                  /*LL_ADC_INJ_TRIG_SOFTWARE, */LL_ADC_INJ_TRIG_EXT_HRTIM_TRG2,
                                  /*0x00000000, */LL_ADC_INJ_TRIG_EXT_RISING,
                                  LL_ADC_INJ_SEQ_SCAN_ENABLE_2RANKS,
                                  LL_ADC_CHANNEL_2,
                                  LL_ADC_CHANNEL_4,
                                  LL_ADC_CHANNEL_0,
                                  LL_ADC_CHANNEL_0); 
  }
  
  /*## Configuration of ADC hierarchical scope: channels #####################*/
  
  if ((LL_ADC_IsEnabled(ADC1) == 0) ||
      ((LL_ADC_REG_IsConversionOngoing(ADC1) == 0) &&
       (LL_ADC_INJ_IsConversionOngoing(ADC1) == 0)   )   )
  {
    /* Configure the 1st injected conversion for Vin on Ch2 */
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_2, LL_ADC_SAMPLINGTIME_7CYCLES_5);
    
    /* Configure the 2nd injected conversion for Vout on Ch4 */
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_19CYCLES_5);
  }
}

/**
  * @brief  Perform the ADC activation procedure to make it ready to convert
  * @param  None
  * @retval None
  */
void ADC_Activate(void)
{
  __IO uint32_t wait_loop_index = 0;
  
#if (USE_TIMEOUT == 1)
  uint32_t Timeout = 0; /* Variable used for timeout management */
#endif /* USE_TIMEOUT */
  
  /*## Operation on ADC hierarchical scope: ADC instance #####################*/
  
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Enable ADC internal voltage regulator */
    LL_ADC_EnableInternalRegulator(ADC1);
    
    /* Delay for ADC internal voltage regulator stabilization.                */
    /* Compute number of CPU cycles to wait for, from delay in us.            */
    /* If system core clock frequency is below 100kHz, wait time is only      */
    /* a few CPU cycles of processing time.                                   */
    wait_loop_index = (LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / 100000) /10);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }
    
    /* Run ADC self calibration */
    LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
    
    /* Poll for ADC effectively calibrated */
#if (USE_TIMEOUT == 1)
    Timeout = ADC_CALIBRATION_TIMEOUT_MS;
#endif /* USE_TIMEOUT */
    
    while (LL_ADC_IsCalibrationOnGoing(ADC1) == 0)
    {
#if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag()) 
      {
        if(Timeout-- == 0)
        {
        /* Time-out occurred. Set LED to blinking mode */
        LED_Blinking(LED_BLINK_ERROR);
        }
      }
#endif /* USE_TIMEOUT */
    }
    
    /* Delay between ADC end of calibration and ADC enable.                   */
    /* (approximative delay: minimum delay without taking into account        */
    /* processing and optimizations CPU cycles).                              */
    wait_loop_index = ADC_DELAY_CALIB_ENABLE_CPU_CYCLES;
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }
    
    /* Enable ADC */
    LL_ADC_Enable(ADC1);
    
    /* Poll for ADC ready to convert */
#if (USE_TIMEOUT == 1)
    Timeout = ADC_ENABLE_TIMEOUT_MS;
#endif /* USE_TIMEOUT */
    
    while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
    {
#if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag()) 
      {
        if(Timeout-- == 0)
        {
        /* Time-out occurred. Set LED to blinking mode */
        LED_Blinking(LED_BLINK_ERROR);
        }
      }
#endif /* USE_TIMEOUT */
    }
    
    /* Note: ADC flag ADRDY is not cleared here to be able to check ADC       */
    /*       status afterwards.                                               */
    /*       This flag should be cleared at ADC Deactivation, before a new    */
    /*       ADC activation, using function "LL_ADC_ClearFlag_ADRDY()".       */
  }
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED_RED
  *     @arg LED_BLUE
  *     @arg LED_ORANGE
  *     @arg LED_GREEN
  * @retval None
  */
void LED_Init(Led_t Led)
{
  switch (Led)
  {
  case LED_RED:
    /* Enable the LED3 Clock */
    LED3_GPIO_CLK_ENABLE();
    
    /* Configure IO in output push-pull mode to drive external LED3 */
    LL_GPIO_SetPinMode(LED3_GPIO_PORT, LED3_PIN, LL_GPIO_MODE_OUTPUT);
    break;
    
  case LED_BLUE:
    /* Enable the LED6 Clock */
    LED6_GPIO_CLK_ENABLE();
    
    /* Configure IO in output push-pull mode to drive external LED6 */
    LL_GPIO_SetPinMode(LED6_GPIO_PORT, LED6_PIN, LL_GPIO_MODE_OUTPUT);
    break;
    
  case LED_ORANGE:
    /* Enable the LED4 Clock */
    LED4_GPIO_CLK_ENABLE();
    
    /* Configure IO in output push-pull mode to drive external LED4 */
    LL_GPIO_SetPinMode(LED4_GPIO_PORT, LED4_PIN, LL_GPIO_MODE_OUTPUT);
    break;
    
  case LED_GREEN:
    /* Enable the LED5 Clock */
    LED5_GPIO_CLK_ENABLE();
    
    /* Configure IO in output push-pull mode to drive external LED5 */
    LL_GPIO_SetPinMode(LED5_GPIO_PORT, LED5_PIN, LL_GPIO_MODE_OUTPUT);
    break;
  }
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED_RED
  *     @arg LED_BLUE
  *     @arg LED_ORANGE
  *     @arg LED_GREEN
  * @retval None
  */
void LED_On(Led_t Led)
{ 
  switch (Led)
  {
  case LED_RED:
    /* Turn LED3 on */
    LL_GPIO_SetOutputPin(LED3_GPIO_PORT, LED3_PIN);
    break;
    
  case LED_BLUE:
    /* Turn LED6 on */
    LL_GPIO_SetOutputPin(LED6_GPIO_PORT, LED6_PIN);
    break;
    
  case LED_ORANGE:
    /* Turn LED4 on */
    LL_GPIO_SetOutputPin(LED4_GPIO_PORT, LED4_PIN);
    break;
    
  case LED_GREEN:
    /* Turn LED5 on */
    LL_GPIO_SetOutputPin(LED5_GPIO_PORT, LED5_PIN);
    break;
  }
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED_RED
  *     @arg LED_BLUE
  *     @arg LED_ORANGE
  *     @arg LED_GREEN
  * @retval None
  */
void LED_Off(Led_t Led)
{
  switch (Led)
  {
  case LED_RED:
    /* Turn LED3 off */
    LL_GPIO_ResetOutputPin(LED3_GPIO_PORT, LED3_PIN);
    break;
    
  case LED_BLUE:
    /* Turn LED6 off */
    LL_GPIO_ResetOutputPin(LED6_GPIO_PORT, LED6_PIN);
    break;
    
  case LED_ORANGE:
    /* Turn LED4 off */
    LL_GPIO_ResetOutputPin(LED4_GPIO_PORT, LED4_PIN);
    break;
    
  case LED_GREEN:
    /* Turn LED5 off */
    LL_GPIO_ResetOutputPin(LED5_GPIO_PORT, LED5_PIN);
    break;
  }
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED_RED
  *     @arg LED_BLUE
  *     @arg LED_ORANGE
  *     @arg LED_GREEN
  * @retval None
  */
void LED_Toggle(Led_t Led)
{
  switch (Led)
  {
  case LED_RED:
    /* Turn LED3 off */
    LL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN);
    break;
    
  case LED_BLUE:
    /* Turn LED6 off */
    LL_GPIO_TogglePin(LED6_GPIO_PORT, LED6_PIN);
    break;
    
  case LED_ORANGE:
    /* Turn LED4 off */
    LL_GPIO_TogglePin(LED4_GPIO_PORT, LED4_PIN);
    break;
    
  case LED_GREEN:
    /* Turn LED5 off */
    LL_GPIO_TogglePin(LED5_GPIO_PORT, LED5_PIN);
    break;
  }
}

/**
  * @brief  Set LED3 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED3_GPIO_PORT, LED3_PIN);
  
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN);  
    LL_mDelay(Period);
  }
}

/**
  * @brief  Configures User push-button in GPIO Mode.
  * @param  None 
  * @retval None
  */
static void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();

  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_DOWN);
}

/**
  * @brief  Returns the User push-button state.
  * @retval The Button GPIO pin value.
  */
static uint32_t UserButton_GetState(void)
{
  return (LL_GPIO_IsInputPinSet(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN) ? SET : RESET);
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
  /* Set FLASH latency */ 
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE if not already activated*/
  if (LL_RCC_HSE_IsReady() == 0)
  {
    /* Enable HSE and wait for activation*/
    LL_RCC_HSE_Enable(); 
    while(LL_RCC_HSE_IsReady() != 1)
    {
    };
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(RCC_CFGR_PLLSRC_HSE_PREDIV, RCC_CFGR_PLLMUL9);
  
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  
  /* Set systick to 1ms in using frequency set to 72MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ ((HSI_VALUE / 2), LL_RCC_PLL_MUL_16) */
  LL_Init1msTick(72000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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
