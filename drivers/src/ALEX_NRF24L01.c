#include "ALEX_NRF24L01.h"

dspi_command_data_config_t SPI1_NRF24L01_Ctar0 =
{
    .isPcsContinuous = false,            // 发送结束后释放cs，否则持续拉低cs
    .whichCtar = kDSPI_Ctar0,
    .whichPcs = kDSPI_Pcs0,
    .clearTransferCount = false,
    .isEndOfQueue = false,
};

dspi_command_data_config_t SPI1_NRF24L01_Ctar1 =
{
    .isPcsContinuous = true,            // 发送结束后释放cs，否则持续拉低cs
    .whichCtar = kDSPI_Ctar1,
    .whichPcs = kDSPI_Pcs0,
    .clearTransferCount = false,
    .isEndOfQueue = false,
};

volatile uint8_t NRF24L01_CONFIG = 0x00;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t NRF24L01_ReadReg(uint8_t reg)
{
    DSPI_FlushFifo(SPI1, true, true);
    DSPI_FlushFifo(SPI1, true, true);
    
    NRF24L01_CS_L;
    NRF24L01_Delay(400);
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar0, ((NRF_READ_REG|reg)<<8)|0xFF);
    uint8_t rdata = (uint8_t)DSPI_ReadData(SPI1);
    NRF24L01_Delay(400);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
    return rdata;
}

void NRF24L01_WriteReg(uint8_t reg, uint8_t wdata)
{
    NRF24L01_CS_L;
    NRF24L01_Delay(400);
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar0, ((NRF_WRITE_REG|reg)<<8)|wdata);
    NRF24L01_Delay(400);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

void NRF24L01_BurstRead(uint8_t reg, uint8_t *pBuff, uint8_t len)
{
    NRF24L01_CS_L;
    NRF24L01_Delay(400);
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, NRF_READ_REG|reg);
    for(uint8_t i=0; i<len; i++)
    {
        DSPI_FlushFifo(SPI1, true, true);
        DSPI_FlushFifo(SPI1, true, true);
        DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xFF);
        pBuff[i] = (uint8_t)DSPI_ReadData(SPI1);
        NRF24L01_Delay(400);
    }
    NRF24L01_Delay(400);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

void NRF24L01_BurstWrite(uint8_t reg, uint8_t *pBuff, uint8_t len)
{
    NRF24L01_CS_L;
    NRF24L01_Delay(400);
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, NRF_WRITE_REG|reg);
    for(uint8_t i=0; i<len; i++)
    {
        DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, pBuff[i]);
        NRF24L01_Delay(400);
    }
    NRF24L01_Delay(400);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :清空RX缓冲区
* @param :无
* @note  :无
* @retval:无
*/
inline void NRF24L01_FlushRxFIFO(void)
{
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, FLUSH_RX);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :清空TX缓冲区
* @param :无
* @note  :无
* @retval:无
*/
inline void NRF24L01_FlushTxFIFO(void)
{
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, FLUSH_TX);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :重用上一个数据包
* @param :无
* @note  :无
* @retval:无
*/
inline void NRF24L01_ReuseLastTxPayload(void)
{
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, REUSE_TX_PL);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :NRF24L01空指令
* @param :无
* @note  :无
* @retval:无
*/
inline void NRF24L01_NOP(void)
{
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, NOP);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :读取NRF24L01状态寄存器值
* @param :无
* @note  :无
* @retval:RF24L01状态
*/
uint8_t NRF24L01_GetStatusRegister()
{
    DSPI_FlushFifo(SPI1, true, true);
    DSPI_FlushFifo(SPI1, true, true);
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, NRF_READ_REG|STATUS);
    uint8_t status = (uint8_t)DSPI_ReadData(SPI1);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
    return status;
}

/**
* @brief :读FIFO中数据宽度
* @param :无
* @note  :无
* @retval:数据宽度
*/
uint8_t NRF24L01_GetFIFOWidth()
{
    DSPI_FlushFifo(SPI1, true, true);
    DSPI_FlushFifo(SPI1, true, true);
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar0, (R_RX_PL_WID<<8)|0xFF);
    uint8_t rdata = (uint8_t)DSPI_ReadData(SPI1);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
    return rdata;
}

/**
* @brief :读RF24L01中断状态
* @param :无
* @note  :无
* @retval:中断状态
*/
inline uint8_t NRF24L01_GetIRQnStatus()
{
    return (NRF24L01_GetStatusRegister()&((1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT)));	//返回中断状态
}

/**
* @brief :清NRF24L01中断
* @param :
@IRQ_Source:中断源
* @note  :无
* @retval:清除后状态寄存器的值
*/
uint8_t NRF24L01_ClearIRQnFlag(uint8_t IRQ_Source)
{
    uint8_t status = NRF24L01_GetStatusRegister();			            //读状态寄存器
    IRQ_Source &= (1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT);	                            //中断标志处理	
    NRF24L01_CS_L;			//片选
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, NRF_WRITE_REG|STATUS);	    //写状态寄存器命令
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1,    IRQ_Source|status);		//清相应中断标志
    NRF24L01_CS_H;			//取消片选
    NRF24L01_Delay(400);
    return NRF24L01_GetStatusRegister();			//返回状态寄存器状态
}

/**
* @brief :读接收到的数据
* @param :无
* @note  :无
* @retval:
@pRxBuf:数据存放地址首地址
*/
uint8_t NRF24L01_ReadRxPayload(uint8_t *pBuff)
{
    uint8_t PipeNum = (NRF24L01_ReadReg(STATUS)>>1)&0x07;
    uint8_t Width   = NRF24L01_GetFIFOWidth();
    
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, RD_RX_PLOAD);      // 读有效数据命令
    for(uint8_t i=0; i<Width; i++)
    {
        DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xFF);
        pBuff[i] = (uint8_t)DSPI_ReadData(SPI1);
    }
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
    
    NRF24L01_FlushRxFIFO();
    return Width;
}

/**
* @brief :发送数据（带应答）
* @param :
*			@pTxBuf:发送数据地址
*			@len:长度
* @note  :一次不超过32个字节
* @retval:无
*/
void NRF24L01_WriteTxPayload_ACK(uint8_t *pBuff, uint8_t len)
{
    NRF24L01_FlushTxFIFO();                 // 清空TX FIFO
    
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, WR_TX_PLOAD);
    for(uint8_t i=0; i<((len>32)?32:len); i++)          // 限制最大发送字符串长度为32
    {
        DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, pBuff[i]);
        NRF24L01_Delay(400);
    }
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :发送数据（不带应答）
* @param :
*			@pTxBuf:发送数据地址
*			@len:长度
* @note  :一次不超过32个字节
* @retval:无
*/
void NRF24L01_WriteTxPayload_NOACK(uint8_t *pBuff, uint8_t len)
{
    if(len>32||len==0)
    {
        return;
    }
    
    NRF24L01_FlushTxFIFO();                 // 清空TX FIFO
    
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, WR_TX_PLOAD_NACK);
    for(uint8_t i=0; i<len; i++)
    {
        DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, pBuff[i]);
        NRF24L01_Delay(400);
    }
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :在接收模式下向TX FIFO写数据(带ACK)
* @param :
*			@pData:数据地址
*			@len:长度
* @note  :一次不超过32个字节
* @retval:无
*/
void NRF24L01_WriteTxPayload_InRx_ACK(uint8_t *pBuff, uint8_t len)
{
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, W_ACK_PLOAD);
    for(uint8_t i=0; i<((len>32)?32:len); i++)          // 限制最大发送字符串长度为32
    {
        DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, pBuff[i]);
        NRF24L01_Delay(400);
    }
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :设置发送地址
* @param :
*			@pAddr:地址存放地址
*			@len:长度
* @note  :无
* @retval:无
*/
inline void NRF24L01_SetTxAddr(uint8_t *pBuff, uint8_t len)
{
    len = (len>5)?5:len;
    NRF24L01_BurstWrite(TX_ADDR, pBuff, len);         // 限制地址小于5个字节
}

/**
* @brief :设置接收通道地址
* @param :
*			@PipeNum:通道
*			@pAddr:地址存肥着地址
*			@Len:长度
* @note  :通道不大于5 地址长度不大于5个字节
* @retval:无
*/
inline void NRF24L01_SetRxAddr(uint8_t PipeNum, uint8_t *pBuff, uint8_t len)
{
    PipeNum = (PipeNum>5)?5:PipeNum;
    len = (len>5)?5:len;
    NRF24L01_BurstWrite(RX_ADDR_P0+PipeNum, pBuff, len);         // 限制通道号小于5，限制地址小于5个字节
}

/**
* @brief :设置通信速度
* @param :
*			@Baudrate:波特率
* @note  :无
* @retval:无
*/
void NRF24L01_SetBaudrate(nRf24l01SpeedType BaudRate)
{
    uint8_t btmp = 0;
    
    btmp = NRF24L01_ReadReg(RF_SETUP);
    btmp &= ~((1U<<5U)|(1U<<3U));
    
    switch(BaudRate)
    {
        case SPEED_250K: 
        btmp |= (1U<<5U);
        break;
        case SPEED_1M: 
        btmp &= ~((1U<<5U)|(1U<<3U));
        break;
        case SPEED_2M: 
        btmp |= (1U<<3U);
        break;
    }
    
    NRF24L01_WriteReg(RF_SETUP, btmp);
}

/**
* @brief :设置功率
* @param :
*			@Speed:速度
* @note  :无
* @retval:无
*/
void NRF24L01_SetPower(nRf24l01PowerType Power)
{
    uint8_t btmp = NRF24L01_ReadReg(RF_SETUP)&~0x07;
    switch(Power)
    {
        case POWER_F18DBM:
        btmp |= PWR_18DB;
        break;
        case POWER_F12DBM:
        btmp |= PWR_12DB;
        break;
        case POWER_F6DBM:
        btmp |= PWR_6DB;
        break;
        case POWER_0DBM:
        btmp |= PWR_0DB;
        break;
        default:
        break;
    }
    NRF24L01_WriteReg(RF_SETUP, btmp);
}

/**
* @brief :设置频点
* @param :
*			@FreqPoint:频点设置参数
* @note  :值不大于127
* @retval:无
*/
inline void RF24LL01_SetHoppingPoint(uint8_t FreqPoint)
{
    NRF24L01_WriteReg(RF_CH, FreqPoint&0x7F);
}

/**
* @brief :NRF24L01检测
* @param :无
* @note  :无
* @retval:无
*/ 
void NRF24L01_Check(void)
{
    uint8_t buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
    uint8_t read_buf[5] = {0, 0, 0, 0, 0};
    
    NRF24L01_BurstWrite(TX_ADDR, buf, 5);			//写入5个字节的地址
    NRF24L01_BurstRead(TX_ADDR, read_buf, 5);		//读出写入的地址  
    for(uint8_t i = 0; i<5; i++)
    {
        if(buf[i]!=read_buf[i])
        {
            T_Printf("NRF NOT FOUND!");
            NRF24L01_Delay500Ms(1);
            NRF24L01_Check();
        }
    } 
    return;
}

/**
* @brief :设置模式
* @param :
*			@Mode:模式发送模式或接收模式
* @note  :无
* @retval:无
*/
void NRF24L01_SetMode(nRf24l01ModeType Mode)
{
    uint8_t controlreg = NRF24L01_ReadReg(CONFIG);
    
    if(Mode == MODE_TX)       
    {
        controlreg &= ~(1U<<PRIM_RX);
    }
    else if(Mode == MODE_RX)  
    {
        controlreg |= (1U<<PRIM_RX);
    }
    
    NRF24L01_WriteReg(CONFIG, controlreg);
}

/**
* @brief :NRF24L01发送一次数据
* @param :
*			@txbuf:待发送数据首地址
*			@Length:发送数据长度
* @note  :无
* @retval:
*			MAX_TX：达到最大重发次数
*			TX_OK：发送完成
*			0xFF:其他原因
*/ 
uint8_t NRF24L01_TxPacket(void *txbuf, uint8_t Length)
{
    uint8_t l_Status = 0;
    uint16_t l_MsTimes = 0;
    
    NRF24L01_FlushTxFIFO();
    
    NRF24L01_CE_L;
    // NRF24L01_BurstWrite(WR_TX_PLOAD, txbuf, Length);	//写数据到TX BUF 32字节  TX_PLOAD_WIDTH                    
    NRF24L01_BurstWrite(WR_TX_PLOAD_NACK, (uint8_t*)txbuf, Length);
    NRF24L01_Delay(400);
    NRF24L01_CE_H;                                      //启动发送
    
    while( 0 != NRF24L01_IRQ)
    {
    	NRF24L01_Delay(400);
    	if( 500 == l_MsTimes++ )						//500ms还没有发送成功，重新初始化设备
    	{
            NRF24L01_Init();
            NRF24L01_SetMode(MODE_TX);
            break;
    	}
    }
    
    l_Status = NRF24L01_ReadReg(STATUS);						//读状态寄存器
    NRF24L01_WriteReg(STATUS, l_Status);						//清除TX_DS或MAX_RT中断标志
    
    if( l_Status & MAX_TX )	//达到最大重发次数
    {
        NRF24L01_WriteReg(FLUSH_TX, 0xff);	//清除TX FIFO寄存器
        return MAX_TX; 
    }
    if(l_Status & TX_OK)	//发送完成
    {
        return TX_OK;
    }
    
    return 0xFF;	//其他原因发送失败
}

/**
* @brief :NRF24L01接收数据
* @param :
*			@rxbuf:接收数据存放地址
* @note  :无
* @retval:接收的数据个数
*/ 
uint8_t NRF24L01_RxPacket(void *rxbuf)
{
    uint8_t l_Status = 0, l_RxLength = 0, l_100MsTimes = 0;
    
    NRF24L01_FlushRxFIFO();
    
    while(0 != NRF24L01_IRQ)
    {
        // NRF24L01_DelayMs(1);
        NRF24L01_Delay(400);
        
        if(255 == l_100MsTimes++)		//重收次数溢出依然没接收到数据，重新初始化模块
        {
            NRF24L01_Init();
            NRF24L01_SetMode(MODE_RX);
            break;
        }
    }
    
    l_Status = NRF24L01_GetStatusRegister();		//读状态寄存器
    NRF24L01_WriteReg(STATUS, l_Status);		//清中断标志
    if(l_Status & RX_OK)	//接收到数据
    {
        l_RxLength = NRF24L01_ReadReg(R_RX_PL_WID);		//读取接收到的数据个数
        NRF24L01_BurstRead(RD_RX_PLOAD, (uint8_t*)rxbuf, l_RxLength);	//接收到数据 
        NRF24L01_WriteReg(FLUSH_RX, 0xff);				//清除RX FIFO
        return l_RxLength; 
    }	
    
    return 0;				//没有收到数据	
}

/**
* @brief :NRF24L01中断处理函数
* @param :无
* @note  :无
* @retval:无
*/ 
void NRF24L01_TxRxISR(void)
{
    static uint8_t TxCNT = 0;
    uint8_t status = NRF24L01_GetStatusRegister();
    NRF24L01_ClearIRQnFlag(IRQ_ALL);
    switch(status>>4)
    {
        // 重发次数溢出中断
        case 0x01:
        TxCNT++;
        // NRF24L01_FlushTxFIFO();
        NRF24L01_FlushRxFIFO();
        GPIO_SetPinsOutput(GPIOE, 1U<<29U);         // PTE29亮
        GPIO_ClearPinsOutput(GPIOC, 1U<<0U);        // PTC0灭
        NRF24L01_SetMode(MODE_RX);
        break;
        
        // 接收中断
        case 0x04:
        NRF24L01_ReadRxPayload((uint8_t*)(&NRF_RxBuf[0]));// 读取接受数据
        GPIO_SetPinsOutput(GPIOC, 1U<<0U);          // PTC0亮
        GPIO_ClearPinsOutput(GPIOE, 1U<<29U);       // PTE29灭
        // NRF24L01_SetMode(MODE_TX);
        break;
        
        // 发送中断
        case 0x02:
        TxCNT = 0;
        GPIO_SetPinsOutput(GPIOE, 1U<<29U);         // PTE29亮
        GPIO_ClearPinsOutput(GPIOC, 1U<<0U);        // PTC0灭
        NRF24L01_SetMode(MODE_RX);
        break;
        
        default:
        NRF24L01_FlushTxFIFO();
        NRF24L01_FlushRxFIFO();
        GPIO_SetPinsOutput(GPIOE, 1U<<29U);         // PTE29亮
        GPIO_ClearPinsOutput(GPIOC, 1U<<0U);        // PTC0灭
        NRF24L01_SetMode(MODE_RX);
        break;
    }
    NRF24L01_CONFIG = NRF24L01_ReadReg(CONFIG);
}

/**
* @brief :RF24L01模块初始化
* @param :无
* @note  :无
* @retval:无
*/
void NRF24L01_Init(void)
{
    uint8_t addr[5] = {INIT_ADDR};
    
    NRF24L01_CE_L;
    NRF24L01_DelayMs(1);
    NRF24L01_CS_H;
    NRF24L01_DelayMs(1);
    // NRF24L01_CE_H;
    
    NRF24L01_ClearIRQnFlag(IRQ_ALL);
    
#if DYNAMIC_PACKET == 1
    NRF24L01_WriteReg(DYNPD, (1U<<0U)); 	                        // 使能通道0动态数据长度
    NRF24L01_WriteReg(FEATRUE, 0x07);
    NRF24L01_ReadReg(DYNPD);
    NRF24L01_ReadReg(FEATRUE);
#elif DYNAMIC_PACKET == 0
    // L01_WriteSingleReg( RX_PW_P0, FIXED_PACKET_LEN );	//固定数据长度
    NRF24L01_WriteReg(RX_PW_P0, FIXED_PACKET_LEN);	                // 固定数据长度
#endif	//DYNAMIC_PACKET
    
    NRF24L01_SetTxAddr(&addr[0], 5);                                // 设置TX地址
    NRF24L01_SetRxAddr(0, &addr[0], 5);                             // 设置RX地址
    NRF24L01_BurstRead(TX_ADDR, addr, 5);
    NRF24L01_BurstRead(RX_ADDR_P0+0, addr, 5);
    
    NRF24L01_WriteReg(EN_AA,    (1<<ENAA_P0));   		            // 设置通道0自动应答
    // NRF24L01_WriteReg(EN_AA,    (0<<ENAA_P0));   		            // 关闭通道0自动应答
    NRF24L01_WriteReg(EN_RXADDR, (1<<ERX_P0));		                // 设置只允许通道0接收
    
    NRF24L01_WriteReg(SETUP_RETR, ARD_4000US|(REPEAT_CNT&0x0F));    // 重发等待时间 250us
    // NRF24L01_WriteReg(SETUP_RETR, ARD_4000US|(0&0x0F));             // 重发次数=0
    NRF24L01_WriteReg(SETUP_AW, AW_5BYTES);     			        // 地址宽度5个字节
    
    // NRF24L01_CS_L;
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, NRF_WRITE_REG|RX_PW_P0);   // 通道0有效数据长度（5字节长度)
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xAA);                     // 通道0有效数据长度 高1字节
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xFF);                     // 通道0有效数据长度 高2字节
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xBB);                     // 通道0有效数据长度 高3字节
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xEE);                     // 通道0有效数据长度 高4字节
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xCC);                     // 通道0有效数据长度 高5字节
    // NRF24L01_CS_H;
    
    RF24LL01_SetHoppingPoint(73);                                   // 初始化通道号为60
    // NRF24L01_WriteReg(RF_CH, 0x3C);             			        // 初始化通道号为60
    
    NRF24L01_SetBaudrate(SPEED_250K);                                 // 初始化发射速率
    NRF24L01_SetPower(POWER_0DBM);                                 // 初始化输出功率
    // NRF24L01_WriteReg(RF_SETUP, 0x26);
    
    NRF24L01_WriteReg(CONFIG, (1U<<EN_CRC)|(1U<<PWR_UP));            // 上电NRF24L01
    
    NRF24L01_SetMode(MODE_TX);
    
    NRF24L01_CE_H;
}
