/**
  ******************************************************************************
  * @file    HRTIM/HRTIM_BuckBoost_AN4449/Src/stm32f3xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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
#include "stm32f3xx_it.h"
#include "main.h"


/** @addtogroup STM32F3xx_HAL_Examples
* @{
*/

/** @addtogroup HRTIM_BuckBoost_AN4449
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEBUG /* for debug purpose only (check comments for use) */
#define OVERLOAD_ON /* to check if converter in overload conditions then stop it */

/* Private constants ------------------------------------------------------------*/
#define POLYNOMIAL_INTERPOLATION_COEFF_NUMBER 4
#define LINEAR_REGRESSION_COEFF_NUMBER 2
#define VOUT_RANGE                 13

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ADC_HandleTypeDef AdcHandle;
HRTIM_HandleTypeDef hhrtim;
BuckBoost_ModeTypeDef Mode;
static uint16_t toggle = 0;
static uint32_t CompareDuty;
static uint32_t CounterDuty = 0;
static uint32_t inter1;
static uint32_t inter2;
static float V1term;
static float V2term;
static float V3term;
static uint32_t LimitDuty;


volatile uint32_t VinConversion, VoutConversion;
uint32_t CurrentDutyA;
uint32_t CurrentDutyB;
uint32_t CurrentDutyB_Mixed;
uint32_t CTMin;
uint32_t CTMax;
uint32_t CTRange;
volatile uint32_t VoutT, VoutRange, VoutA, VoutB;

volatile uint8_t Idle2Buck;
volatile uint8_t Idle2Boost; 
volatile uint8_t Run2Stop;

/* Protection table for Buck based on polynomial interpolation */
uint32_t ProTableBuck[VOUT_RANGE][POLYNOMIAL_INTERPOLATION_COEFF_NUMBER] =
  {
    {9690, 378284, 5262010, 31172},
    {8815, 367222, 5519670, 35775},
    {8017, 352917, 5672600, 39800},
    {7952, 363557, 6089150, 44656},
    {8083, 376875, 6484920, 49270},
    {4400, 248500, 5144900, 46845},
    {7417, 372500, 6939080, 57272},
    {0, 61000, 2690000, 39804},
    {0, 68500, 2985500, 43788},
    {0, 0, 1036000,  31186},
    {0, 0, 0,  16588},
    {0, 0, 0,  16588},
    {0, 0, 0,  16588}
  };

/* Protection table for Boost based on linear regression */
uint16_t ProTableBoost[VOUT_RANGE][LINEAR_REGRESSION_COEFF_NUMBER] =
  {
    {6135, 22046},
    {4609, 21015},
    {3779, 20890},
    {3131, 20499},
    {2685, 20318},
    {2336, 20094},
    {2073, 19973},
    {1864, 19876},
    {1696, 19814},
    {1552, 19730},
    {1431, 19674},
    {1328, 19622},
    {1239, 19583}

  };
/* Protection table for Mixed based on linear regression */
uint16_t ProTableMixed[VOUT_RANGE][LINEAR_REGRESSION_COEFF_NUMBER] =
  {
    {5812, 27177},
    {3763, 22586},
    {3054, 22158},
    {2522, 21591},
    {2136, 21135},
    {1835, 20646},
    {1595, 20164},
    {1455, 20240},
    {1324, 20169},
    {1194, 19833},
    {1093, 19656},
    {1047, 20074},
    {963,  19792}

  };


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern void SetHRTIM_BuckMode(void);
extern void SetHRTIM_BoostMode(void);
extern void SetHRTIM_MixedMode(void);
extern int32_t PI_Buck(void);
extern int32_t PI_Boost(void);
extern int32_t PI_Mixed(void);

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
* @brief   This function handles NMI exception.
* @param  None
* @retval None
*/
void NMI_Handler(void)
{}

/**
* @brief  This function handles Hard Fault exception.
* @param  None
* @retval None
*/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
* @brief  This function handles Memory Manage exception.
* @param  None
* @retval None
*/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
* @brief  This function handles Bus Fault exception.
* @param  None
* @retval None
*/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
* @brief  This function handles Usage Fault exception.
* @param  None
* @retval None
*/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
* @brief  This function handles SVCall exception.
* @param  None
* @retval None
*/
void SVC_Handler(void)
{}

/**
* @brief  This function handles Debug Monitor exception.
* @param  None
* @retval None
*/
void DebugMon_Handler(void)
{}

/**
* @brief  This function handles PendSVC exception.
* @param  None
* @retval None
*/
void PendSV_Handler(void)
{}

/**
* @brief  This function handles SysTick Handler.
* @param  None
* @retval None
*/
void SysTick_Handler(void)
{
  HAL_IncTick();
}


/**
* @brief  This function handles external line 0 interrupt request.
* @param  None
* @retval None
*/
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
}

/******************************************************************************/
/*                 STM32F3xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) , for the       */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f3xx.s).                                               */
/******************************************************************************/

void HRTIM1_TIMD_IRQHandler(void)
{
#if defined(DEBUG)
  __HAL_HRTIM_TIMER_CLEAR_IT(&hhrtim, HRTIM_TIMERINDEX_TIMER_D, HRTIM_TIM_IT_REP);
#endif //debug
}

/**
* @brief  This function handles HRTIM1 TIMA interrupt request.
* @param  None
* @retval None
*/
void HRTIM1_TIMA_IRQHandler(void)
{
  /* Local variables used to prevent compilation warning */
  uint32_t lVoutT = VoutT;
  uint32_t lVoutRange = VoutRange;
  uint32_t lVoutA = VoutA;
  uint32_t lVoutB = VoutB;

  //BSP_LED_On(LED3); /* Uncomment for interrupt duration */
  /* Interrupt occurs every 8 * 4µs = 32µs (repetition counter * PWM period) */
  /* LEDs are toggling every 2500 * 32µs = 80ms */
  /* Overload is checked every 80/50 = 1.6ms */

  /* Clear Interrupt flag */
  __HAL_HRTIM_TIMER_CLEAR_IT(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_TIM_IT_REP);

  /* Create 1 event every 80ms period */
  toggle++;
  if (toggle == 2500)
  {
    toggle = 0;
  }

  switch (Mode)
  {

      /* Idle Mode waiting application start */
    case BUCKBOOST_MODE_IDLE:
      /* Check if a mode change is set and apply converter mode change*/
      if (Idle2Buck == SET)
      {
        SetHRTIM_BuckMode();
      }
      if (Idle2Boost == SET)
      {
        SetHRTIM_BoostMode();
      }
      break;


      /*************************/
      /* Buck Mode Vout << Vin */
      /*************************/
    case BUCKBOOST_MODE_BUCK:

      BSP_LED_Off(LED6);
      /* Toggling GREEN LED if BUCK mode active */
      if (toggle == 0)
      {
        BSP_LED_Toggle(LED5);
      }

      /* Compute PI buck Mode */
      CurrentDutyA = PI_Buck();
      /* Update DutyA on BUCK side */
      __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, CurrentDutyA);
      /* ADC trigger position update */
      if (CurrentDutyA > BUCK_PWM_PERIOD / 2)
      {
        /* Set ADC trigger position according CurrentDutyA */
        __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_4, DT_RISING + CurrentDutyA / 2);
      }
      else
      {
        __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_4, ((BUCK_PWM_PERIOD - CurrentDutyA) / 2) + CurrentDutyA);
      }
      /* Check for any conditions change then stop converter or change mode */
      /* Max % duty reached for a given time */
      if (CTMax >= MAX_ERROR)
      {
        /* test Voltage */
        if (VoutConversion > (lVoutT / 4))/* Check if Vout reaches at least 25% of VoutT and not stuck to GND */
        {
          SetHRTIM_MixedMode();
        }
        /* Otherwise STOP converter */
        else
        {
          /* Stop converter */
          Run2Stop = SET;
        }
      }
      /* Min % duty reached for a given time */
      if (CTMin >= MAX_ERROR)
      {
        /* Stop converter */
        Run2Stop = SET;
      }


#if defined(OVERLOAD_ON)
      /* BUCKBOOST_MODE_BUCK */
      /* Every 1.6ms */
      if (toggle % 50 == 0)
      {
        /* Check for Vin value */
        V1term = (((VinConversion * REAL_3V3) / 0x1000) * 10000)/VIN_RESISTOR_RATIO;
        V1term /= 1000;
        /* Compute x^2 and x^3 terms for polynomial interpolation */
        V2term = V1term * V1term;
        V3term = V2term * V1term;
        /* Compute polynomial interpolation for corresponding Vin and Vout conditions */
        inter1 = (uint32_t) (- (ProTableBuck[lVoutRange][0] * V3term) / 1000 +  (ProTableBuck[lVoutRange][1] * V2term) / 1000 - (ProTableBuck[lVoutRange][2] * V1term) / 1000 + ProTableBuck[lVoutRange][3]);

        /* If VOUT_TARGET not an integer value compute 2nd polynomial interpolation */
        if (lVoutA != 0)
        {
          inter2 = (uint32_t) (- (ProTableBuck[lVoutRange + 1][0] * V3term) / 1000 +  (ProTableBuck[lVoutRange + 1][1] * V2term) / 1000 - (ProTableBuck[lVoutRange + 1][2] * V1term) / 1000 + ProTableBuck[lVoutRange + 1][3]);
          /* and finally compute linear interpolation according VOUT_TARGET settings between the 2 curves */
          LimitDuty = (lVoutA * inter1) / 1000 + (lVoutB * inter2) / 1000;
        }
        else
        {
          LimitDuty = inter1;
        }
        /* limited to Max or Min boundaries */
        if (LimitDuty >= MAX_DUTY_A)
        {
          LimitDuty = MAX_DUTY_A;
        }
        if (LimitDuty <= MIN_DUTY_A)
        {
          LimitDuty = MIN_DUTY_A;
        }
      }
#endif //OVERLOAD_ON

      break;

      /**************************/
      /* Boost Mode Vout >> Vin */
      /**************************/
    case BUCKBOOST_MODE_BOOST:

      BSP_LED_Off(LED5);
      /* Toggling BLUE LED if BOOST mode active */
      if (toggle == 0)
      {
        BSP_LED_Toggle(LED6);
      }
      /* Compute PI boost Mode */
      CurrentDutyB = PI_Boost();
      /* Update DutyB on BOOST side */
      __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, CurrentDutyB);

      if (CurrentDutyB > BUCK_PWM_PERIOD / 2)
      {
        /* Set ADC trigger position according CurrentDutyB */
        __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_4, DT_RISING + CurrentDutyB / 2);
      }
      else
      {
        __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_4, ((BUCK_PWM_PERIOD - CurrentDutyB) / 2) + CurrentDutyB);
      }

      if (CTMax >= MAX_ERROR)
      {
        Run2Stop = SET;
      }
      if (CTMin >= MAX_ERROR)
      {
        SetHRTIM_MixedMode();
      }

#if defined(OVERLOAD_ON)
      /* BUCKBOOST_MODE_BOOST */
      if (toggle % 50 == 0)
      {
        /* Check for Vin value */
        V1term = (((VinConversion * REAL_3V3) / 0x1000) * 10000)/VIN_RESISTOR_RATIO;
        V1term /= 1000;
        /* Compute linear regression */
        inter1 = (uint16_t) (- (ProTableBoost[lVoutRange][0] * V1term) +  ProTableBoost[lVoutRange][1]);

        /* If VOUT_TARGET not an integer value compute 2nd linear regression */
        if (lVoutA != 0)
        {
          inter2 = (uint16_t) (- (ProTableBoost[lVoutRange + 1][0] * V1term) +  ProTableBoost[lVoutRange + 1][1]);
          /* and finally compute linear interpolation according VOUT_TARGET settings between the 2 regression rights */
          LimitDuty = (lVoutA * inter1) / 1000 + (lVoutB * inter2) / 1000;
        }
        else
        {
          LimitDuty = inter1;
        }
        /* limited to Max or Min boundaries */
        if (LimitDuty >= MAX_DUTY_B)
        {
          LimitDuty = MAX_DUTY_B;
        }
        if (LimitDuty <= MIN_DUTY_B)
        {
          LimitDuty = MIN_DUTY_B;
        }
      }
#endif //OVERLOAD_ON

      break;


      /****************************/
      /* Mixed Mode Vout near Vin */
      /****************************/
    case BUCKBOOST_MODE_MIXED:

      /* Toggling both GREEN AND BLUE LEDs if MIXED mode active */
      if (toggle == 0)
      {
        BSP_LED_Toggle(LED6);
      }
      if (toggle == 1250)
      {
        BSP_LED_Toggle(LED5);
      }
      /* Compute PI mixed Mode */
      CurrentDutyB_Mixed = PI_Mixed();
      /* Set fixed DutyA on BUCK side set to 80% */
      __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, (BUCK_PWM_PERIOD * 80) / 100);
      /* Set fixed ADC trigger position located between DutyA (80%) and DutyB (max. 40%) */
      __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_4, (BUCK_PWM_PERIOD * 60) / 100);
      /* Update DutyB on BOOST side */
      __HAL_HRTIM_SETCOMPARE(&hhrtim, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_2, CurrentDutyB_Mixed);

      if (CTMax >= MAX_ERROR)
      {
        SetHRTIM_BoostMode();
      }
      if (CTMin >= MAX_ERROR)
      {
        SetHRTIM_BuckMode();
      }

#if defined(OVERLOAD_ON)
      /* BUCKBOOST_MODE_MIXED */
      if (toggle % 50 == 0)
      {
        /* Check for Vin value */
        V1term = (((VinConversion * REAL_3V3) / 0x1000) * 10000)/VIN_RESISTOR_RATIO;
        V1term /= 1000;
        /* Compute linear regression */
        inter1 = (uint16_t) (- (ProTableMixed[lVoutRange][0] * V1term) +  ProTableMixed[lVoutRange][1]);

        /* If VOUT_TARGET not an integer value compute 2nd linear regression */
        if (lVoutA != 0)
        {
          inter2 = (uint16_t) (- (ProTableMixed[lVoutRange + 1][0] * V1term) +  ProTableMixed[lVoutRange + 1][1]);
          /* and finally compute linear interpolation according VOUT_TARGET settings between the 2 regression rights */
          LimitDuty = (lVoutA * inter1) / 1000 + (lVoutB * inter2) / 1000;
        }
        else
        {
          LimitDuty = inter1;
        }
        /* limited to Max or Min boundaries */
        if (LimitDuty >= MAX_DUTY_B_MIXED)
        {
          LimitDuty = MAX_DUTY_B_MIXED;
        }
        if (LimitDuty <= MIN_DUTY_B_MIXED)
        {
          LimitDuty = MIN_DUTY_B_MIXED;
        }
      }
#endif //OVERLOAD_ON

      break;
  }



#if defined(OVERLOAD_ON)
  if (toggle % 50 == 0)
  {
    /* Check for the current Duty cycle to be compared with limit duty*/
    if (Mode == BUCKBOOST_MODE_BUCK)
    {
      CompareDuty = CurrentDutyA;
    }
    else if (Mode == BUCKBOOST_MODE_BOOST)
    {
      CompareDuty = CurrentDutyB;
    }
    else if (Mode == BUCKBOOST_MODE_MIXED)
    {
      CompareDuty = CurrentDutyB_Mixed;
    }
    /* If current Duty near limit then LD4 (orange) starts toggling */
    if (CompareDuty > ((LimitDuty * OVERLOAD_MARGIN)/100))
    {
      if (toggle % 1250 == 0)
      {
        BSP_LED_Toggle(LED4);
      }
    }
    else
    {
      BSP_LED_Off(LED4);
    }

    /* If current Duty is above the limit then increment Duty counter */
    if (CompareDuty > LimitDuty)
    {
      CounterDuty++;
    }
    /* otherwise decrement if still positive */
    else
    {
      if (CounterDuty != 0)
      {
        CounterDuty--;
      }
    }
    /* Stop converter if overload is present for a minimum period */
    if (CounterDuty >= MAX_OVERLOAD)
    {
      Run2Stop = SET;
    }
  }

#endif //OVERLOAD_ON

  BSP_LED_Off(LED3);// Uncomment for interrupt duration */
}




/**
* @brief  This function handles ADC1_2 interrupt request.
* @param  None
* @retval None
*/
void ADC1_2_IRQHandler(void)
{
  /* Clear ADC interrupt flag */
  __HAL_ADC_CLEAR_FLAG(&AdcHandle, ADC_FLAG_JEOS);
  /* Get conversion value of Vin and Vout synchronized with Timer A CMP4 */
  VinConversion = HAL_ADCEx_InjectedGetValue(&AdcHandle, ADC_INJECTED_RANK_1);
  VoutConversion = HAL_ADCEx_InjectedGetValue(&AdcHandle, ADC_INJECTED_RANK_2);
}



/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
