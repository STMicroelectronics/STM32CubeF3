/**
  ******************************************************************************
  * @file    SDADC/SDADC_PressureMeasurement/Src/stm32f3xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F3xx_HAL_Examples
  * @{
  */

/** @defgroup SDADC_PressureMeasurement
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief SDADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock
  *           - Peripheral's GPIO
  *           - Peripheral's TIMER
  * @param hsdadc: SDADC handle pointer
  * @retval None
  */
void HAL_SDADC_MspInit(SDADC_HandleTypeDef *hsdadc)
{
  static TIM_OC_InitTypeDef tim_oc;
  static GPIO_InitTypeDef gpio;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Enable SDADC clock #################################################*/
  /* Configure Clock prescaler : The SDADC should run @6MHz */
  /* If Sysclk is 72MHz, SDADC divider should be 12 */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDADC;
  PeriphClkInitStruct.SdadcClockSelection  = RCC_SDADCSYSCLK_DIV12;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable SDADC clock */
  MPX2102_SDADC_CLK_ENABLE();

  /*##-2- Power On SDADC  ####################################################*/
  HAL_PWREx_EnableSDADC(MPX2102_PWR_SDADC);

  /*##-3- Enable the SDADC interrupt  ########################################*/
  HAL_NVIC_SetPriority(MPX2102_SDADC_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(MPX2102_SDADC_IRQn);

  /*##-4- Configure GPIO  ####################################################*/
  /* Configure MPX2102_GPIO_PIN_P and MPX2102_GPIO_PIN_N in analog mode*/
  MPX2102_GPIO_CLK_ENABLE();
  gpio.Mode  = GPIO_MODE_ANALOG;
  gpio.Pin   = MPX2102_GPIO_PIN_P | MPX2102_GPIO_PIN_N;
  gpio.Speed = GPIO_SPEED_FREQ_LOW;
  gpio.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(MPX2102_SDADC_GPIO_PORT, &gpio);

  /*##-4- Configure TIMER  ###################################################*/
  /* Enable the timer clock */
  MPX2102_TIMER_CLK_ENABLE();

  /* Configure the timer in PWM mode */
  htim.Instance = MPX2102_TIMER;
  htim.Init.ClockDivision = 0;
  htim.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim.Init.Period = 0xFFFF; /* Freq = 72Mhz/65535 = 1.098 KHz */
  htim.Init.Prescaler = 0;
  htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_PWM_Init(&htim);

  tim_oc.OCMode = TIM_OCMODE_PWM1;
  tim_oc.Pulse = 0x7FFF;
  tim_oc.OCPolarity = TIM_OCPOLARITY_HIGH;
  tim_oc.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim, &tim_oc, MPX2102_TIMER_CHANNEL);
}

/**
  * @brief SDADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hsdadc: SDADC handle pointer
  * @retval None
  */
void HAL_SDADC_MspDeInit(SDADC_HandleTypeDef *hsdadc)
{
  /*##-1- Power Off SDADC  ###################################################*/
  HAL_PWREx_DisableSDADC(MPX2102_PWR_SDADC);

  /*##-2- Disable power clock  ###############################################*/
  __HAL_RCC_PWR_CLK_DISABLE();

  /*##-3- Power Off SDADC  ###################################################*/
  MPX2102_GPIO_CLK_DISABLE();

  /*##-4- Disable SDADC clock  ###############################################*/
  MPX2102_SDADC_CLK_DISABLE();

  /*##-5- Disable interrupt  #################################################*/
  HAL_NVIC_DisableIRQ(MPX2102_SDADC_IRQn);

  /*##-6- Disable Timer  #####################################################*/
  MPX2102_TIMER_CLK_DISABLE();
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
