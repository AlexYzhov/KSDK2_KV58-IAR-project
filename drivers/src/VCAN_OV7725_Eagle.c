/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_OV7725_Eagle.c
 * @brief      鹰眼OV7725驱动代码
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-07
 */

#include "include.h"

#define DEBUG_PRINTF DbgConsole_Printf
#define OV7725_EAGLE_Delay_ms(ms)  ALEX_lptmr_DelayMs(ms)


uint8_t   *OV7725_Eagle_IMG_Buff;

volatile IMG_STATUS_e OV7725_Eagle_img_flag = IMG_FINISH;   //图像状态

//内部函数声明
static uint8_t OV7725_Eagle_reg_Init(void);
static void OV7725_Eagle_Port_Init();


/*!
 *  @brief      鹰眼OV7725初始化
 *  @since      v5.0
 */
uint8_t OV7725_Eagle_Init(uint8_t *imgaddr)
{
    OV7725_Eagle_IMG_Buff = imgaddr;
    while(OV7725_Eagle_reg_Init() == 0);
    OV7725_Eagle_Port_Init();
    return 0;
}

/*!
 *  @brief      鹰眼OV7725管脚初始化（内部调用）
 *  @since      v5.0
 */
void OV7725_Eagle_Port_Init()
{
    edma_transfer_config_t DMA0_Transer_config;
    // edma_tcd_t nextTCD;
    
    DMA0_Transer_config.srcAddr          = (uint32_t)&(GPIOB->PDIR);            // 数据源来自PORTB0
    DMA0_Transer_config.srcOffset        = 0x00U;                               // 数据源PORTB偏移量=0
    DMA0_Transer_config.destAddr         = (uint32_t)OV7725_Eagle_IMG_Buff;     // 数据源指向OV7725_Eagle_IMG_Buff
    DMA0_Transer_config.destOffset       = 0x01U;                               // 数据源指向地址每次自增1 byte
    DMA0_Transer_config.destTransferSize = kEDMA_TransferSize1Bytes;            // DMA每次传输1byte
    DMA0_Transer_config.srcTransferSize  = kEDMA_TransferSize1Bytes;            // DMA每次传输1byte
    DMA0_Transer_config.majorLoopCounts  = CAMERA_DMA_NUM;                      // VSYNC触发DMA一次Major loop采集CAMERA_DMA_NUM (bytes)的数据
    DMA0_Transer_config.minorLoopBytes   = 0x01U;                               // 每个Minor loop发送1byte数据
    
    // nextTCD.DLAST_SGA  &= -CAMERA_DMA_NUM;
    // nextTCD.SLAST      &= DMA_SLAST_SLAST(0);
    
    EDMA_ResetChannel(DMA0, 0);                                         // 复位DMACH0
    
    EDMA_SetTransferConfig(DMA0, 0, &DMA0_Transer_config, NULL);        // nexTCD / NULL
    
    EDMA_SetBandWidth(DMA0, 0, kEDMA_BandwidthStall8Cycle);             // DMA0->CSR, 每读取一次DMA引擎stall 8个周期
    EDMA_EnableAutoStopRequest(DMA0, 0, true);                         // DMA0->CSR  主循环结束后自动停止硬件请求
    EDMA_EnableChannelInterrupts(DMA0, 0, kEDMA_MajorInterruptEnable);  // DMA0->CSR  主循环结束后产生中断
    
    DMAMUX_SetSource(DMAMUX, 0, 49);         // 设置DMACH0触发源为PORTA {DMAMUX->CHCFG[0] = src(49)}
    DMAMUX_EnableChannel(DMAMUX, 0);         // 使能DMACH0传输
    
    DisableIRQ(CAMERA_DMA_CH);
    DisableIRQ(PORTA_IRQn);                  // Disable Interrupts according to PORTA
    EDMA_ClearChannelStatusFlags(DMA0, 0, kEDMA_InterruptFlag);
    
    PORT_SetPinInterruptConfig(PORTA, 24U, kPORT_DMAFallingEdge);          // PTA24 ~ PCLK.  信号下降沿触发DMA传输
    PORT_SetPinInterruptConfig(PORTA, 25U, kPORT_InterruptFallingEdge);    // PTA25 ~ VSYNC. 信号下降沿触发CPU中断
}

/*!
 *  @brief      鹰眼OV7725场中断服务函数
 *  @since      v5.0
 */
void OV7725_Eagle_VSYNC(uint8_t* IMG)
{
    OV7725_Eagle_IMG_Buff = IMG;
    OV7725_Eagle_img_flag = IMG_START;
    //场中断需要判断是场结束还是场开始
    if(OV7725_Eagle_img_flag == IMG_START)                   //需要开始采集图像
    {
        DMA0->TCD[0].DADDR = (uint32_t)OV7725_Eagle_IMG_Buff;  
        OV7725_Eagle_img_flag = IMG_GATHER;                  //标记图像采集中        
        EDMA_EnableChannelRequest(DMA0, 0);
        EnableIRQ(CAMERA_DMA_CH);
    }
    else          //图像采集错误
    {          
        OV7725_Eagle_img_flag = IMG_FAIL;                    //标记图像采集失败
    }
}

/*!
 *  @brief      鹰眼OV7725 DMA中断服务函数
 *  @since      v5.0
 */
void OV7725_Eagle_DMA()
{
    DisableIRQ(CAMERA_DMA_CH);
    DMA0->INT |= DMA_INT_INT0(1);
    OV7725_Eagle_img_flag = IMG_FINISH;
    
    //PORTA->ISFR = ~0;                              //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
    EnableIRQ(PORTA_IRQn);  
}

/*!
 *  @brief      鹰眼OV7725采集图像（采集到的数据存储在 初始化时配置的地址上）
 *  @since      v5.0
 */
void OV7725_Eagle_Get_Img()
{
    OV7725_Eagle_img_flag = IMG_START;             //开始采集图像          
    PORTA->ISFR = ~0;                              //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
    EnableIRQ(PORTA_IRQn);                         //允许PTA的中断
    // while(OV7725_Eagle_img_flag == IMG_FINISH)    //等待图像采集完毕
    // {
    //     if(OV7725_Eagle_img_flag == IMG_FAIL)      //假如图像采集错误，则重新开始采集
    //     {
    //         OV7725_Eagle_img_flag = IMG_START;     //开始采集图像
    //         PORTA->ISFR = ~0;                      //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
    //         EnableIRQ(PORTA_IRQn);                 //允许PTA的中断
    //     }
    // }
}

/*!
 *  @brief      鹰眼OV7725寄存器 初始化
 *  @return     初始化结果（0表示失败，1表示成功）
 *  @since      v5.0
 */
uint8_t OV7725_Eagle_reg_Init(void)
{
    uint16_t i = 0;
    uint8_t Sensor_IDCode = 0;

    // I2C_WriteByte (I2C3, OV7725_ADDR, OV7725_COM7, 0x80);
    SCCB_WriteByte_soft(OV7725_ADDR, OV7725_COM7, 0x80);

    OV7725_EAGLE_Delay_ms(200);
    
    // Sensor_IDCode = SCCB_ReadByte(I2C3, OV7725_ADDR, OV7725_VER);
    Sensor_IDCode = SCCB_ReadByte_soft(OV7725_ADDR, OV7725_VER);
    
    DEBUG_PRINTF("\nGet ID success，SENSOR ID is 0x%x", Sensor_IDCode);
    DEBUG_PRINTF("\nConfig Register Number is %d ", OV7725_Eagle_cfgnum);
    
    OV7725_EAGLE_Delay_ms(10);
    
    if(Sensor_IDCode == OV7725_ID)
    {
        for( i = 0 ; i < OV7725_Eagle_cfgnum ; i++ )
        {
            // I2C_WriteByte(I2C3, OV7725_ADDR, OV7725_Eagle_reg[i].Addr, OV7725_Eagle_reg[i].val);
            SCCB_WriteByte_soft(OV7725_ADDR, OV7725_Eagle_reg[i].Addr, OV7725_Eagle_reg[i].val);
            
            OV7725_EAGLE_Delay_ms(2);
        }
    }
    else
    {
        // uint8_t dummy = I2C3->D;                // 无意义读取一次，释放总线
        OV7725_Eagle_reg_Init();                // 递归调用自己，重复配置7725直到成功
        return 0;
    }
    DEBUG_PRINTF("\nOV7725 Register Config Success!");
    return 1;
}

/* 图像解压函数 */
void Img_Extract(void *dst, void *src, uint32_t srclen)
{
    uint8_t colour[2] = {255, 0}; //0 和 1 分别对应的颜色
    uint8_t * mdst = dst;
    uint8_t * msrc = src;
    //注：山外的摄像头 0 表示 白色，1表示 黑色
    uint8_t tmpsrc;
    while(srclen --)
    {
        tmpsrc = *msrc++;
        *mdst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}

/*OV7725初始化配置表*/
reg_s OV7725_Eagle_reg[] =
{
    //寄存器，寄存器值次                // 50fps    75fps   112fps  150fps
    {OV7725_COM4         , 0xC1},       // 0xC1     0x41    0x81    0xC1
    {OV7725_CLKRC        , 0x02},       // 0x02     0x00    0x00    0x00
    {OV7725_COM2         , 0x03},
    {OV7725_COM3         , 0xD0},
    {OV7725_COM7         , 0x40},
    
    {OV7725_COM8         , 0xFF},       // 自动增益/工频滤波设置
    {OV7725_BDBase       , 0x98},
    {OV7725_BDMStep      , 0x03},
    {OV7725_CNST         , 0x35},
    
    {OV7725_HSTART       , 0x3F},
    {OV7725_HSIZE        , 0x50},
    {OV7725_VSTRT        , 0x03},
    {OV7725_VSIZE        , 0x78},
    {OV7725_HREF         , 0x00},
    {OV7725_SCAL0        , 0x0A},
    {OV7725_AWB_Ctrl0    , 0xE0},
    {OV7725_DSPAuto      , 0xff},
    {OV7725_DSP_Ctrl2    , 0x0C},
    {OV7725_DSP_Ctrl3    , 0x00},
    {OV7725_DSP_Ctrl4    , 0x00},

#if (CAMERA_W == 80)
    {OV7725_HOutSize     , 0x14},
#elif (CAMERA_W == 160)
    {OV7725_HOutSize     , 0x28},
#elif (CAMERA_W == 240)
    {OV7725_HOutSize     , 0x3c},
#elif (CAMERA_W == 320)
    {OV7725_HOutSize     , 0x50},
#else

#endif

#if (CAMERA_H == 60 )
    {OV7725_VOutSize     , 0x1E},
#elif (CAMERA_H == 120 )
    {OV7725_VOutSize     , 0x3c},
#elif (CAMERA_H == 180 )
    {OV7725_VOutSize     , 0x5a},
#elif (CAMERA_H == 240 )
    {OV7725_VOutSize     , 0x78},
#else

#endif

    {OV7725_EXHCH        , 0x00},
    {OV7725_GAM1         , 0x0c},
    {OV7725_GAM2         , 0x16},
    {OV7725_GAM3         , 0x2a},
    {OV7725_GAM4         , 0x4e},
    {OV7725_GAM5         , 0x61},
    {OV7725_GAM6         , 0x6f},
    {OV7725_GAM7         , 0x7b},
    {OV7725_GAM8         , 0x86},
    {OV7725_GAM9         , 0x8e},
    {OV7725_GAM10        , 0x97},
    {OV7725_GAM11        , 0xa4},
    {OV7725_GAM12        , 0xaf},
    {OV7725_GAM13        , 0xc5},
    {OV7725_GAM14        , 0xd7},
    {OV7725_GAM15        , 0xe8},
    {OV7725_SLOP         , 0x20},
    {OV7725_LC_RADI      , 0x00},
    {OV7725_LC_COEF      , 0x13},
    {OV7725_LC_XC        , 0x08},
    {OV7725_LC_COEFB     , 0x14},
    {OV7725_LC_COEFR     , 0x17},
    {OV7725_LC_CTR       , 0x05},
    // {OV7725_BDBase       , 0x99},
    // {OV7725_BDMStep      , 0x03},
    {OV7725_SDE          , 0x04},
    {OV7725_BRIGHT       , 0x00},
    
    {OV7725_SIGN         , 0x06},
    {OV7725_UVADJ0       , 0x11},
    {OV7725_UVADJ1       , 0x02},

};

uint8_t OV7725_Eagle_cfgnum = sizeof(OV7725_Eagle_reg)/sizeof(OV7725_Eagle_reg[0]); /*结构体数组成员数目*/


