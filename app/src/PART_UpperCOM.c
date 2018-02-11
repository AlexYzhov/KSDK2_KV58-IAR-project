#include "include.h"

void UpperCOM_PutBuff(uint8_t *buff, uint32_t len)
{
    while(len--)
    {
        UART_WriteByte(UpperCOM, *buff);
        ALEX_CPU_Delay(0x2000);
        buff++;
    }
}

void UpperCOM_SendImg(void *ImgAddr, uint32_t ImgSize)
{
    uint8_t HEAD[2] = { 0x01, 0xFE };
    uint8_t TAIL[2] = { 0xFE, 0x01 };

    UpperCOM_PutBuff(HEAD, sizeof(HEAD));          //先发送命令
    UpperCOM_PutBuff((uint8_t *)ImgAddr, ImgSize); //再发送图像
    UpperCOM_PutBuff(TAIL, sizeof(TAIL));          //再发送命令
}

void UpperCOM_SendWare(void *WareAddr, uint32_t WareSize)
{
    static uint8_t HEAD[2];
        HEAD[0] = 0x03U;
        HEAD[1] = 0xFCU;
    static uint8_t TAIL[2];
        TAIL[0] = 0xFCU;
        TAIL[1] = 0x03U;
    
    // UART_WriteBlocking(UpperCOM, HEAD, sizeof(HEAD));
    // UART_WriteBlocking(UpperCOM, (uint8_t*)WareAddr, WareSize);
    // UART_WriteBlocking(UpperCOM, TAIL, sizeof(TAIL));
    
    UpperCOM_PutBuff(HEAD, sizeof(HEAD));
    UpperCOM_PutBuff((uint8_t*)WareAddr, WareSize);
    UpperCOM_PutBuff(TAIL, sizeof(TAIL));
}