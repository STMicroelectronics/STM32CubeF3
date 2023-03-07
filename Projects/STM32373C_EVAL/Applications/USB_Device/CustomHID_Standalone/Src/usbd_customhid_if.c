/**
  ******************************************************************************
  * @file    USB_Device/CustomHID_Standalone/Src/usbd_customhid_if.c
  * @author  MCD Application Team
  * @brief   USB Device Custom HID interface file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_customhid_if.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static int8_t CustomHID_Init     (void);
static int8_t CustomHID_DeInit   (void);
static int8_t CustomHID_OutEvent (uint8_t event_idx, uint8_t state);
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef  AdcHandle;
uint32_t ADCConvertedValue = 0;
uint32_t ADC_Prev_ConvertedValue = 0;
uint8_t SendBuffer[2];
extern USBD_HandleTypeDef hUSBDDevice;

__ALIGN_BEGIN static uint8_t CustomHID_ReportDesc[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  0x06, 0xFF, 0x00,      /* USAGE_PAGE (Vendor Page: 0xFF00) */                       
  0x09, 0x01,            /* USAGE (Demo Kit)               */    
  0xa1, 0x01,            /* COLLECTION (Application)       */            
  /* 6 */
  
  /* Led 1 */        
  0x85, LED1_REPORT_ID,  /*     REPORT_ID (1)		     */
  0x09, 0x01,            /*     USAGE (LED 1)	             */
  0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */          
  0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */           
  0x75, 0x08,            /*     REPORT_SIZE (8)            */        
  0x95, LED1_REPORT_COUNT, /*     REPORT_COUNT (1)           */       
  0xB1, 0x82,             /*    FEATURE (Data,Var,Abs,Vol) */     
  
  0x85, LED1_REPORT_ID,  /*     REPORT_ID (1)              */
  0x09, 0x01,            /*     USAGE (LED 1)              */
  0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
  /* 26 */
  
  /* Led 2 */
  0x85, LED2_REPORT_ID,  /*     REPORT_ID 2		     */
  0x09, 0x02,            /*     USAGE (LED 2)	             */
  0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */          
  0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */           
  0x75, 0x08,            /*     REPORT_SIZE (8)            */        
  0x95, LED2_REPORT_COUNT, /*     REPORT_COUNT (1)           */       
  0xB1, 0x82,             /*    FEATURE (Data,Var,Abs,Vol) */     
  
  0x85, LED2_REPORT_ID,  /*     REPORT_ID (2)              */
  0x09, 0x02,            /*     USAGE (LED 2)              */
  0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
  /* 46 */
  
  /* Led 3 */        
  0x85, LED3_REPORT_ID,  /*     REPORT_ID (3)		     */
  0x09, 0x03,            /*     USAGE (LED 3)	             */
  0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */          
  0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */           
  0x75, 0x08,            /*     REPORT_SIZE (8)            */        
  0x95, LED3_REPORT_COUNT, /*     REPORT_COUNT (1)           */       
  0xB1, 0x82,             /*    FEATURE (Data,Var,Abs,Vol) */     
  
  0x85, LED3_REPORT_ID,  /*     REPORT_ID (3)              */
  0x09, 0x03,            /*     USAGE (LED 3)              */
  0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
  /* 66 */
  
  /* Led 4 */
  0x85, LED4_REPORT_ID,  /*     REPORT_ID 4)		     */
  0x09, 0x04,            /*     USAGE (LED 4)	             */
  0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */          
  0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */           
  0x75, 0x08,            /*     REPORT_SIZE (8)            */        
  0x95, LED4_REPORT_COUNT, /*     REPORT_COUNT (1)           */       
  0xB1, 0x82,            /*     FEATURE (Data,Var,Abs,Vol) */     
  
  0x85, LED4_REPORT_ID,  /*     REPORT_ID (4)              */
  0x09, 0x04,            /*     USAGE (LED 4)              */
  0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
  /* 86 */
  
  /* key Push Button */  
  0x85, KEY_REPORT_ID,   /*     REPORT_ID (5)              */
  0x09, 0x05,            /*     USAGE (Push Button)        */      
  0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */      
  0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */      
  0x75, 0x01,            /*     REPORT_SIZE (1)            */  
  0x81, 0x82,            /*     INPUT (Data,Var,Abs,Vol)   */   
  
  0x09, 0x05,            /*     USAGE (Push Button)        */               
  0x75, 0x01,            /*     REPORT_SIZE (1)            */           
  0xb1, 0x82,            /*     FEATURE (Data,Var,Abs,Vol) */  
  
  0x75, 0x07,            /*     REPORT_SIZE (7)            */           
  0x81, 0x83,            /*     INPUT (Cnst,Var,Abs,Vol)   */                    
  0x85, KEY_REPORT_ID,   /*     REPORT_ID (2)              */         
  
  0x75, 0x07,            /*     REPORT_SIZE (7)            */           
  0xb1, 0x83,            /*     FEATURE (Cnst,Var,Abs,Vol) */                      
  /* 114 */
  
  /* Tamper Push Button */  
  0x85, TAMPER_REPORT_ID,/*     REPORT_ID (6)              */
  0x09, 0x06,            /*     USAGE (Tamper Push Button) */      
  0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */      
  0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */      
  0x75, 0x01,            /*     REPORT_SIZE (1)            */  
  0x81, 0x82,            /*     INPUT (Data,Var,Abs,Vol)   */   
  
  0x09, 0x06,            /*     USAGE (Tamper Push Button) */               
  0x75, 0x01,            /*     REPORT_SIZE (1)            */           
  0xb1, 0x82,            /*     FEATURE (Data,Var,Abs,Vol) */  
  
  0x75, 0x07,            /*     REPORT_SIZE (7)            */           
  0x81, 0x83,            /*     INPUT (Cnst,Var,Abs,Vol)   */                    
  0x85, TAMPER_REPORT_ID,/*     REPORT_ID (6)              */         
  
  0x75, 0x07,            /*     REPORT_SIZE (7)            */           
  0xb1, 0x83,            /*     FEATURE (Cnst,Var,Abs,Vol) */  
  /* 142 */
  
  /* ADC IN */
  0x85, ADC_REPORT_ID,   /*     REPORT_ID                 */         
  0x09, 0x07,            /*     USAGE (ADC IN)             */          
  0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */               
  0x26, 0xff, 0x00,      /*     LOGICAL_MAXIMUM (255)      */                 
  0x75, 0x08,            /*     REPORT_SIZE (8)            */           
  0x81, 0x82,            /*     INPUT (Data,Var,Abs,Vol)   */                    
  0x85, ADC_REPORT_ID,   /*     REPORT_ID (7)              */                 
  0x09, 0x07,            /*     USAGE (ADC in)             */                     
  0xb1, 0x82,            /*     FEATURE (Data,Var,Abs,Vol) */                                 
  /* 161 */
  
  0xc0 	                 /*     END_COLLECTION	             */
}; 

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops = 
{
  CustomHID_ReportDesc,
  CustomHID_Init,
  CustomHID_DeInit,
  CustomHID_OutEvent,
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  CustomHID_Init
  *         Initializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CustomHID_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  ADC_ChannelConfTypeDef sConfig;
  
  /* Configure the ADC peripheral */
  AdcHandle.Instance = ADCx;
  
  /* Configure the ADC clock */
  __HAL_RCC_ADC1_CONFIG(RCC_ADC1PCLK2_DIV6);
  
  AdcHandle.Init.ScanConvMode = DISABLE;
  AdcHandle.Init.ContinuousConvMode = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfDiscConversion = 0;
  AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&AdcHandle);
   
  /* Configure ADC regular channel */  
  sConfig.Channel = ADCx_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
  HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

  /* Start the conversion process and enable interrupt */  
  HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)&ADCConvertedValue, 1);
    
  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  
  /* Enable GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /* Configure PA2 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable and set EXTI2_TSC Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);
  
  return (0);
}

/**
  * @brief  CustomHID_DeInit
  *         DeInitializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CustomHID_DeInit(void)
{
  /*
  Add your deinitialization code here 
  */  
  return (0);
}


/**
  * @brief  CustomHID_OutEvent
  *         Manage the CUSTOM HID class Out Event    
  * @param  event_idx: LED Report Number
  * @param  state: LED states (ON/OFF)
  */
static int8_t CustomHID_OutEvent  (uint8_t event_idx, uint8_t state)
{ 
  switch(event_idx)
  {
  case 1: /* LED1 */
    (state == 1) ? BSP_LED_On(LED1) : BSP_LED_Off(LED1); 
    break;
    
  case 2: /* LED2 */
    (state == 1) ? BSP_LED_On(LED2) : BSP_LED_Off(LED2); 
    break;
  case 3: /* LED3 */
    (state == 1) ? BSP_LED_On(LED3) : BSP_LED_Off(LED3); 
    break;
  case 4: /* LED4 */
    (state == 1) ? BSP_LED_On(LED4) : BSP_LED_Off(LED4); 
    break;
    
  default:
    BSP_LED_Off(LED1);
    BSP_LED_Off(LED2);
    BSP_LED_Off(LED3);
    BSP_LED_Off(LED4); 
    break;
  }
  return (0);
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == KEY_BUTTON_PIN)
  {
    SendBuffer[0] = KEY_REPORT_ID; 
    
    if(BSP_PB_GetState(BUTTON_KEY) == GPIO_PIN_RESET)
    {
      SendBuffer[1] = 0x01;
    }
    else
    {
      SendBuffer[1] = 0x00;
    }
    USBD_CUSTOM_HID_SendReport(&hUSBDDevice, SendBuffer, 2);
  }
}
