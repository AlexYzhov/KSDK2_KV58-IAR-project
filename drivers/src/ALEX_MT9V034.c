#include "include.h"

extern bool MT9V034_CaptureAccomplished;
extern volatile uint8_t MT9V034_IMGBUFF[MT9V034_SIZE];

void MT9V034_WriteReg(uint8_t SlaveAddr, uint8_t RegAddr, uint16_t RegVal)
{
    //SCCB_UnlockI2C();
    MT9V034_writereg_start:
    SCCB_Start();
    SCCB_WriteByte_soft(SlaveAddr, RegAddr, RegVal);
    SCCB_Stop();
    MT9V034_DelayMs(2);
    
    if((RegAddr !=  MT9V034_RESET) && (RegVal != MT9V034_ReadReg(SlaveAddr, RegAddr)))
    {
        goto MT9V034_writereg_start;
    }
    
    MT9V034_DelayMs(2);
    return;
}

uint16_t MT9V034_ReadReg(uint8_t SlaveAddr, uint8_t RegAddr)
{
    uint16_t val;
    val = SCCB_ReadByte_soft(SlaveAddr, RegAddr);
    SCCB_Stop();
    MT9V034_DelayMs(2);
    return val;
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
    DMA0_Transer_config.majorLoopCounts  = MT9V034_SIZE;                        // FV触发DMA一次Major MT9V034_SIZE (bytes)的数据
    //DMA0_Transer_config.majorLoopCounts  = MT9V034_W;                           // HSYNC触发DMA一次Major loop采集MT9V034_W (bytes)的数据
    DMA0_Transer_config.minorLoopBytes   = 0x01U;
    
    EDMA_ResetChannel(DMA0, MT9V034_DMA_CHANNEL);                                         // 复位DMACH0
    EDMA_SetTransferConfig(DMA0, MT9V034_DMA_CHANNEL, &DMA0_Transer_config, NULL);        // nexTCD / NULL
    
    EDMA_SetBandWidth(DMA0, MT9V034_DMA_CHANNEL, kEDMA_BandwidthStallNone);             // DMA0->CSR, 每读取一次DMA引擎stall 0个周期
    EDMA_EnableAutoStopRequest(DMA0, MT9V034_DMA_CHANNEL, true);                          // DMA0->CSR  主循环结束后自动停止硬件请求
    EDMA_EnableChannelInterrupts(DMA0, MT9V034_DMA_CHANNEL, kEDMA_MajorInterruptEnable);  // DMA0->CSR  主循环结束后产生中断
    EDMA_EnableChannelRequest(DMA0, MT9V034_DMA_CHANNEL);
    
    DMAMUX_SetSource(DMAMUX, MT9V034_DMA_CHANNEL, 18);         // 设置DMACH0触发源为XBARA_OUT0 {DMAMUX->CHCFG[0] = src(49)}
    DMAMUX_EnableChannel(DMAMUX, MT9V034_DMA_CHANNEL);         // 使能DMACH0传输
    //DMAMUX_SetSource(DMAMUX, MT9V034_DMA_CHANNEL, 49);         // 设置DMACH0触发源为PORTA {DMAMUX->CHCFG[0] = src(49)}
    //DMAMUX_EnableChannel(DMAMUX, MT9V034_DMA_CHANNEL);         // 使能DMACH0传输
    
    DisableIRQ(MT9V034_DMA_CHANNEL);
    DisableIRQ(PORTA_IRQn);                  // Disable Interrupts according to PORTA
    
    EDMA_ClearChannelStatusFlags(DMA0, MT9V034_DMA_CHANNEL, kEDMA_InterruptFlag);
    
    PORT_SetPinInterruptConfig(PORTA, 24U, kPORT_InterruptOrDMADisabled);    // PTA24 ~ PCLK.  不使能中断或DMA
    PORT_SetPinInterruptConfig(PORTA, 25U, kPORT_InterruptOrDMADisabled);    // PTA25 ~ HREF.  不使能中断或DMA
    //PORT_SetPinInterruptConfig(PORTA, 24U, kPORT_DMAFallingEdge);          // PTA24 ~ PCLK.  信号下降沿触发DMA传输
    //PORT_SetPinInterruptConfig(PORTA, 25U, kPORT_InterruptFallingEdge);    // PTA25 ~ HREF.  信号上升沿触发CPU中断
    PORT_SetPinInterruptConfig(PORTA, 26U, kPORT_InterruptRisingEdge);     // PTA26 ~ VSYNC. 信号上升沿触发CPU中断
    
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
    
    //MT9V034_SetReservedReg(MT9V034_I2C_ADDR);
    MT9V034_SetFrameResolution(MT9V034_I2C_ADDR, MT9V034_W, MT9V034_H);
    
    MT9V034_SetAutoExposure(MT9V034_I2C_ADDR, true);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0xAC, 0x0001);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0xAD, 0x01E0);
    
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x2C, 0x0004);
    
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x7F, 0x3000);           // test pattern
    
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0F, 0x0000);

    
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x08, 0x01BB);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x09, 0x01D9);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0xA5, 0x003A);
    
    //100dB HDR:
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x08, 0x03D4);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x09, 0x03E7);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0A, 0x0064);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0B, 0x03E8);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0F, 0x0103);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x35, 0x8010);
    
    //80dB HDR
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x08, 0x03CA);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x09, 0x03DE);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0A, 0x0064);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0B, 0x03E8);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x0F, 0x0103);
    //MT9V034_WriteReg(MT9V034_I2C_ADDR, 0x35, 0x8010);
    
    MT9V034_WriteReg(MT9V034_I2C_ADDR, 0xA5, 0x0030);
    
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
    MT9V034_DelayMs(10);
    
    //Unlock MT9V034, allowing user to initiate register settings and readout
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_CHIP_CONTROL, 0x0188);
        
    //Reset Again.
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_RESET, 0x0001);
    MT9V034_DelayMs(10);
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

    data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COL_FLIP);       //LQ-MT9V034 needs vertical mirror to capture correct image
    
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

    if(ISFR_FLAG&(1U<<26U))
    {
        //PORTA->ISFR |= (1U<<26U);
        //MT9V034_CaptureAccomplished = false;
        //PORTA->PCR[25U] = (PORTA->PCR[25U] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(kPORT_InterruptRisingEdge);
        //return;
        
        PORTA->ISFR |= (1U<<26U);
        MT9V034_CaptureAccomplished = false;
        
        DMA0->TCD[0].DADDR = (uint32_t)&MT9V034_IMGBUFF[0];
        //EDMA_EnableChannelRequest(DMA0, MT9V034_DMA_CHANNEL);
        DMA0->SERQ = DMA_SERQ_SERQ(MT9V034_DMA_CHANNEL);
    }
   
    //if(ISFR_FLAG&(1U<<25U))
    //{
    //    MT9V034_LineValid_Callback(line++);
    //    
    //    PORTA->ISFR |= (1U<<25U);
    //    
    //    if(line == MT9V034_H)
    //    {
    //        line = 0;
    //        MT9V034_CaptureAccomplished = true;
    //        PORTA->PCR[25U] = (PORTA->PCR[25U] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(kPORT_InterruptOrDMADisabled);
    //    }
    //}
}

__ramfunc void MT9V034_LineValid_Callback(uint16_t line)
{
    DMA0->TCD[0].DADDR = (uint32_t)&MT9V034_IMGBUFF[line*MT9V034_W];
    DMA0->SERQ = DMA_SERQ_SERQ(MT9V034_DMA_CHANNEL);
}

__ramfunc void MT9V034_DMA_Callback(void)
{
    DMA0->INT |= DMA_INT_INT0(1);
    MT9V034_CaptureAccomplished = true;
}

///////////////////////////////////////////////////////////////////////////////

uint8_t regTable[][3] = {
    {{0x00},{0x00}, {0x00}},
};