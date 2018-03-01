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
 *  @brief      SCCB起始信号
 *  @since      v5.0
 */
static uint8_t SCCB_Start(void)
{
    SDA_DDR_OUT();

    SDA_H();
    SCL_H();
    SCCB_DELAY();

    //确认SDA进入高电平
    SDA_DDR_IN();
    if(!SDA_IN())
    {
        SDA_DDR_OUT();
        return 0;   /* SDA线为低电平则总线忙,退出 */
    }
    SDA_DDR_OUT();

    SDA_L();
    SCCB_DELAY();
    SCL_L();

    //确认SDA进入低电平
    if(SDA_IN())
    {
        SDA_DDR_OUT();
        return 0;   /* SDA线为高电平则总线出错,退出 */
    }
    
    return 1;
}

/*!
 *  @brief      SCCB停止信号
 *  @since      v5.0
 */
void SCCB_Stop(void)
{
    SDA_DDR_OUT();

    SCL_L();
    SDA_L();
    SCCB_DELAY();
    SCL_H();
    SDA_H();
    SCCB_DELAY();
}

/*!
 *  @brief      SCCB应答信号
 *  @since      v5.0
 */
/*
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
*/

/*!
 *  @brief      SCCB无应答信号
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
 *  @brief      SCCB 等待应答
 *  @return     应答结果（0表示无应答，1表示有应答）
 *  @since      v5.0
 */
static int SCCB_WaitAck(void)
{
    SDA_DDR_IN();
    SCCB_DELAY();

    //SCL_H();
    //SDA_H();

    SCL_H();
    SCCB_DELAY();

    if(SDA_IN())           //应答为高电平，异常，通信失败
    {
        SDA_DDR_OUT();
        SCL_L();
        return 0;
    }
    SDA_DDR_OUT();
    
    SCL_L();
    return 1;
}

/*!
 *  @brief      SCCB 发送的数据
 *  @param      SendByte    需要发送的数据
 *  @since      v5.0
 */
static void SCCB_SendByte(uint8_t SendByte)
{
    uint8_t i = 8;
    while(i--)
    {

        if(SendByte & 0x80)     //SDA 输出数据
        {
            SDA_H();
        }
        else
        {
            SDA_L();
        }
        SendByte <<= 1;
        SCCB_DELAY();
        SCL_H();                //SCL 拉高，采集信号
        SCCB_DELAY();
        SCL_L();                //SCL 时钟线拉低
        //SCCB_DELAY();
    }
    //SCL_L();
}

/*!
 *  @brief      接收SCCB总线的数据
 *  @return     接收到的数据
 *  @since      v5.0
 */
static int SCCB_ReceiveByte(void)
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
    SDA_DDR_OUT();
    SCL_L();
    return ReceiveByte;
}

// ---------------------------------------------------------------------------------------- //

int16_t SCCB_WriteByte_soft(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data)
{
    sccb_write_start:
  
    if(!SCCB_Start())
    {
        goto sccb_write_start;
    }
    SCCB_SendByte(SlaveAddress<<1|0);                    /* 器件地址 */
    if(!SCCB_WaitAck())
    {
        SCCB_Stop();
        goto sccb_write_start;
    }
    
    SCCB_SendByte((uint8_t)(REG_Address&0x00FF));   /* 设置低起始地址 */
    SCCB_WaitAck();
    SCCB_SendByte(REG_data);
    SCCB_WaitAck();
    //SCCB_Stop();
    return 1;
}

int16_t SCCB_ReadByte_soft(uint8_t SlaveAddress, uint8_t REG_Address)
{  
    uint8_t pBuffer = 0;

    sccb_read_start:
    if(!SCCB_Start())
    {
        goto sccb_read_start;
    }
    SCCB_SendByte(SlaveAddress<<1|0);         /* 器件地址 */
    if(!SCCB_WaitAck())
    {
        SCCB_Stop();
        goto sccb_read_start;
    }

    SCCB_SendByte(REG_Address);           /* 设置低起始地址 */
    SCCB_WaitAck();
    
    //SCCB_Stop();

    if(!SCCB_Start())
    {
        goto sccb_read_start;
    }
    SCCB_SendByte(SlaveAddress<<1|1);               /* 器件地址 */
    if(!SCCB_WaitAck())
    {
        SCCB_Stop();
        goto sccb_read_start;
    }

    // while(length)
    // {
    //     *pBuffer = SCCB_ReceiveByte();
    //     if(length == 1)
    //     {
    //         SCCB_NoAck();
    //     }
    //     else
    //     {
    //         SCCB_Ack();
    //     }
    //     pBuffer++;
    //     length--;
    // }
    
    pBuffer = SCCB_ReceiveByte();       // 只读一个数据
    SCCB_NoAck();
    
    return pBuffer;
}
#endif

