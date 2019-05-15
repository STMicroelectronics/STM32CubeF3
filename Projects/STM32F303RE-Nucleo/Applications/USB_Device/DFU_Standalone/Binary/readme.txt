/**
  @page Binary Description of the binary template
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    USB_Device/DFU_Standalone/Binary/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the binary template example.
  ******************************************************************************
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
  @endverbatim

@par Example Description 

This directory contains a binary template (in DFU format) to be loaded into 
Flash memory using Device Firmware Upgrade application.

This file was converted to the DFU format using the "DFU File Manager Tool" included in the "DfuSe" PC software install.
For more details on how to convert a .bin file to DFU format please refer to the
UM0412 user manual "Getting started with DfuSe USB device firmware upgrade STMicroelectronics extension"
available from the STMicroelectronics microcontroller website www.st.com.
   
This binary is a simple LED toggling.
The system Timer (Systick) is used for to generate the delay.
The offset address of this binary is 0x08007000 which matches the definition in
DFU application "USBD_DFU_APP_DEFAULT_ADD".


@par Hardware and Software environment

  - This example runs on STM32F303xE devices
    
  - This example has been tested with STM32F303RE-Nucleo Rev C board and can be
    easily tailored to any other supported device and development board.   
      
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
