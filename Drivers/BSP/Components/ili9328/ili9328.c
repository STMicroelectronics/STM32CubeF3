/**
  ******************************************************************************
  * @file    ili9328.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    21-December-2015
  * @brief   This file includes the LCD driver for ILI9328 LCD.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
#include "ili9328.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup ILI9328
  * @brief     This file provides a set of functions needed to drive the 
  *            ILI9328 LCD.
  * @{
  */

/** @defgroup ILI9328_Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup ILI9328_Private_Defines
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup ILI9328_Private_Macros
  * @{
  */
     
/**
  * @}
  */  

/** @defgroup ILI9328_Private_Variables
  * @{
  */ 
LCD_DrvTypeDef   ili9328_drv = 
{
  ili9328_Init,
  ili9328_ReadID,
  ili9328_DisplayOn,
  ili9328_DisplayOff,
  ili9328_SetCursor,
  ili9328_WritePixel,
  ili9328_ReadPixel,
  ili9328_SetDisplayWindow,
  ili9328_DrawHLine,
  ili9328_DrawVLine,
  ili9328_GetLcdPixelWidth,
  ili9328_GetLcdPixelHeight,
  ili9328_DrawBitmap,  
};

static uint8_t Is_ili9328_Initialized = 0;
static uint16_t ArrayRGB[320] = {0};

/**
  * @}
  */ 
  
/** @defgroup ILI9328_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup ILI9328_Private_Functions
  * @{
  */   

/**
  * @brief  Initialise the ILI9328 LCD Component.
  * @param  None
  * @retval None
  */
void ili9328_Init(void)
{  
  if(Is_ili9328_Initialized == 0)
  {
    Is_ili9328_Initialized = 1;
    /* Initialise ILI9328 low level bus layer --------------------------------*/
    LCD_IO_Init();
    
    /* Start Initial Sequence ------------------------------------------------*/
    ili9328_WriteReg(LCD_REG_0, 0x0001); /* Start internal OSC. */
    ili9328_WriteReg(LCD_REG_1, 0x0100); /* Set SS and SM bit */
    ili9328_WriteReg(LCD_REG_2, 0x0700); /* Set 1 line inversion */
    ili9328_WriteReg(LCD_REG_3, 0x1018); /* Set GRAM write direction and BGR=1. */
    ili9328_WriteReg(LCD_REG_4, 0x0000); /* Resize register */
    ili9328_WriteReg(LCD_REG_8, 0x0302); /* Set the back porch and front porch */
    ili9328_WriteReg(LCD_REG_9, 0x0000); /* Set non-display area refresh cycle ISC[3:0] */
    ili9328_WriteReg(LCD_REG_10, 0x0000); /* FMARK function */
    ili9328_WriteReg(LCD_REG_12, 0x0000); /* RGB interface setting */
    ili9328_WriteReg(LCD_REG_13, 0x0000); /* Frame marker Position */
    ili9328_WriteReg(LCD_REG_15, 0x0000); /* RGB interface polarity */
    
    ili9328_DisplayOn();
    
    /* Adjust the Gamma Curve (ILI9328)---------------------------------------*/
    ili9328_WriteReg(LCD_REG_48, 0x0000);
    ili9328_WriteReg(LCD_REG_49, 0x0107);
    ili9328_WriteReg(LCD_REG_50, 0x0000);
    ili9328_WriteReg(LCD_REG_53, 0x0203);
    ili9328_WriteReg(LCD_REG_54, 0x0402);
    ili9328_WriteReg(LCD_REG_55, 0x0000);
    ili9328_WriteReg(LCD_REG_56, 0x0207);
    ili9328_WriteReg(LCD_REG_57, 0x0000);
    ili9328_WriteReg(LCD_REG_60, 0x0203);
    ili9328_WriteReg(LCD_REG_61, 0x0403);
    
    /* Set GRAM area ---------------------------------------------------------*/
    ili9328_SetDisplayWindow(0, 0, ili9328_GetLcdPixelWidth(), ili9328_GetLcdPixelHeight());
    
    ili9328_WriteReg(LCD_REG_96,  0xA700); /* Gate Scan Line(GS=1, scan direction is G320~G1) */
    ili9328_WriteReg(LCD_REG_97,  0x0003); /* NDL,VLE, REV */
    ili9328_WriteReg(LCD_REG_106, 0x0000); /* set scrolling line */
    
    /* Partial Display Control -----------------------------------------------*/
    ili9328_WriteReg(LCD_REG_128, 0x0000);
    ili9328_WriteReg(LCD_REG_129, 0x0000);
    ili9328_WriteReg(LCD_REG_130, 0x0000);
    ili9328_WriteReg(LCD_REG_131, 0x0000);
    ili9328_WriteReg(LCD_REG_132, 0x0000);
    ili9328_WriteReg(LCD_REG_133, 0x0000);
    
    /* Panel Control ---------------------------------------------------------*/
    ili9328_WriteReg(LCD_REG_144, 0x0010);
    ili9328_WriteReg(LCD_REG_146, 0x0000);
    ili9328_WriteReg(LCD_REG_147, 0x0003);
    ili9328_WriteReg(LCD_REG_149, 0x0110);
    ili9328_WriteReg(LCD_REG_151, 0x0000);
    ili9328_WriteReg(LCD_REG_152, 0x0000);
    
    /* set GRAM write direction and BGR = 1 */
    /* I/D=00 (Horizontal : increment, Vertical : decrement) */
    /* AM=1 (address is updated in vertical writing direction) */
    ili9328_WriteReg(LCD_REG_3, 0x1018);
  }
  
  /* Set the Cursor */ 
  ili9328_SetCursor(0, 0);
    
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void ili9328_DisplayOn(void)
{
  /* Power On sequence ---------------------------------------------------------*/
  ili9328_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  ili9328_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  ili9328_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
  ili9328_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude */
  LCD_Delay(20);
  ili9328_WriteReg(LCD_REG_16, 0x14B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_Delay(5);
  ili9328_WriteReg(LCD_REG_17, 0x0007); /* DC1[2:0], DC0[2:0], VC[2:0] */
  LCD_Delay(5);
  ili9328_WriteReg(LCD_REG_18, 0x0018); /* VREG1OUT voltage */
  LCD_Delay(5);    
  ili9328_WriteReg(LCD_REG_19, 0x1000); /* VDV[4:0] for VCOM amplitude */
  ili9328_WriteReg(LCD_REG_41, 0x0015); /* VCM[4:0] for VCOMH */
  LCD_Delay(5);    
 
  /* Display On */
  ili9328_WriteReg(LCD_REG_7, 0x0133); /* 262K color and display ON */
  LCD_Delay(50);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void ili9328_DisplayOff(void)
{
  /* Display Off */
  ili9328_WriteReg(LCD_REG_7, 0x0); 

  /* Power Off sequence ---------------------------------------------------------*/
  ili9328_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  ili9328_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  ili9328_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
  ili9328_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude*/
  
  ili9328_WriteReg(LCD_REG_41, 0x0000); /* VCM[4:0] for VCOMH */
  
}

/**
  * @brief  Get the LCD pixel Width.
  * @param  None
  * @retval The Lcd Pixel Width
  */
uint16_t ili9328_GetLcdPixelWidth(void)
{
 return (uint16_t)ILI9328_LCD_PIXEL_WIDTH;
}

/**
  * @brief  Get the LCD pixel Height.
  * @param  None
  * @retval The Lcd Pixel Height
  */
uint16_t ili9328_GetLcdPixelHeight(void)
{
 return (uint16_t)ILI9328_LCD_PIXEL_HEIGHT;
}

/**
  * @brief  Get the ILI9328 ID.
  * @param  None
  * @retval The ILI9328 ID 
  */
uint16_t ili9328_ReadID(void)
{
  if(Is_ili9328_Initialized == 0)
  {
    LCD_IO_Init();
  }
  return (ili9328_ReadReg(0x00));
}

/**
  * @brief  Set Cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void ili9328_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  ili9328_WriteReg(LCD_REG_32, Xpos);
  ili9328_WriteReg(LCD_REG_33, Ypos);      
}

/**
  * @brief  Write pixel.   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
* @param  RGBCode: the RGB pixel color
  * @retval None
  */
void ili9328_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  /* Set Cursor */
  ili9328_SetCursor(Xpos, Ypos);
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);

  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData((uint8_t*)&RGBCode, 2);
}

/**
  * @brief  Read pixel.
  * @param  None
  * @retval the RGB pixel color
  */
uint16_t ili9328_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  /* Set Cursor */
  ili9328_SetCursor(Xpos, Ypos);
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
  
  /* Dummy read */
  LCD_IO_ReadData(LCD_REG_34);
  
  /* Read 16-bit Reg */
  return (LCD_IO_ReadData(LCD_REG_34));
}

/**
  * @brief  Writes to the selected LCD register.
* @param  LCDReg:      address of the selected register.
* @param  LCDRegValue: value to write to the selected register.
  * @retval None
  */
void ili9328_WriteReg(uint8_t LCDReg, uint16_t LCDRegValue)
{
  LCD_IO_WriteReg(LCDReg);
  
  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData((uint8_t*)&LCDRegValue, 2);
}

/**
  * @brief  Reads the selected LCD Register.
* @param  LCDReg: address of the selected register.
  * @retval LCD Register Value.
  */
uint16_t ili9328_ReadReg(uint8_t LCDReg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD_IO_WriteReg(LCDReg);
  
  /* Read 16-bit Reg */
  return (LCD_IO_ReadData(LCDReg));
}

/**
  * @brief  Sets a display window
  * @param  Xpos:   specifies the X bottom left position.
  * @param  Ypos:   specifies the Y bottom left position.
  * @param  Height: display window height.
  * @param  Width:  display window width.
  * @retval None
  */
void ili9328_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Horizontal GRAM Start Address */
  ili9328_WriteReg(LCD_REG_80, (Xpos));
  /* Horizontal GRAM End Address */
  ili9328_WriteReg(LCD_REG_81, (Xpos + Height - 1));
  
  /* Vertical GRAM Start Address */
  ili9328_WriteReg(LCD_REG_82, (Ypos));
  /* Vertical GRAM End Address */
  ili9328_WriteReg(LCD_REG_83, (Ypos + Width - 1));  
}

/**
  * @brief  Draw vertical line.
* @param  RGBCode: Specifies the RGB color   
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  Length:   specifies the Line length.  
  * @retval None
  */
void ili9328_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t i = 0;
  
  /* Set Cursor */
  ili9328_SetCursor(Xpos, Ypos); 
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);

  /* Sent a complete line */
  for(i = 0; i < Length; i++)
  {
    ArrayRGB[i] = RGBCode;
  }  

  LCD_IO_WriteMultipleData((uint8_t*)&ArrayRGB[0], Length * 2);
}

/**
  * @brief  Draw vertical line.
* @param  RGBCode: Specifies the RGB color    
  * @param  Xpos:     specifies the X position.
  * @param  Ypos:     specifies the Y position.
  * @param  Length:   specifies the Line length.  
  * @retval None
  */
void ili9328_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t counter = 0;

  /* Set Cursor */
  ili9328_SetCursor(Xpos, Ypos);
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);

  /* Fill a complete vertical line */
  for(counter = 0; counter < Length; counter++)
  {
    ArrayRGB[counter] = RGBCode;
  }
  
  /* Write 16-bit GRAM Reg */
  LCD_IO_WriteMultipleData((uint8_t*)&ArrayRGB[0], Length * 2);
}

/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  */
void ili9328_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
  uint32_t index = 0, size = 0;
  
  /* Read bitmap size */
  size = *(volatile uint16_t *) (pbmp + 2);
  size |= (*(volatile uint16_t *) (pbmp + 4)) << 16;
  /* Get bitmap data address offset */
  index = *(volatile uint16_t *) (pbmp + 10);
  index |= (*(volatile uint16_t *) (pbmp + 12)) << 16;
  size = (size - index)/2;
  pbmp += index;

  /* Set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : decrement, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  ili9328_WriteReg(LCD_REG_3, 0x1008);

  /* Set Cursor */
  ili9328_SetCursor(Xpos, Ypos);  
  
  /* Prepare to write GRAM */
  LCD_IO_WriteReg(LCD_REG_34);
 
  LCD_IO_WriteMultipleData((uint8_t*)pbmp, size*2);
 
  /* Set GRAM write direction and BGR = 1 */
  /* I/D = 01 (Horizontal : increment, Vertical : decrement) */
  /* AM = 1 (address is updated in vertical writing direction) */
  ili9328_WriteReg(LCD_REG_3, 0x1018);
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

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
