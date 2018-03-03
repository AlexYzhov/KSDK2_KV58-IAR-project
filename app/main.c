/*
* Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following  disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "include.h"

bool MT9V034_CaptureAccomplished = false;
volatile uint8_t MT9V034_IMGBUFF[MT9V034_SIZE];

void PORTA_IRQHandler(void)
{
    //volatile uint32_t ISFR_FLAG = PORTA->ISFR;
    //PORTA->ISFR = 0x00000000;
    
    MT9V034_FrameValid_Callback(PORTA->ISFR);
}

void DMA0_DMA16_IRQHandler(void)
{
    MT9V034_DMA_Callback();
}

uint32_t main(void)
{
  Init_ALL();
  
  MT9V034_Init();
  
  while(GPIOA->PDIR&(1U<<17U));           //DKEY4�����£���ȴ�
        
  if(GPIOA->PDIR&(1U<<14U))         //DKEY3������
  {
      SIM->CLKDIV1 = 0x01340000U;
  }
  else
  {
      SIM->CLKDIV1 = 0x01390000U;
  }
  
  //LCD_Init();
  //LCD_Clear(BLACK);
  //LCD_String((Point_t){60, 60}, "Hello", WHITE, BLACK);
  
  //GPIO_WritePinOutput(GPIOC, 0U, 0U);
  GPIOC->PCOR |= (1U<<0U);
  
  //UpperCOM_PutBuff("Hello World!\r\n", sizeof("Hello World!\r\n"));
  
  for(;;)
  {
      if(MT9V034_CaptureAccomplished)
      {
          DisableIRQ(PORTA_IRQn);
          UpperCOM_SendImg(MT9V034_IMGBUFF, sizeof(MT9V034_IMGBUFF));
          EnableIRQ(PORTA_IRQn);
      }
      
      
  }
}

void HardFault_Handler()
{
  return;
}