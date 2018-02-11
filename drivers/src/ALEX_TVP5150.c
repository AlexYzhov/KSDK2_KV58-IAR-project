#include  "include.h"

#define TVP_I2C_PORT I2C3
#define TVP_SCK PTE11    //纯粹标记管脚信息
#define TVP_SDA PTE10    //纯粹标记管脚信息
#define TVP_I2CSEL PTB0  //纯粹标记管脚信息

#define somenop();   asm("nop"); asm("nop");asm("nop"); asm("nop");
#define byte         unsigned char


// void TVP5150_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)
// {
//     I2C_MasterStart(I2C3, SlaveAddress, kI2C_Write);
//     ALEX_lptmr_DelayMs(20);
//     I2C_MasterWriteBlocking(I2C3, &REG_Address, sizeof(REG_Address), kI2C_TransferNoStopFlag);
//     ALEX_lptmr_DelayMs(20);
//     I2C_MasterWriteBlocking(I2C3, &REG_data, sizeof(REG_data), kI2C_TransferNoStopFlag);
//     ALEX_lptmr_DelayMs(20);
//     I2C_MasterStop(I2C3);
// }

void TVP5150_Init()
{
    I2C_WriteByte(I2C3, TVP5150_Addr>>1, TVP_REG0, REG0_Default);
    ALEX_lptmr_DelayMs(20);
    
    I2C_WriteByte(I2C3, TVP5150_Addr>>1, TVP_REG3, REG3_Default);
    ALEX_lptmr_DelayMs(20);
    
    I2C_WriteByte(I2C3, TVP5150_Addr>>1, TVP_REG13, REG13_Default);
    ALEX_lptmr_DelayMs(20);
}