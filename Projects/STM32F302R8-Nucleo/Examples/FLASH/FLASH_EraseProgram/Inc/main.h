/**
  ******************************************************************************
  * @file    FLASH/FLASH_EraseProgram/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32f3xx_nucleo.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Base address of the Flash sectors */
/* Base address of the Flash pages */
#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base address of Page 0, 2 Kbytes */
#define ADDR_FLASH_PAGE_1     ((uint32_t)0x08000800) /* Base address of Page 1, 2 Kbytes */
#define ADDR_FLASH_PAGE_2     ((uint32_t)0x08001000) /* Base address of Page 2, 2 Kbytes */
#define ADDR_FLASH_PAGE_3     ((uint32_t)0x08001800) /* Base address of Page 3, 2 Kbytes */
#define ADDR_FLASH_PAGE_4     ((uint32_t)0x08002000) /* Base address of Page 4, 2 Kbytes */
#define ADDR_FLASH_PAGE_5     ((uint32_t)0x08002800) /* Base address of Page 5, 2 Kbytes */
#define ADDR_FLASH_PAGE_6     ((uint32_t)0x08003000) /* Base address of Page 6, 2 Kbytes */
#define ADDR_FLASH_PAGE_7     ((uint32_t)0x08003800) /* Base address of Page 7, 2 Kbytes */
#define ADDR_FLASH_PAGE_8     ((uint32_t)0x08004000) /* Base address of Page 8, 2 Kbytes */
#define ADDR_FLASH_PAGE_9     ((uint32_t)0x08004800) /* Base address of Page 9, 2 Kbytes */
#define ADDR_FLASH_PAGE_10    ((uint32_t)0x08005000) /* Base address of Page 10, 2 Kbytes */
#define ADDR_FLASH_PAGE_11    ((uint32_t)0x08005800) /* Base address of Page 11, 2 Kbytes */
#define ADDR_FLASH_PAGE_12    ((uint32_t)0x08006000) /* Base address of Page 12, 2 Kbytes */
#define ADDR_FLASH_PAGE_13    ((uint32_t)0x08006800) /* Base address of Page 13, 2 Kbytes */
#define ADDR_FLASH_PAGE_14    ((uint32_t)0x08007000) /* Base address of Page 14, 2 Kbytes */
#define ADDR_FLASH_PAGE_15    ((uint32_t)0x08007800) /* Base address of Page 15, 2 Kbytes */
#define ADDR_FLASH_PAGE_16    ((uint32_t)0x08008000) /* Base address of Page 16, 2 Kbytes */
#define ADDR_FLASH_PAGE_17    ((uint32_t)0x08008800) /* Base address of Page 17, 2 Kbytes */
#define ADDR_FLASH_PAGE_18    ((uint32_t)0x08009000) /* Base address of Page 18, 2 Kbytes */
#define ADDR_FLASH_PAGE_19    ((uint32_t)0x08009800) /* Base address of Page 19, 2 Kbytes */
#define ADDR_FLASH_PAGE_20    ((uint32_t)0x0800A000) /* Base address of Page 20, 2 Kbytes */
#define ADDR_FLASH_PAGE_21    ((uint32_t)0x0800A800) /* Base address of Page 21, 2 Kbytes  */
#define ADDR_FLASH_PAGE_22    ((uint32_t)0x0800B000) /* Base address of Page 22, 2 Kbytes  */
#define ADDR_FLASH_PAGE_23    ((uint32_t)0x0800B800) /* Base address of Page 23, 2 Kbytes */
#define ADDR_FLASH_PAGE_24    ((uint32_t)0x0800C000) /* Base address of Page 24, 2 Kbytes */
#define ADDR_FLASH_PAGE_25    ((uint32_t)0x0800C800) /* Base address of Page 25, 2 Kbytes */
#define ADDR_FLASH_PAGE_26    ((uint32_t)0x0800D000) /* Base address of Page 26, 2 Kbytes */
#define ADDR_FLASH_PAGE_27    ((uint32_t)0x0800D800) /* Base address of Page 27, 2 Kbytes */
#define ADDR_FLASH_PAGE_28    ((uint32_t)0x0800E000) /* Base address of Page 28, 2 Kbytes */
#define ADDR_FLASH_PAGE_29    ((uint32_t)0x0800E800) /* Base address of Page 29, 2 Kbytes */
#define ADDR_FLASH_PAGE_30    ((uint32_t)0x0800F000) /* Base address of Page 30, 2 Kbytes */
#define ADDR_FLASH_PAGE_31    ((uint32_t)0x0800F800) /* Base address of Page 31, 2 Kbytes */

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
