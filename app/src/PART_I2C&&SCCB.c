#include "include.h"

#define _SCCB_SOFT_ 1

void I2C_WriteByte(I2C_Type *base, uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data)
{
    I2C_MasterStart(base, SlaveAddress, kI2C_Write);
    ALEX_lptmr_DelayMs(2);
    I2C_MasterWriteBlocking(base, &REG_Address, sizeof(REG_Address), kI2C_TransferNoStopFlag);
    ALEX_lptmr_DelayMs(2);
    I2C_MasterWriteBlocking(base, &REG_data, sizeof(REG_data), kI2C_TransferNoStopFlag);
    I2C_MasterStop(base);
}

uint8_t I2C_ReadByte(I2C_Type *base, uint8_t SlaveAddress, uint8_t REG_Address)
{
    uint8_t recvData;
    
    I2C_MasterStart(base, SlaveAddress, kI2C_Write);
    ALEX_lptmr_DelayMs(2);
    I2C_MasterWriteBlocking(base, &REG_Address, sizeof(REG_Address), kI2C_TransferNoStopFlag);
    ALEX_lptmr_DelayMs(2);
    
    
    I2C_MasterRepeatedStart(base, SlaveAddress, kI2C_Read);
    ALEX_lptmr_DelayMs(2);    
    I2C_MasterReadBlocking(base, &recvData, sizeof(recvData), kI2C_TransferDefaultFlag);        // kI2C_TransferDefaultFlag
    // I2C_MasterStop(base);
    
    return recvData;
}

#if _SCCB_SOFT_
/*!
 *  @brief      SCCB��ʼ�ź�
 *  @since      v5.0
 */
void SCCB_Start(void)
{
  
    SCL_L();
    SDA_H();
    SCCB_DELAY();
    SCL_H();
    SCCB_DELAY();
    SDA_L();
    SCCB_DELAY();
    SCL_L();
    SCCB_DELAY();
}

void SCCB_Stop(void)
{
    SCL_L();
    //SCCB_DELAY();
    SDA_L();
    SCCB_DELAY();
    SCL_H();
    SCCB_DELAY();
    SDA_H();
    SCCB_DELAY();
    SCL_L();
    SCCB_DELAY();
}

/*!
 *  @brief      SCCBӦ���ź�
 *  @since      v5.0
 */
static void SCCB_Ack(void)
{
    SCL_L();
    SCCB_DELAY();
    SDA_L();
    SCCB_DELAY();
    SCL_H();
    SCCB_DELAY();
    SCL_L();
    SCCB_DELAY();
}

/*!
 *  @brief      SCCB��Ӧ���ź�
 *  @since      v5.0
 */
static void SCCB_NoAck(void)
{
    SCL_L();
    SCCB_DELAY();
    SDA_H();
    SCCB_DELAY();
    SCL_H();
    SCCB_DELAY();
    SCL_L();
    SCCB_DELAY();
}

/*!
 *  @brief      SCCB �ȴ�Ӧ��
 *  @return     Ӧ������0��ʾ��Ӧ��1��ʾ��Ӧ��
 *  @since      v5.0
 */
int SCCB_WaitAck(void)
{
    SCL_L();
    //SDA_H();
    SDA_DDR_IN();

    SCL_H();

    SCCB_DELAY();

    if(SDA_IN())           //Ӧ��Ϊ�ߵ�ƽ���쳣��ͨ��ʧ��
    {
        SCL_L();
        SCCB_DELAY();
        SDA_DDR_OUT();
        return 0;
    }
    SCL_L();
    SCCB_DELAY();
    SDA_DDR_OUT();
    return 1;
}

/*!
 *  @brief      SCCB ���͵�����
 *  @param      SendByte    ��Ҫ���͵�����
 *  @since      v5.0
 */
void SCCB_SendByte(uint8_t SendByte)
{
    uint8_t i = 8;
    SCL_L();
    SCCB_DELAY();
    while(i--)
    {

        if(SendByte & 0x80)     //SDA �������
        {
            SDA_H();
        }
        else
        {
            SDA_L();
        }
        SendByte <<= 1;
        SCCB_DELAY();
        SCL_H();                //SCL ���ߣ��ɼ��ź�
        SCCB_DELAY();
        SCL_L();
        SCCB_DELAY();
        //SCL ʱ��������
        //SCCB_DELAY();
    }
    //SCL_L();
}

/*!
 *  @brief      ����SCCB���ߵ�����
 *  @return     ���յ�������
 *  @since      v5.0
 */
int SCCB_ReceiveByte(void)
{
    uint8_t i = 8;
    uint8_t ReceiveByte = 0;

    //SDA_H();
    //SCCB_DELAY();
    SDA_DDR_IN();

    while(i--)
    {
        ReceiveByte <<= 1;
        SCL_L();
        SCCB_DELAY();
        SCL_H();
        SCCB_DELAY();

        if(SDA_IN())
        {
            ReceiveByte |= 0x01;
        }
    }
    SCL_L();
    SCCB_DELAY();
    SDA_DDR_OUT();
    return ReceiveByte;
}

// ---------------------------------------------------------------------------------------- //

void SCCB_UnlockI2C()
{
    //SCCB_Stop();
    
    SCCB_DELAY();
    SCCB_DELAY();
    
    for(uint8_t i=0; i<9; i++)
    {
        SCL_L();
        SCCB_DELAY();
        SCL_H();
        SCCB_DELAY();
    }
    
    SCCB_DELAY();
}

int16_t SCCB_WriteByte_soft(uint8_t SlaveAddress, uint8_t REG_Address, uint16_t REG_data)
{
    SCCB_SendByte( SlaveAddress<<1|0 );                    /* ������ַ */
    if(!SCCB_WaitAck())
    {
        SCCB_Stop();
        return 0;
    }
    SCCB_SendByte(REG_Address);   /* ���õ���ʼ��ַ */
    SCCB_WaitAck();
    SCCB_SendByte((uint8_t)(REG_data>>8));
    SCCB_WaitAck();
    SCCB_SendByte((uint8_t)REG_data&0x00FF);
    SCCB_WaitAck();
    return 1;
}


int16_t SCCB_ReadByte_soft(uint8_t SlaveAddress, uint8_t REG_Address)
{
    uint16_t pBuffer = 0;
    SCCB_Start();
    SCCB_SendByte(SlaveAddress<<1|0);         /* ������ַ */
    if(!SCCB_WaitAck())
    {
        SCCB_Stop();
        return -1;
    }
    SCCB_SendByte(REG_Address);           /* ���õ���ʼ��ַ */
    SCCB_WaitAck();
    SCCB_DELAY();
    SCCB_Start();
    SCCB_SendByte(SlaveAddress<<1|1);               /* ������ַ */

    if(!SCCB_WaitAck())
    {
        SCCB_Stop();
        return -3;
    }
    
    pBuffer = SCCB_ReceiveByte();       // ֻ��һ������
    SCCB_Ack();
    pBuffer <<= 8;
    pBuffer |=SCCB_ReceiveByte();
    SCCB_NoAck();
    
    return pBuffer;
}
#endif

