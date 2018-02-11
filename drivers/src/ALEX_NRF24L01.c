#include "ALEX_NRF24L01.h"

dspi_command_data_config_t SPI1_NRF24L01_Ctar0 =
{
    .isPcsContinuous = false,            // ���ͽ������ͷ�cs�������������cs
    .whichCtar = kDSPI_Ctar0,
    .whichPcs = kDSPI_Pcs0,
    .clearTransferCount = false,
    .isEndOfQueue = false,
};

dspi_command_data_config_t SPI1_NRF24L01_Ctar1 =
{
    .isPcsContinuous = true,            // ���ͽ������ͷ�cs�������������cs
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
* @brief :���RX������
* @param :��
* @note  :��
* @retval:��
*/
inline void NRF24L01_FlushRxFIFO(void)
{
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, FLUSH_RX);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :���TX������
* @param :��
* @note  :��
* @retval:��
*/
inline void NRF24L01_FlushTxFIFO(void)
{
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, FLUSH_TX);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :������һ�����ݰ�
* @param :��
* @note  :��
* @retval:��
*/
inline void NRF24L01_ReuseLastTxPayload(void)
{
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, REUSE_TX_PL);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :NRF24L01��ָ��
* @param :��
* @note  :��
* @retval:��
*/
inline void NRF24L01_NOP(void)
{
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, NOP);
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :��ȡNRF24L01״̬�Ĵ���ֵ
* @param :��
* @note  :��
* @retval:RF24L01״̬
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
* @brief :��FIFO�����ݿ��
* @param :��
* @note  :��
* @retval:���ݿ��
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
* @brief :��RF24L01�ж�״̬
* @param :��
* @note  :��
* @retval:�ж�״̬
*/
inline uint8_t NRF24L01_GetIRQnStatus()
{
    return (NRF24L01_GetStatusRegister()&((1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT)));	//�����ж�״̬
}

/**
* @brief :��NRF24L01�ж�
* @param :
@IRQ_Source:�ж�Դ
* @note  :��
* @retval:�����״̬�Ĵ�����ֵ
*/
uint8_t NRF24L01_ClearIRQnFlag(uint8_t IRQ_Source)
{
    uint8_t status = NRF24L01_GetStatusRegister();			            //��״̬�Ĵ���
    IRQ_Source &= (1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT);	                            //�жϱ�־����	
    NRF24L01_CS_L;			//Ƭѡ
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, NRF_WRITE_REG|STATUS);	    //д״̬�Ĵ�������
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1,    IRQ_Source|status);		//����Ӧ�жϱ�־
    NRF24L01_CS_H;			//ȡ��Ƭѡ
    NRF24L01_Delay(400);
    return NRF24L01_GetStatusRegister();			//����״̬�Ĵ���״̬
}

/**
* @brief :�����յ�������
* @param :��
* @note  :��
* @retval:
@pRxBuf:���ݴ�ŵ�ַ�׵�ַ
*/
uint8_t NRF24L01_ReadRxPayload(uint8_t *pBuff)
{
    uint8_t PipeNum = (NRF24L01_ReadReg(STATUS)>>1)&0x07;
    uint8_t Width   = NRF24L01_GetFIFOWidth();
    
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, RD_RX_PLOAD);      // ����Ч��������
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
* @brief :�������ݣ���Ӧ��
* @param :
*			@pTxBuf:�������ݵ�ַ
*			@len:����
* @note  :һ�β�����32���ֽ�
* @retval:��
*/
void NRF24L01_WriteTxPayload_ACK(uint8_t *pBuff, uint8_t len)
{
    NRF24L01_FlushTxFIFO();                 // ���TX FIFO
    
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, WR_TX_PLOAD);
    for(uint8_t i=0; i<((len>32)?32:len); i++)          // ����������ַ�������Ϊ32
    {
        DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, pBuff[i]);
        NRF24L01_Delay(400);
    }
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :�������ݣ�����Ӧ��
* @param :
*			@pTxBuf:�������ݵ�ַ
*			@len:����
* @note  :һ�β�����32���ֽ�
* @retval:��
*/
void NRF24L01_WriteTxPayload_NOACK(uint8_t *pBuff, uint8_t len)
{
    if(len>32||len==0)
    {
        return;
    }
    
    NRF24L01_FlushTxFIFO();                 // ���TX FIFO
    
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
* @brief :�ڽ���ģʽ����TX FIFOд����(��ACK)
* @param :
*			@pData:���ݵ�ַ
*			@len:����
* @note  :һ�β�����32���ֽ�
* @retval:��
*/
void NRF24L01_WriteTxPayload_InRx_ACK(uint8_t *pBuff, uint8_t len)
{
    NRF24L01_CS_L;
    DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, W_ACK_PLOAD);
    for(uint8_t i=0; i<((len>32)?32:len); i++)          // ����������ַ�������Ϊ32
    {
        DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, pBuff[i]);
        NRF24L01_Delay(400);
    }
    NRF24L01_CS_H;
    NRF24L01_Delay(400);
}

/**
* @brief :���÷��͵�ַ
* @param :
*			@pAddr:��ַ��ŵ�ַ
*			@len:����
* @note  :��
* @retval:��
*/
inline void NRF24L01_SetTxAddr(uint8_t *pBuff, uint8_t len)
{
    len = (len>5)?5:len;
    NRF24L01_BurstWrite(TX_ADDR, pBuff, len);         // ���Ƶ�ַС��5���ֽ�
}

/**
* @brief :���ý���ͨ����ַ
* @param :
*			@PipeNum:ͨ��
*			@pAddr:��ַ����ŵ�ַ
*			@Len:����
* @note  :ͨ��������5 ��ַ���Ȳ�����5���ֽ�
* @retval:��
*/
inline void NRF24L01_SetRxAddr(uint8_t PipeNum, uint8_t *pBuff, uint8_t len)
{
    PipeNum = (PipeNum>5)?5:PipeNum;
    len = (len>5)?5:len;
    NRF24L01_BurstWrite(RX_ADDR_P0+PipeNum, pBuff, len);         // ����ͨ����С��5�����Ƶ�ַС��5���ֽ�
}

/**
* @brief :����ͨ���ٶ�
* @param :
*			@Baudrate:������
* @note  :��
* @retval:��
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
* @brief :���ù���
* @param :
*			@Speed:�ٶ�
* @note  :��
* @retval:��
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
* @brief :����Ƶ��
* @param :
*			@FreqPoint:Ƶ�����ò���
* @note  :ֵ������127
* @retval:��
*/
inline void RF24LL01_SetHoppingPoint(uint8_t FreqPoint)
{
    NRF24L01_WriteReg(RF_CH, FreqPoint&0x7F);
}

/**
* @brief :NRF24L01���
* @param :��
* @note  :��
* @retval:��
*/ 
void NRF24L01_Check(void)
{
    uint8_t buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
    uint8_t read_buf[5] = {0, 0, 0, 0, 0};
    
    NRF24L01_BurstWrite(TX_ADDR, buf, 5);			//д��5���ֽڵĵ�ַ
    NRF24L01_BurstRead(TX_ADDR, read_buf, 5);		//����д��ĵ�ַ  
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
* @brief :����ģʽ
* @param :
*			@Mode:ģʽ����ģʽ�����ģʽ
* @note  :��
* @retval:��
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
* @brief :NRF24L01����һ������
* @param :
*			@txbuf:�����������׵�ַ
*			@Length:�������ݳ���
* @note  :��
* @retval:
*			MAX_TX���ﵽ����ط�����
*			TX_OK���������
*			0xFF:����ԭ��
*/ 
uint8_t NRF24L01_TxPacket(void *txbuf, uint8_t Length)
{
    uint8_t l_Status = 0;
    uint16_t l_MsTimes = 0;
    
    NRF24L01_FlushTxFIFO();
    
    NRF24L01_CE_L;
    // NRF24L01_BurstWrite(WR_TX_PLOAD, txbuf, Length);	//д���ݵ�TX BUF 32�ֽ�  TX_PLOAD_WIDTH                    
    NRF24L01_BurstWrite(WR_TX_PLOAD_NACK, (uint8_t*)txbuf, Length);
    NRF24L01_Delay(400);
    NRF24L01_CE_H;                                      //��������
    
    while( 0 != NRF24L01_IRQ)
    {
    	NRF24L01_Delay(400);
    	if( 500 == l_MsTimes++ )						//500ms��û�з��ͳɹ������³�ʼ���豸
    	{
            NRF24L01_Init();
            NRF24L01_SetMode(MODE_TX);
            break;
    	}
    }
    
    l_Status = NRF24L01_ReadReg(STATUS);						//��״̬�Ĵ���
    NRF24L01_WriteReg(STATUS, l_Status);						//���TX_DS��MAX_RT�жϱ�־
    
    if( l_Status & MAX_TX )	//�ﵽ����ط�����
    {
        NRF24L01_WriteReg(FLUSH_TX, 0xff);	//���TX FIFO�Ĵ���
        return MAX_TX; 
    }
    if(l_Status & TX_OK)	//�������
    {
        return TX_OK;
    }
    
    return 0xFF;	//����ԭ����ʧ��
}

/**
* @brief :NRF24L01��������
* @param :
*			@rxbuf:�������ݴ�ŵ�ַ
* @note  :��
* @retval:���յ����ݸ���
*/ 
uint8_t NRF24L01_RxPacket(void *rxbuf)
{
    uint8_t l_Status = 0, l_RxLength = 0, l_100MsTimes = 0;
    
    NRF24L01_FlushRxFIFO();
    
    while(0 != NRF24L01_IRQ)
    {
        // NRF24L01_DelayMs(1);
        NRF24L01_Delay(400);
        
        if(255 == l_100MsTimes++)		//���մ��������Ȼû���յ����ݣ����³�ʼ��ģ��
        {
            NRF24L01_Init();
            NRF24L01_SetMode(MODE_RX);
            break;
        }
    }
    
    l_Status = NRF24L01_GetStatusRegister();		//��״̬�Ĵ���
    NRF24L01_WriteReg(STATUS, l_Status);		//���жϱ�־
    if(l_Status & RX_OK)	//���յ�����
    {
        l_RxLength = NRF24L01_ReadReg(R_RX_PL_WID);		//��ȡ���յ������ݸ���
        NRF24L01_BurstRead(RD_RX_PLOAD, (uint8_t*)rxbuf, l_RxLength);	//���յ����� 
        NRF24L01_WriteReg(FLUSH_RX, 0xff);				//���RX FIFO
        return l_RxLength; 
    }	
    
    return 0;				//û���յ�����	
}

/**
* @brief :NRF24L01�жϴ�����
* @param :��
* @note  :��
* @retval:��
*/ 
void NRF24L01_TxRxISR(void)
{
    static uint8_t TxCNT = 0;
    uint8_t status = NRF24L01_GetStatusRegister();
    NRF24L01_ClearIRQnFlag(IRQ_ALL);
    switch(status>>4)
    {
        // �ط���������ж�
        case 0x01:
        TxCNT++;
        // NRF24L01_FlushTxFIFO();
        NRF24L01_FlushRxFIFO();
        GPIO_SetPinsOutput(GPIOE, 1U<<29U);         // PTE29��
        GPIO_ClearPinsOutput(GPIOC, 1U<<0U);        // PTC0��
        NRF24L01_SetMode(MODE_RX);
        break;
        
        // �����ж�
        case 0x04:
        NRF24L01_ReadRxPayload((uint8_t*)(&NRF_RxBuf[0]));// ��ȡ��������
        GPIO_SetPinsOutput(GPIOC, 1U<<0U);          // PTC0��
        GPIO_ClearPinsOutput(GPIOE, 1U<<29U);       // PTE29��
        // NRF24L01_SetMode(MODE_TX);
        break;
        
        // �����ж�
        case 0x02:
        TxCNT = 0;
        GPIO_SetPinsOutput(GPIOE, 1U<<29U);         // PTE29��
        GPIO_ClearPinsOutput(GPIOC, 1U<<0U);        // PTC0��
        NRF24L01_SetMode(MODE_RX);
        break;
        
        default:
        NRF24L01_FlushTxFIFO();
        NRF24L01_FlushRxFIFO();
        GPIO_SetPinsOutput(GPIOE, 1U<<29U);         // PTE29��
        GPIO_ClearPinsOutput(GPIOC, 1U<<0U);        // PTC0��
        NRF24L01_SetMode(MODE_RX);
        break;
    }
    NRF24L01_CONFIG = NRF24L01_ReadReg(CONFIG);
}

/**
* @brief :RF24L01ģ���ʼ��
* @param :��
* @note  :��
* @retval:��
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
    NRF24L01_WriteReg(DYNPD, (1U<<0U)); 	                        // ʹ��ͨ��0��̬���ݳ���
    NRF24L01_WriteReg(FEATRUE, 0x07);
    NRF24L01_ReadReg(DYNPD);
    NRF24L01_ReadReg(FEATRUE);
#elif DYNAMIC_PACKET == 0
    // L01_WriteSingleReg( RX_PW_P0, FIXED_PACKET_LEN );	//�̶����ݳ���
    NRF24L01_WriteReg(RX_PW_P0, FIXED_PACKET_LEN);	                // �̶����ݳ���
#endif	//DYNAMIC_PACKET
    
    NRF24L01_SetTxAddr(&addr[0], 5);                                // ����TX��ַ
    NRF24L01_SetRxAddr(0, &addr[0], 5);                             // ����RX��ַ
    NRF24L01_BurstRead(TX_ADDR, addr, 5);
    NRF24L01_BurstRead(RX_ADDR_P0+0, addr, 5);
    
    NRF24L01_WriteReg(EN_AA,    (1<<ENAA_P0));   		            // ����ͨ��0�Զ�Ӧ��
    // NRF24L01_WriteReg(EN_AA,    (0<<ENAA_P0));   		            // �ر�ͨ��0�Զ�Ӧ��
    NRF24L01_WriteReg(EN_RXADDR, (1<<ERX_P0));		                // ����ֻ����ͨ��0����
    
    NRF24L01_WriteReg(SETUP_RETR, ARD_4000US|(REPEAT_CNT&0x0F));    // �ط��ȴ�ʱ�� 250us
    // NRF24L01_WriteReg(SETUP_RETR, ARD_4000US|(0&0x0F));             // �ط�����=0
    NRF24L01_WriteReg(SETUP_AW, AW_5BYTES);     			        // ��ַ���5���ֽ�
    
    // NRF24L01_CS_L;
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, NRF_WRITE_REG|RX_PW_P0);   // ͨ��0��Ч���ݳ��ȣ�5�ֽڳ���)
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xAA);                     // ͨ��0��Ч���ݳ��� ��1�ֽ�
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xFF);                     // ͨ��0��Ч���ݳ��� ��2�ֽ�
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xBB);                     // ͨ��0��Ч���ݳ��� ��3�ֽ�
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xEE);                     // ͨ��0��Ч���ݳ��� ��4�ֽ�
    // DSPI_MasterWriteDataBlocking(SPI1, &SPI1_NRF24L01_Ctar1, 0xCC);                     // ͨ��0��Ч���ݳ��� ��5�ֽ�
    // NRF24L01_CS_H;
    
    RF24LL01_SetHoppingPoint(73);                                   // ��ʼ��ͨ����Ϊ60
    // NRF24L01_WriteReg(RF_CH, 0x3C);             			        // ��ʼ��ͨ����Ϊ60
    
    NRF24L01_SetBaudrate(SPEED_250K);                                 // ��ʼ����������
    NRF24L01_SetPower(POWER_0DBM);                                 // ��ʼ���������
    // NRF24L01_WriteReg(RF_SETUP, 0x26);
    
    NRF24L01_WriteReg(CONFIG, (1U<<EN_CRC)|(1U<<PWR_UP));            // �ϵ�NRF24L01
    
    NRF24L01_SetMode(MODE_TX);
    
    NRF24L01_CE_H;
}
