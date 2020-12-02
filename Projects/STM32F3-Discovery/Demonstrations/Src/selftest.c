/**
  ******************************************************************************
  * @file    Demonstrations/Src/selftest.c
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

/** @addtogroup STM32F3-Discovery_Demo STM32F3-Discovery_Demo
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "selftest.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern  __IO uint8_t UserButtonPressed;
extern USBD_HandleTypeDef USBD_Device;
/* Init af threshold to detect acceleration on MEMS */
int16_t ThresholdHigh = 2500;
int16_t ThresholdLow = -2500;
/* Private function prototypes -----------------------------------------------*/
static void USB_GetPointerData_Test(uint8_t *pbuf);
static void USB_GetPointerData_Demo(uint8_t *pbuf);
static void Demo_GyroReadAngRate (float* pfData);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test ACCELERATOR MEMS Hardware.
  *   The main objective of this test is to check acceleration on 2 axis X and Y
* @param  None
* @retval None
*/
void ACCELERO_MEMS_Test(void)
{
  int16_t buffer[3] = {0};
  int16_t xval, yval = 0x00;

  __disable_irq();
  /* Read Acceleration*/
  BSP_ACCELERO_GetXYZ(buffer);
  __enable_irq();

  /* Update autoreload and capture compare registers value*/
  xval = buffer[0];
  yval = buffer[1];

  if((ABS(xval))>(ABS(yval)))
  {
    if(xval > ThresholdHigh)
    {
      /* LED7 On */
      BSP_LED_On(LED7);
      HAL_Delay(10);
    }
    else if(xval < ThresholdLow)
    {
      /* LED6 On */
      BSP_LED_On(LED6);
      HAL_Delay(10);
    }
    else
      {
      HAL_Delay(10);
      }
    }
  else
  {
    if(yval < ThresholdLow)
    {
      /* LED3 On */
      BSP_LED_On(LED3);
      HAL_Delay(10);
    }
    else if(yval > ThresholdHigh)
    {
      /* LED10 On */
      BSP_LED_On(LED10);
      HAL_Delay(10);
    }
    else
    {
      HAL_Delay(10);
    }
  }
    BSP_LED_Off(LED3);
    BSP_LED_Off(LED6);
    BSP_LED_Off(LED7);
    BSP_LED_Off(LED4);
    BSP_LED_Off(LED10);
    BSP_LED_Off(LED8);
    BSP_LED_Off(LED9);
    BSP_LED_Off(LED5);
}

/**
  * @brief Test GYROSCOPE MEMS Hardware.
  *   The main objectif of this test is to check the hardware connection of the
  *   MEMS peripheral.
  * @param None
* @retval None
*/
void GYRO_MEMS_Test(void)
{
  /* Gyroscope variable */
  float Buffer[3];
  float Xval,Yval = 0x00;

  /* Read Gyro Angular data */
  BSP_GYRO_GetXYZ(Buffer);

  /* Update autoreload and capture compare registers value*/
  Xval = ABS((Buffer[0]));
  Yval = ABS((Buffer[1]));

  if(Xval>Yval)
  {
    if(Buffer[0] > 5000.0f)
    {
      /* LD10 On */
      BSP_LED_On(LED10);
      HAL_Delay(10);
    }
    else if(Buffer[0] < -5000.0f)
    {
      /* LED3 On */
      BSP_LED_On(LED3);
      HAL_Delay(10);
    }
    else
    {
      HAL_Delay(10);
    }
  }
  else
  {
    if(Buffer[1] < -5000.0f)
    {
        /* LD6 on */
        BSP_LED_On(LED6);
        HAL_Delay(10);
    }
    else if(Buffer[1] > 5000.0f)
    {
        /* LD7 On */
        BSP_LED_On(LED7);
        HAL_Delay(10);
    }
    else
    {
      HAL_Delay(10);
    }
  }
    BSP_LED_Off(LED3);
    BSP_LED_Off(LED6);
    BSP_LED_Off(LED7);
    BSP_LED_Off(LED4);
    BSP_LED_Off(LED10);
    BSP_LED_Off(LED8);
    BSP_LED_Off(LED9);
    BSP_LED_Off(LED5);
}

/**
  * @brief  USB Test : Configure the USB
  * @param  None
  * @retval None
  */
void USB_Test(void)
{
  uint8_t HID_Buffer[4];

  while ((BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET))
  {
    USB_GetPointerData_Test(HID_Buffer);

    /* send data though IN endpoint*/
    if((HID_Buffer[1] != 0) || (HID_Buffer[2] != 0))
    {
      USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
      HAL_Delay (50);
    }
  }

  /* Wait for User button is released */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {}

  /* Turn Off Leds */
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED6);
  BSP_LED_Off(LED7);
  BSP_LED_Off(LED10);
}

/**
  * @brief  USBD_HID_GetPos
  * @param  None
  * @retval Pointer to report
*/
static void USB_GetPointerData_Test(uint8_t *pbuf)
{
  static int8_t x = 0;
  static int8_t y = 0;
  static int8_t Sens = 0;
  static int8_t Pas = 0;

  if (Pas == 20)
  {
    Pas=0;
    Sens++;
  }

  if(Sens == 0)
  {
    x=Pas++;
    y=0;
    BSP_LED_Toggle(LED3);
  }
  if(Sens == 1)
  {
    y=Pas++;
    x=0;
    BSP_LED_Toggle(LED7);
  }
  if (Sens == 2)
  {
    x=256-Pas++;
    y=0;
    BSP_LED_Toggle(LED10);
  }
  if (Sens == 3)
  {
    y=256-Pas++;
    x=0;
    BSP_LED_Toggle(LED6);
  }
  if (Sens == 4)
  {
    Sens=0;
    x=0;
    y=0;
  }

  pbuf[0] = 0;
  pbuf[1] = x;
  pbuf[2] = y;
  pbuf[3] = 0;

}

/**
  * @brief  USB Mouse cursor moving
  * @param  None
  * @retval None
  */
void USB_Demo(void)
{
  uint8_t HID_Buffer[4];

  BSP_LED_On(LED3);
  BSP_LED_On(LED6);
  BSP_LED_Off(LED10);
  BSP_LED_Off(LED7);

  while ((BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET))
  {
    USB_GetPointerData_Demo(HID_Buffer);

    /* send data though IN endpoint*/
    if((HID_Buffer[1] != 0) || (HID_Buffer[2] != 0))
    {
      USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
    }
  }

  /* Turn Off Leds */
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED7);
  BSP_LED_Off(LED9);
  BSP_LED_Off(LED10);
  BSP_LED_Off(LED8);
  BSP_LED_Off(LED6);
}

/**
  * @brief  USBD_HID_GetPos
  * @param  None
  * @retval Pointer to report
  */
void USB_GetPointerData_Demo(uint8_t *pbuf)
{
  static float Buffer[6] = {0};

  /* Read Gyro Angular data */
  Demo_GyroReadAngRate(Buffer);

  pbuf[0] = 0;
  pbuf[1] = -(int8_t)(Buffer[2]) / 15;
  pbuf[2] =  (int8_t)(Buffer[1]) / 15;
  pbuf[3] = 0;

  BSP_LED_Toggle(LED3);
  BSP_LED_Toggle(LED10);
  BSP_LED_Toggle(LED6);
  BSP_LED_Toggle(LED7);
}

/**
  * @brief  Calculate the angular Data rate Gyroscope.
  * @param  pfData : Data out pointer
  * @retval None
  */
static void Demo_GyroReadAngRate (float* pfData)
{
  uint8_t tmpbuffer[6] ={0};
  int16_t RawData[3] = {0};
  uint8_t tmpreg = 0;
  float sensitivity = 0;
  int i =0;

  GYRO_IO_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);

  GYRO_IO_Read(tmpbuffer,L3GD20_OUT_X_L_ADDR,6);

  /* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
  if(!(tmpreg & L3GD20_BLE_MSB))
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i+1] << 8) + tmpbuffer[2*i]);
    }
  }
  else
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i] << 8) + tmpbuffer[2*i+1]);
    }
  }

  /* Switch the sensitivity value set in the CRTL4 */
  switch(tmpreg & L3GD20_FULLSCALE_SELECTION)
  {
  case L3GD20_FULLSCALE_250:
    sensitivity=L3GD20_SENSITIVITY_250DPS;
    break;

  case L3GD20_FULLSCALE_500:
    sensitivity=L3GD20_SENSITIVITY_500DPS;
    break;

  case L3GD20_FULLSCALE_2000:
    sensitivity=L3GD20_SENSITIVITY_2000DPS;
    break;

    default:
      sensitivity=L3GD20_SENSITIVITY_250DPS;
  }
  /* Divide by sensitivity */
  for(i=0; i<3; i++)
  {
    pfData[i]=(float)(RawData[i] / sensitivity);
  }
}


/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
