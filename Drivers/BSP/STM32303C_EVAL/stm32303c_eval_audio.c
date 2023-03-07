/**
  ******************************************************************************
  * @file    stm32303c_eval_audio.c
  * @author  MCD Application Team
  * @brief   This file provides the Audio driver for the STM32303C_EVAL 
  *          evaluation board(MB1019).  
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/*==============================================================================
                                             User NOTES
How To use this driver:
-----------------------
   + This driver supports STM32F30x devices on STM32303C_EVAL Evaluation boards:
   + Call the function BSP_AUDIO_OUT_Init(
                                    OutputDevice: physical output mode (OUTPUT_DEVICE_SPEAKER, 
                                                 OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_AUTO or 
                                                 OUTPUT_DEVICE_BOTH)
                                    Volume: initial volume to be set (0 is min (mute), 100 is max (100%)
                                    AudioFreq: Audio frequency in Hz (8000, 16000, 22500, 32000 ...)
                                     this parameter is relative to the audio file/stream type.
                                   )
      This function configures all the hardware required for the audio application (codec, I2C, I2S, 
      GPIOs, DMA and interrupt if needed). This function returns 0 if configuration is OK.
      if the returned value is different from 0 or the function is stuck then the communication with
      the codec has failed (try to un-plug the power or reset device in this case).
      - OUTPUT_DEVICE_SPEAKER: only speaker will be set as output for the audio stream.
      - OUTPUT_DEVICE_HEADPHONE: only headphones will be set as output for the audio stream.
      - OUTPUT_DEVICE_AUTO: Selection of output device is made through external switch (implemented 
         into the audio jack on the evaluation board). When the Headphone is connected it is used
         as output. When the headphone is disconnected from the audio jack, the output is
         automatically switched to Speaker.
      - OUTPUT_DEVICE_BOTH: both Speaker and Headphone are used as outputs for the audio stream
         at the same time.
   + Call the function BSP_AUDIO_OUT_Play(
                                  pBuffer: pointer to the audio data file address
                                  Size: size of the buffer to be sent in Bytes
                                 )
      to start playing (for the first time) from the audio file/stream.
   + Call the function BSP_AUDIO_OUT_Pause() to pause playing 
   + Call the function BSP_AUDIO_OUT_Resume() to resume playing.
       Note. After calling BSP_AUDIO_OUT_Pause() function for pause, only BSP_AUDIO_OUT_Resume() should be called
          for resume (it is not allowed to call BSP_AUDIO_OUT_Play() in this case).
       Note. This function should be called only when the audio file is played or paused (not stopped).
   + For each mode, you may need to implement the relative callback functions into your code.
      The Callback functions are named BSP_AUDIO_OUT_XXX_CallBack() and only their prototypes are declared in 
      the stm32303c_eval_audio.h file. (refer to the example for more details on the callbacks implementations)
   + To Stop playing, to modify the volume level or to mute, use the functions
       BSP_AUDIO_OUT_Stop(), BSP_AUDIO_OUT_SetVolume(), BSP_AUDIO_OUT_SetFrequency(), BSP_AUDIO_OUT_SetOutputMode() and BSP_AUDIO_OUT_SetMute().
   + The driver API and the callback functions are at the end of the stm32303c_eval_audio.h file.

   Driver architecture:
   -------------------
   + This driver provide the High Audio Layer: consists of the function API exported in the stm32303c_eval_audio.h file
       (BSP_AUDIO_OUT_Init(), BSP_AUDIO_OUT_Play() ...)
+ This driver provide also the Media Access Layer (MAL): which consists of functions allowing to access the media containing/
       providing the audio file/stream. These functions are also included as local functions into
       the stm32303c_eval_audio.c file (I2Sx_MspInit() and I2Sx_Init())

 Known Limitations:
-------------------
   1- When using the Speaker, if the audio file quality is not high enough, the speaker output
      may produce high and uncomfortable noise level. To avoid this issue, to use speaker
      output properly, try to increase audio file sampling rate (typically higher than 48KHz).
      This operation will lead to larger file size.
   2- Communication with the audio codec (through I2C) may be corrupted if it is interrupted by some
      user interrupt routines (in this case, interrupts could be disabled just before the start of 
      communication then re-enabled when it is over). Note that this communication is only done at
    the configuration phase (BSP_AUDIO_OUT_Init() or BSP_AUDIO_OUT_Stop()) and when Volume control modification is 
      performed (BSP_AUDIO_OUT_SetVolume() or AUDIO_OUT_Mute() or BSP_AUDIO_OUT_SetOutputMode()).
      When the audio data is played, no communication is required with the audio codec.
  3- Parsing of audio file is not implemented (in order to determine audio file properties: Mono/Stereo, Data size, 
     File size, Audio Frequency, Audio Data header size ...). The configuration is fixed for the given audio file.
  4- Mono audio streaming is not supported (in order to play mono audio streams, each data should be sent twice 
     on the I2S or should be duplicated on the source buffer. Or convert the stream in stereo before playing).
   5- Supports only 16-bits audio data size.
==============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "stm32303c_eval_audio.h"

/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup STM32303C_EVAL
  * @{
  */

/** @defgroup STM32303C_EVAL_AUDIO STM32303C_EVAL AUDIO
  * @brief       This file includes the low layer audio driver available on STM32303C_EVAL 
  * evaluation board(MB1019).
  * @{
  */ 

/** @defgroup STM32303C_EVAL_AUDIO_Private_Types Private Types
  * @{
  */ 
/**
  * @}
  */ 
  
/* Private defines ------------------------------------------------------------*/
/** @defgroup STM32303C_EVAL_AUDIO_Private_Constants Private Constants
  * @{
  */ 
/**
  * @}
  */ 

/* Private macros ------------------------------------------------------------*/
/** @defgroup STM32303C_EVAL_AUDIO_Private_Macros Private Macros
  * @{
  */
/**
  * @}
  */ 
  
/* Private variables ---------------------------------------------------------*/
/** @defgroup STM32303C_EVAL_AUDIO_Private_Variables Private Variables
  * @{
  */

/*### PLAY ###*/
static AUDIO_DrvTypeDef           *pAudioDrv = NULL;
I2S_HandleTypeDef                 hAudioOutI2s;

/**
  * @}
  */ 

/* Private function prototypes -----------------------------------------------*/
/** @defgroup STM32303C_EVAL_AUDIO_Private_Functions Private Functions
  * @{
  */ 
static void                 I2Sx_MspInit(void);
static AUDIO_StatusTypeDef  I2Sx_Init(uint32_t AudioFreq);
/**
  * @}
  */ 

/* Exported functions --------------------------------------------------------*/
/** @addtogroup STM32303C_EVAL_AUDIO_Exported_Functions
  * @{
  */ 

/**
  * @brief  Configure the audio peripherals.
  * @param  OutputDevice OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param  Volume Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq Audio frequency used to play the audio stream.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{    
  uint8_t ret = AUDIO_OK;
  uint32_t deviceid = 0x00;

  if(pAudioDrv == NULL)
  {
    deviceid = cs42l52_drv.ReadID(AUDIO_I2C_ADDRESS);

    if((deviceid & CS42L52_ID_MASK) == CS42L52_ID)
    {  
	      /* Initialize the audio driver structure */
      pAudioDrv = &cs42l52_drv; 
      ret = AUDIO_OK;
    }
    else
    {
      ret = AUDIO_ERROR;
    }
  }
  
  if(ret == AUDIO_OK)
  {
    if(pAudioDrv->Init(AUDIO_I2C_ADDRESS, OutputDevice, Volume, AudioFreq) != 0)
    {
      ret = AUDIO_ERROR;
    }
    else
    {
      /* I2S data transfer preparation:
      Prepare the Media to be used for the audio transfer from memory to I2S peripheral */
      /* Configure the I2S peripheral */
      ret = I2Sx_Init(AudioFreq);
    }
  }
  
  return ret;
}

/**
  * @brief  Starts playing audio stream from a data buffer for a determined size. 
  * @param  pBuffer Pointer to the buffer 
  * @param  Size Number of audio data BYTES.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Play(uint16_t* pBuffer, uint32_t Size)
{
  /* Call the audio Codec Play function */
  if (pAudioDrv->Play(AUDIO_I2C_ADDRESS, pBuffer, Size) != 0)
  {
    return AUDIO_ERROR;
  }
  else 
  {
    /* Update the Media layer and enable it for play */  
    return (HAL_I2S_Transmit_DMA(&hAudioOutI2s, pBuffer, DMA_MAX(Size))); 
  }
}

/**
  * @brief Sends n-Bytes on the I2S interface.
  * @param pData pointer on data address 
  * @param Size number of data to be written
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_ChangeBuffer(uint16_t *pData, uint16_t Size)
{
   return (HAL_I2S_Transmit_DMA(&hAudioOutI2s, pData, Size)); 
}

/**
  * @brief  This function Pauses the audio file stream. In case of using DMA, the DMA Pause feature is used.
  * @note When calling BSP_AUDIO_OUT_Pause() function for pause, only
  *          BSP_AUDIO_OUT_Resume() function should be called for resume (use of BSP_AUDIO_OUT_Play() 
  *          function for resume could lead to unexpected behavior).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Pause(void)
{    
  /* Call the Audio Codec Pause/Resume function */
  if (pAudioDrv->Pause(AUDIO_I2C_ADDRESS) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Call the Media layer pause function */
    return (HAL_I2S_DMAPause(&hAudioOutI2s));
  }
}

/**
  * @brief  This function  Resumes the audio file stream.  
  * @note When calling BSP_AUDIO_OUT_Pause() function for pause, only
  *          BSP_AUDIO_OUT_Resume() function should be called for resume (use of BSP_AUDIO_OUT_Play() 
  *          function for resume could lead to unexpected behavior).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Resume(void)
{    
  /* Call the Audio Codec Pause/Resume function */
  if(pAudioDrv->Resume(AUDIO_I2C_ADDRESS) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Call the Media layer resume function */
    return (HAL_I2S_DMAResume(&hAudioOutI2s));
  }
}

/**
  * @brief  Stops audio playing and Power down the Audio Codec. 
  * @param  Option could be one of the following parameters 
  *           - CODEC_PDWN_SW: for software power off (by writing registers). 
  *                            Then no need to reconfigure the Codec after power on.
  *           - CODEC_PDWN_HW: completely shut down the codec (physically). 
  *                            Then need to reconfigure the Codec after power on.  
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_Stop(uint32_t Option)
{
  /* Call DMA Stop to disable DMA stream before stopping codec */
  HAL_I2S_DMAStop(&hAudioOutI2s);
  
  /* Call Audio Codec Stop function */
  if(pAudioDrv->Stop(AUDIO_I2C_ADDRESS, Option) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    if(Option == CODEC_PDWN_HW)
    { 
      /* Wait at least 100us */
      HAL_Delay(1);
    }
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Controls the current audio volume level. 
  * @param  Volume Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_SetVolume(uint8_t Volume)
{
  /* Call the codec volume control function with converted volume value */
  if(pAudioDrv->SetVolume(AUDIO_I2C_ADDRESS, Volume) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Enables or disables the MUTE mode by software 
  * @param  Cmd could be AUDIO_MUTE_ON to mute sound or AUDIO_MUTE_OFF to 
  *         unmute the codec and restore previous volume level.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_SetMute(uint32_t Cmd)
{ 
  /* Call the Codec Mute function */
  if(pAudioDrv->SetMute(AUDIO_I2C_ADDRESS, Cmd) != 0)
  {
    return AUDIO_ERROR;
  }
  else
  {
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Switch dynamically (while audio file is played) the output target 
  *         (speaker or headphone).
  * @note   This function modifies a global variable of the audio codec driver: OutputDev.
  * @param  Output specifies the audio output target: OUTPUT_DEVICE_SPEAKER,
  *         OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO 
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_SetOutputMode(uint8_t Output)
{
  /* Call the Codec output Device function */
  if(pAudioDrv->SetOutputMode(AUDIO_I2C_ADDRESS, Output) != 0)
{
    return AUDIO_ERROR;
  }
  else
  {
    /* Return AUDIO_OK when all operations are correctly done */
    return AUDIO_OK;
  }
}

/**
  * @brief  Update the audio frequency.
  * @param  AudioFreq Audio frequency used to play the audio stream.
  * @retval AUDIO_OK if correct communication, else wrong communication
  */
uint8_t BSP_AUDIO_OUT_SetFrequency(uint32_t AudioFreq)
{ 
  /* Update the I2S audio frequency configuration */
  return (I2Sx_Init(AudioFreq));
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hi2s I2S handle
  * @retval None
  */
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  if(hi2s->Instance == I2Sx)
  {
    /* Call the user function which will manage directly transfer complete*/  
    BSP_AUDIO_OUT_TransferComplete_CallBack();       
  }
}

/**
  * @brief Tx Transfer Half completed callbacks
  * @param hi2s I2S handle
  * @retval None
  */
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  if(hi2s->Instance == I2Sx)
  {
    /* Manage the remaining file size and new address offset: This function 
    should be coded by user (its prototype is already declared in stm32303c_eval_audio.h) */  
    BSP_AUDIO_OUT_HalfTransfer_CallBack();
  }
}

/**
  * @brief I2S error callbacks
  * @param hi2s I2S handle
  * @retval None
  */
void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{
  /* Manage the error generated on DMA: This function 
     should be coded by user (its prototype is already declared in stm32303c_eval_audio.h) */  
  if(hi2s->Instance == I2Sx)
  {
    BSP_AUDIO_OUT_Error_CallBack();
  }
}

/**
  * @brief  Manages the DMA full Transfer complete event.
  * @retval None
  */
__weak void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @retval None
  */
__weak void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
}

/**
  * @brief  Audio OUT Error callback function
  * @retval None
  */
__weak void BSP_AUDIO_OUT_Error_CallBack(void)
{
}

/**
  * @}
  */

/** @addtogroup STM32303C_EVAL_AUDIO_Private_Functions
  * @{
  */

/******************************************************************************
                            Static Function
*******************************************************************************/

/**
  * @brief AUDIO OUT I2S MSP Init
  * @retval None
  */
static void I2Sx_MspInit(void)
{
  static DMA_HandleTypeDef hdma_i2sTx;
  GPIO_InitTypeDef  GPIO_InitStruct;
  I2S_HandleTypeDef *hi2s = &hAudioOutI2s;

  /* Enable I2S GPIO clocks */
  I2Sx_MCK_WS_GPIO_CLK_ENABLE();
  I2Sx_SCK_DIN_GPIO_CLK_ENABLE();

  /* I2S pins configuration: SCK and DIN pins -----------------------------*/
  GPIO_InitStruct.Pin         = (I2Sx_SCK_PIN | I2Sx_DIN_PIN); 
  GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull        = GPIO_NOPULL;
  GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate   = I2Sx_SCK_DIN_AF;
  HAL_GPIO_Init(I2Sx_SCK_DIN_GPIO_PORT, &GPIO_InitStruct);

  /* I2S pins configuration: WS pin -----------------------------*/
  GPIO_InitStruct.Pin         = I2Sx_WS_PIN; 
  GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull        = GPIO_NOPULL;
  GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate   = I2Sx_WS_AF;
  HAL_GPIO_Init(I2Sx_WS_GPIO_PORT, &GPIO_InitStruct);

  /* I2S pins configuration: MCK pin -----------------------------*/
  GPIO_InitStruct.Pin         = I2Sx_MCK_PIN; 
  GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull        = GPIO_NOPULL;
  GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate   = I2Sx_MCK_AF;
  HAL_GPIO_Init(I2Sx_MCK_GPIO_PORT, &GPIO_InitStruct);

  /* Enable I2S clock */
  I2Sx_CLK_ENABLE();

  /* Force the I2S peripheral clock reset */
  I2Sx_FORCE_RESET();

  /* Release the I2S peripheral clock reset */
  I2Sx_RELEASE_RESET();

  /* Enable the I2S DMA clock */
  I2Sx_DMAx_CLK_ENABLE(); 
  
  /* Configure the hdma_i2sTx handle parameters */   
  hdma_i2sTx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_i2sTx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_i2sTx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_i2sTx.Init.PeriphDataAlignment = I2Sx_DMAx_PERIPH_DATA_SIZE;
  hdma_i2sTx.Init.MemDataAlignment    = I2Sx_DMAx_MEM_DATA_SIZE;
  hdma_i2sTx.Init.Mode                = DMA_NORMAL;
  hdma_i2sTx.Init.Priority            = DMA_PRIORITY_HIGH;

  hdma_i2sTx.Instance                 = I2Sx_DMAx_CHANNEL;

  /* Associate the DMA handle */
  __HAL_LINKDMA(hi2s, hdmatx, hdma_i2sTx);
   
  /* Configure the DMA Stream */
  HAL_DMA_Init(&hdma_i2sTx);
  
  /* I2S DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority((IRQn_Type)I2Sx_DMAx_IRQ, AUDIO_OUT_IRQ_PREPRIO, AUDIO_OUT_IRQ_SUBPRIO);
  HAL_NVIC_EnableIRQ((IRQn_Type)I2Sx_DMAx_IRQ); 
}

/**
  * @brief  Initializes the Audio Codec audio interface (I2S)
  * @param  AudioFreq Audio frequency to be configured for the I2S peripheral. 
  * @retval AUDIO_StatusTypeDef AUDIO Status
  */
static AUDIO_StatusTypeDef I2Sx_Init(uint32_t AudioFreq)
{
  /* I2S peripheral configuration */
  hAudioOutI2s.Init.AudioFreq   = AudioFreq;
  hAudioOutI2s.Init.ClockSource = I2S_CLOCK_SYSCLK;
  hAudioOutI2s.Init.CPOL        = I2S_CPOL_LOW;
  hAudioOutI2s.Init.DataFormat  = I2S_DATAFORMAT_16B;
  hAudioOutI2s.Init.MCLKOutput  = I2S_MCLKOUTPUT_ENABLE;
  hAudioOutI2s.Init.Mode        = I2S_MODE_MASTER_TX;
  hAudioOutI2s.Init.Standard    = I2S_STANDARD;
  hAudioOutI2s.Instance         = I2Sx;

  /* Disable I2S block */
  __HAL_I2S_DISABLE(&hAudioOutI2s);

  /* Initialize the I2S peripheral with the structure above */
  if(HAL_I2S_GetState(&hAudioOutI2s) == HAL_I2S_STATE_RESET)
  { 
    I2Sx_MspInit();
  }

  if (HAL_I2S_Init(&hAudioOutI2s) != HAL_OK)
  {
    return AUDIO_ERROR;
  }
  
  return AUDIO_OK;
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

/**
  * @}
  */ 

