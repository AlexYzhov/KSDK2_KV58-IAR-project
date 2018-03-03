#include "include.h"

extern bool MT9V034_CaptureAccomplished;
extern volatile uint8_t MT9V034_IMGBUFF[MT9V034_SIZE];

void MT9V034_WriteReg(uint8_t SlaveAddr, uint8_t RegAddr, uint16_t RegVal)
{
    SCCB_UnlockI2C();
    
    MT9V034_writereg_start:
    
    SCCB_WriteByte_soft(SlaveAddr, RegAddr, (uint8_t)(RegVal>>8));
    SCCB_WriteByte_soft(SlaveAddr, 0xF0, (uint8_t)RegVal&0x00FF);
    SCCB_Stop();
    
    MT9V034_Delay(2);
    
    if((RegAddr !=  MT9V034_RESET) && (RegVal != MT9V034_ReadReg(SlaveAddr, RegAddr)))
    {
        goto MT9V034_writereg_start;
    }
    
    MT9V034_Delay(20);
    
    return;
}

uint16_t MT9V034_ReadReg(uint8_t SlaveAddr, uint8_t RegAddr)
{
    uint16_t val[2] = {0, 0};
    
    SCCB_UnlockI2C();

    val[0] = SCCB_ReadByte_soft(SlaveAddr, RegAddr);
    val[1] = SCCB_ReadByte_soft(SlaveAddr, 0xF0);
    
    SCCB_Stop();
    
    MT9V034_Delay(2);
    
    return (uint16_t)((val[0]<<8)|val[1]);
}

///////////////////////////////////////////////////////////////////////////////

void MT9V034_DMA_Init(uint8_t* pMT9V032_IMG_Buff)
{
    edma_transfer_config_t DMA0_Transer_config;

    DMA0_Transer_config.srcAddr          = (uint32_t)&(GPIOB->PDIR);            // 数据源来自PORTB0
    DMA0_Transer_config.srcOffset        = 0x00U;                               // 数据源PORTB偏移量=0
    DMA0_Transer_config.destAddr         = (uint32_t)pMT9V032_IMG_Buff;         // pMT9V032_IMG_Buff
    DMA0_Transer_config.destOffset       = 0x01U;                               // 数据源指向地址每次自增1 byte
    DMA0_Transer_config.destTransferSize = kEDMA_TransferSize1Bytes;            // DMA每次传输1byte
    DMA0_Transer_config.srcTransferSize  = kEDMA_TransferSize1Bytes;            // DMA每次传输1byte
    DMA0_Transer_config.majorLoopCounts  = MT9V034_MAX_WIDTH;                      // VSYNC触发DMA一次Major loop采集CAMERA_DMA_NUM (bytes)的数据
    DMA0_Transer_config.minorLoopBytes   = 0x01U;
    
    EDMA_ResetChannel(DMA0, MT9V034_DMA_CHANNEL);                                         // 复位DMACH0
    EDMA_SetTransferConfig(DMA0, MT9V034_DMA_CHANNEL, &DMA0_Transer_config, NULL);        // nexTCD / NULL
    
    EDMA_SetBandWidth(DMA0, MT9V034_DMA_CHANNEL, kEDMA_BandwidthStallNone);             // DMA0->CSR, 每读取一次DMA引擎stall 0个周期
    EDMA_EnableAutoStopRequest(DMA0, MT9V034_DMA_CHANNEL, true);                          // DMA0->CSR  主循环结束后自动停止硬件请求
    EDMA_EnableChannelInterrupts(DMA0, MT9V034_DMA_CHANNEL, kEDMA_MajorInterruptEnable);  // DMA0->CSR  主循环结束后产生中断
    
    DMAMUX_SetSource(DMAMUX, MT9V034_DMA_CHANNEL, 49);         // 设置DMACH0触发源为PORTA {DMAMUX->CHCFG[0] = src(49)}
    DMAMUX_EnableChannel(DMAMUX, MT9V034_DMA_CHANNEL);         // 使能DMACH0传输
    
    DisableIRQ(MT9V034_DMA_CHANNEL);
    DisableIRQ(PORTA_IRQn);                  // Disable Interrupts according to PORTA
    
    EDMA_ClearChannelStatusFlags(DMA0, MT9V034_DMA_CHANNEL, kEDMA_InterruptFlag);
    
    PORT_SetPinInterruptConfig(PORTA, 24U, kPORT_DMARisingEdge);          // PTA24 ~ PCLK.  信号下降沿触发DMA传输
    PORT_SetPinInterruptConfig(PORTA, 25U, kPORT_InterruptRisingEdge);     // PTA25 ~ VSYNC. 信号上升沿触发CPU中断
    PORT_SetPinInterruptConfig(PORTA, 26U, kPORT_InterruptRisingEdge);     // PTA26 ~ HREF.  信号上升沿触发CPU中断
    
    EnableIRQ(MT9V034_DMA_CHANNEL);
    EnableIRQ(PORTA_IRQn);
}

bool MT9V034_Reg_Init(uint8_t MT9V034_I2C_ADDR)
{
    uint16_t id = MT9V034_ReadReg(MT9V034_I2C_ADDR, 0x00);
    if(MT9V034_DEFAULT_VERSION != id)
    {
        return false;
    }
    
    MT9V034_Reset(MT9V034_I2C_ADDR);
    
    MT9V034_SetReservedReg(MT9V034_I2C_ADDR);
    MT9V034_SetFrameResolution(MT9V034_I2C_ADDR, MT9V034_W, MT9V034_H);
    
    MT9V034_SetAutoExposure(MT9V034_I2C_ADDR, true);
    
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0xC6, 0x00);

    return true;
}

void MT9V034_Init(void)
{
    if(GPIOA->PDIR&(1U<<16U))              //DKEY6, ���¼���ʼ��
    {
        MT9V034_Reg_Init(MT9V034_SLAVE_ADDRESS);
    }
    
    MT9V034_DMA_Init(MT9V034_IMGBUFF);
}

///////////////////////////////////////////////////////////////////////////////

void MT9V034_Reset(uint8_t MT9V034_I2C_ADDR)
{
    //Reset MT9V034, but register config will not change.
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_RESET, 0x0001);
    MT9V034_Delay(10);
    
    //Unlock MT9V034, allowing user to initiate register settings and readout
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_CHIP_CONTROL, 0x0088);
        
    //Reset Again.
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_RESET, 0x0001);
    MT9V034_Delay(10);
}

void MT9V034_SetReservedReg(uint8_t MT9V034_I2C_ADDR)
{
    //Here we write some reserved registers as recommendations from Rev.G datasheet, Table.8
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x13, 0x2D2E);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x20, 0x03C7);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x24, 0x001B);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x2B, 0x0003);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x2F, 0x0003);
}

void MT9V034_SetFrameRate(uint8_t MT9V034_I2C_ADDR, uint8_t frameRate)
{
    
}

void MT9V034_SetFrameResolution(uint8_t MT9V034_I2C_ADDR, uint16_t width, uint16_t height)
{
    uint16_t data = 0;

    if((width*4)<=MT9V034_MAX_WIDTH && (height*4)<=MT9V034_MAX_HEIGHT)
    {
        width *= 4;
        height *= 4;
        data |= MT9V034_READ_MODE_ROW_BIN_4;
        data |= MT9V034_READ_MODE_COL_BIN_4;
    }
    else if((width*2)<=MT9V034_MAX_WIDTH && (height*2)<=MT9V034_MAX_HEIGHT)
    {
        width *= 2;
        height *= 2;
        data |= MT9V034_READ_MODE_ROW_BIN_2;
        data |= MT9V034_READ_MODE_COL_BIN_2;
    }

    data |= (MT9V034_READ_MODE_ROW_FLIP);       //LQ-MT9V034 needs vertical mirror to capture correct image
    
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_READ_MODE, data);

    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_WINDOW_WIDTH,  width);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_WINDOW_HEIGHT, height);
    
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_COL_START, (MT9V034_MAX_WIDTH-width)/2+MT9V034_COL_START_MIN);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_ROW_START, (MT9V034_MAX_HEIGHT-height)/2+MT9V034_ROW_START_MIN);
}

void MT9V034_SetAutoExposure(uint8_t MT9V034_I2C_ADDR, bool enable)
{
    uint16_t reg = MT9V034_ReadReg(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE);
    if(true == enable)
    {
        MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg|MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE);
    }
    else
    {
        MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg&~(MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE));
    }
}

///////////////////////////////////////////////////////////////////////////////

__ramfunc void MT9V034_FrameValid_Callback(uint32_t ISFR_FLAG)
{
    static uint16_t line = 0;

    if(ISFR_FLAG&(1U<<25U))
    {
        PORTA->ISFR |= (1U<<25U);
        MT9V034_CaptureAccomplished = false;
        return;
    }
   
    if(ISFR_FLAG&(1U<<26U))
    {
        MT9V034_LineValid_Callback(line++);
        
        PORTA->ISFR |= (1U<<26U);
        
        if(line == MT9V034_H-1)
        {
            line = 0;
            MT9V034_CaptureAccomplished = true;
        }
    }
}

__ramfunc void MT9V034_LineValid_Callback(uint16_t line)
{
    DMA0->TCD[0].DADDR = (uint32_t)&MT9V034_IMGBUFF[line*MT9V034_W];
    DMA0->SERQ = DMA_SERQ_SERQ(MT9V034_DMA_CHANNEL);
}

__ramfunc void MT9V034_DMA_Callback(void)
{
    DMA0->INT |= DMA_INT_INT0(1);
}