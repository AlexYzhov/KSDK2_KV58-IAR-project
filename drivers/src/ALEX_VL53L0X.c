#include "include.h"


uint16_t bswap(uint8_t b[])
{
    uint16_t val = ((b[0]<< 8) & b[1]);
    return val;
}

uint16_t VL53L0X_decode_vcsel_period(short vcsel_period_reg)
{
    uint16_t vcsel_period_pclks = (vcsel_period_reg + 1) <<1;
    return vcsel_period_pclks;
}

uint16_t makeuint16(int lsb, int msb)
{
    return ((msb & 0xFF) << 8) | (lsb & 0xFF);
}

//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//����,�������
uint8_t VL53L0X_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)      // check��
{
    // uint8_t i;
    
    I2C_MasterStart(I2C1, VL53L0X_Add, kI2C_Write);
    ALEX_lptmr_DelayMs(1);
    
    I2C_MasterWriteBlocking(I2C1, &reg, sizeof(reg), kI2C_TransferNoStopFlag);
    ALEX_lptmr_DelayMs(1);
    
    I2C_MasterWriteBlocking(I2C1, buf, len, kI2C_TransferNoStopFlag);
    
    I2C_MasterStop(I2C1);
    
    return 0;
} 
//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
uint8_t VL53L0X_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{
    I2C_MasterStart(I2C1, VL53L0X_Add, kI2C_Write);       // kStataus_I2C_Nak
    ALEX_lptmr_DelayMs(1);       // Wait Ack
    
    I2C_MasterWriteBlocking(I2C1, &reg, sizeof(reg), kI2C_TransferNoStopFlag);
    ALEX_lptmr_DelayMs(1);
    
    I2C_MasterRepeatedStart(I2C1, VL53L0X_Add, kI2C_Read);
    ALEX_lptmr_DelayMs(1);       // Wait Ack
    
    I2C_MasterReadBlocking(I2C1, buf, len, kI2C_TransferNoStopFlag);
    I2C_MasterStop(I2C1);
    
    return 0;
}

//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������

uint8_t VL53L0X_Write_Byte(uint8_t reg,uint8_t data)            // check��
{ 
    I2C_MasterStart(I2C1, VL53L0X_Add, kI2C_Write);
    ALEX_lptmr_DelayMs(1);
    
    I2C_MasterWriteBlocking(I2C1, &reg, sizeof(reg), kI2C_TransferNoStopFlag);
    ALEX_lptmr_DelayMs(1);
    
    I2C_MasterWriteBlocking(I2C1, &data, sizeof(data), kI2C_TransferNoStopFlag);
    ALEX_lptmr_DelayMs(1);
    
    I2C_MasterStop(I2C1);
    
    return 0;
}

//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
uint8_t VL53L0X_Read_Byte(uint8_t reg)
{
    uint8_t res = 0;
    
    I2C_MasterStart(I2C1, VL53L0X_Add, kI2C_Write);
    ALEX_lptmr_DelayMs(1);
    
    I2C_MasterWriteBlocking(I2C1, &reg, sizeof(reg), kI2C_TransferNoStopFlag);
    ALEX_lptmr_DelayMs(1);
    
    I2C_MasterRepeatedStart(I2C1, VL53L0X_Add, kI2C_Read);
    ALEX_lptmr_DelayMs(1);
    
    I2C_MasterReadBlocking(I2C1, &res, sizeof(reg), kI2C_TransferNoStopFlag);
    I2C_MasterStop(I2C1);
    
    return res;
}

void VL53L0X_Set_HIGHSPEED(void)
{
    
}

uint16_t VL53L0X_ReadSingleShotInMillimeters(void)
{
    VL53L0X_Write_Byte(VL53L0X_REG_SYSRANGE_START, VL53L0X_REG_SYSRANGE_MODE_START_STOP);
    
    while(!(VL53L0X_Read_Byte(VL53L0X_REG_RESULT_RANGE_STATUS)&0x01))
    {
        ALEX_lptmr_DelayMs(5);
    }
    
    return (VL53L0X_Read_Byte(VL53L0X_REG_RESULT_RANGE_STATUS+10)<<8)|(VL53L0X_Read_Byte(VL53L0X_REG_RESULT_RANGE_STATUS+11));
}

void VL53L0X_Init(void)
{
    //VL53L0X_Write_Byte
}
