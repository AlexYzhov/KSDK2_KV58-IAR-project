#include "include.h"

#define SIOC PTD8 //SCCB_SCL
#define SIOD PTD9 //SCCB_SDA

void SCCB_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)
{
    LPLD_I2C_StartTrans(I2C0, SlaveAddress, I2C_MWSR);
    LPLD_I2C_WaitAck(I2C0, I2C_ACK_ON);
    
    LPLD_I2C_WriteByte(I2C0, REG_Address);
    LPLD_I2C_WaitAck(I2C0, I2C_ACK_ON);
    
    LPLD_I2C_WriteByte(I2C0, REG_data);
    LPLD_I2C_WaitAck(I2C0, I2C_ACK_ON);
    
    LPLD_I2C_Stop(I2C0);
}

void Init_7725(void)
{
    LPLD_LPTMR_DelayMs(100);            //等待7725工作稳定
    
    SCCB_Write(OV7725_Addr>>1, 0x11, 0x03);
    LPLD_LPTMR_DelayMs(20);
    
}