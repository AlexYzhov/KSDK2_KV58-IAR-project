/* This is a template for board specific configuration */
#include "include.h"

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    
}

void BOARD_InitUART0(void)
{
    uart_config_t UART0_config;
    
    UART0_config.baudRate_Bps = 115200U;                        // 波特率：115200
    UART0_config.parityMode = kUART_ParityDisabled;             // 校验位：None
    UART0_config.stopBitCount = kUART_OneStopBit;               // 停止位：1
    UART0_config.txFifoWatermark = 0;                           // tx缓冲：0
    UART0_config.rxFifoWatermark = 1;                           // rx缓冲：1
    UART0_config.enableRx = true;
    UART0_config.enableTx = false;
    
    UART_Init(UART0, &UART0_config, SystemFastPeripheralDriverClock);            // uart的输入时钟为Bus Clock
    
    UART_EnableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable);
    EnableIRQ(UART0_RX_TX_IRQn);
}

void BOARD_InitUART4(void)
{
    uart_config_t UART4_config;
    
    UART4_config.baudRate_Bps = 256000u;                          // 波特率：115200
    UART4_config.parityMode = kUART_ParityDisabled;             // 校验位：None
    UART4_config.stopBitCount = kUART_OneStopBit;               // 停止位：1
    UART4_config.txFifoWatermark = 1;                           // tx缓冲：0
    UART4_config.rxFifoWatermark = 1;                           // rx缓冲：0
    UART4_config.enableRx = true;
    UART4_config.enableTx = true;
    
    UART_Init(UART4, &UART4_config, SystemFastPeripheralDriverClock);            // uart的输入时钟为Bus Clock
}

void BOARD_InitADC1(void)                      // !NOT TESTED YET
{
    // hsadc_config_t HSADC1_config;
    // 
    // HSADC_GetDefaultConfig(&HSADC1_config);       // 获取ADC默认配置
    // 
    // HSADC1_config.resolution = kHSADC_Resolution12Bit;
    // 
    // 
    // HSADC_Init(HSADC1, &HSADC1_config);             // 配置ADC1
    // HSADC_EnableHardwareTrigger(HSADC1, true);        // 使能ADC硬件触发(Auto Calibration需要)
    // HSADC_DoAutoCalibration(HSADC1);                  // 启动自校准(使用ADC前必须完成校正步骤)
    // HSADC_EnableHardwareTrigger(HSADC1, false);       // 关闭ADC硬件触发
    // HSADC_SetHardwareAverage(HSADC1, kADC16_HardwareAverageDisabled);             // 设置ADC1采集0次硬件平均
    // 
    // HSADC_SetChannelMuxMode(ADC1, kADC16_ChannelMuxB);
    
    hsadc_config_t HSADC1_config;
    hsadc_converter_config_t  HSADC1_Converter_config;
    
    HSADC_GetDefaultConfig(&HSADC1_config);
    // HSADC1_config.dualConverterScanMode = kHSADC_DualConverterWorkAsTriggeredSequential;             // HSADC工作在单序列采集模式(非并发)
    HSADC1_config.enableSimultaneousMode = false;                                              // 关闭并行采集模式
    // HSADC1_config.idleWorkMode = kHSADC_IdleKeepNormal;                                         // 空闲时不挂起，保持正常工作状态
    // HSADC1_config.resolution = kHSADC_Resolution12Bit;                                          // 转换精度12bit
    
    HSADC1_Converter_config.clockDivisor = 5U;                                                  // 输入时钟(System Clock)分频系数=5，HSADC输入时钟应小于80Mhz
    HSADC1_Converter_config.samplingTimeCount = 0U;
    HSADC1_Converter_config.powerUpCalibrationModeMask = kHSADC_CalibrationModeSingleEnded;     // 自校准时采用单端转换模式
    
    HSADC_Init(HSADC1, &HSADC1_config);
    HSADC_SetConverterConfig(HSADC1, kHSADC_ConverterA|kHSADC_ConverterB, &HSADC1_Converter_config);
    
    HSADC_EnableConverterPower (HSADC1, kHSADC_ConverterA|kHSADC_ConverterB, true);              // 打开HSADC模块电源
    HSADC_EnableConverter (HSADC1, kHSADC_ConverterA|kHSADC_ConverterB, true);                   // 使能HSADC(打开时钟门控)
    // HSADC_EnableConverterSyncInput(HSADC1, kHSADC_ConverterB, false);          // 禁用外同步触发HSADC转化
    
    hsadc_sample_config_t HSADC1_LeftMotor_config;                              // PTC8 ~ HSADC1B_11
    hsadc_sample_config_t HSADC1_RightMotor_config;                             // PTC9 ~ HSADC1B_12
    
    HSADC1_LeftMotor_config.enableDifferentialPair = false;                    // 禁用差分对输入，使用单端采集模式
    HSADC1_LeftMotor_config.channelNumber = 7U + 8U;                            // 由HSADC内部结构：
    HSADC1_LeftMotor_config.channel67MuxNumber = 2U;                            // HSADC1_B_Ch11，对应通道由ch7-mux-2连接到PTC8.
    HSADC1_LeftMotor_config.zeroCrossingMode = kHSADC_ZeroCorssingDisabled;
    HSADC1_LeftMotor_config.enableWaitSync = false;
    
    HSADC1_RightMotor_config.enableDifferentialPair = false;                    // 禁用差分对输入，使用单端采集模式
    HSADC1_RightMotor_config.channelNumber = 6U + 8U;                            // 由HSADC内部结构：
    HSADC1_RightMotor_config.channel67MuxNumber = 3U;                            // HSADC1_B_Ch12，对应通道由ch6-mux-3连接到PTC9.
    HSADC1_RightMotor_config.zeroCrossingMode = kHSADC_ZeroCorssingDisabled;
    HSADC1_RightMotor_config.enableWaitSync = false;
    
    HSADC_SetSampleConfig(HSADC1, 8U, &HSADC1_LeftMotor_config);                // 放置HSADC1_B_Ch11信号在slot 8中.(slot 0~7仅限HSADCx_A通道的0~7连接， slot 8~15仅限HSADCx_B的通道0~7连接)
    HSADC_SetSampleConfig(HSADC1, 9U, &HSADC1_RightMotor_config);               // 放置HSADC1_B_Ch12信号在slot 9中.(slot 0~7仅限HSADCx_A通道的0~7连接， slot 8~15仅限HSADCx_B的通道0~7连接)
    
    HSADC_EnableSample(HSADC1, HSADC_SAMPLE_MASK(8U)|HSADC_SAMPLE_MASK(9U), true);                      // 注册slot8(PTC8~ch11)和slot9(PTC9~ch12)
    HSADC_EnableSample(HSADC1, (uint16_t)(~(HSADC_SAMPLE_MASK(8U)|HSADC_SAMPLE_MASK(9U))), false);      // 关闭其他的slots
    
    HSADC_DoAutoCalibration(HSADC1, kHSADC_ConverterB, kHSADC_CalibrationModeSingleEnded);
    // while ((kHSADC_ConverterBEndOfScanFlag|kHSADC_ConverterBEndOfCalibrationFlag) != ((kHSADC_ConverterBEndOfScanFlag|kHSADC_ConverterBEndOfCalibrationFlag)&HSADC_GetStatusFlags(HSADC1)));
    while (kHSADC_ConverterBEndOfCalibrationFlag!=kHSADC_ConverterBEndOfCalibrationFlag & HSADC_GetStatusFlags(HSADC1));
    
    // HSADC_ClearStatusFlags(HSADC1, kHSADC_ConverterAEndOfScanFlag|kHSADC_ConverterAEndOfCalibrationFlag|kHSADC_ConverterBEndOfScanFlag|kHSADC_ConverterBEndOfCalibrationFlag);
    HSADC_ClearStatusFlags(HSADC1, kHSADC_ConverterBEndOfCalibrationFlag);
    
    //-------------------------------------------------------------------------//
}

void BOARD_InitI2C1(void)
{
    i2c_master_config_t I2C1_config;
    
    I2C_MasterGetDefaultConfig(&I2C1_config);           // 获取I2C默认设置
    I2C1_config.baudRate_Bps = 400*1000U;               // 设置I2C1通信速率200K
    I2C1_config.enableMaster = true;                   // 设置I2C1为主机
    
    I2C_MasterInit(I2C1, &I2C1_config, SystemBusClock); // I2C的输入时钟为Bus Clock
}

void BOARD_InitI2C3(void)
{
  /*
    i2c_master_config_t I2C3_config;
    I2C_MasterGetDefaultConfig(&I2C3_config);           // 获取I2C默认设置
    I2C3_config.baudRate_Bps = 200*1000U;               // 设置I2C3通信速率400K
    I2C3_config.enableMaster = true;                   // 设置I2C3为主机
    
    I2C_MasterInit(I2C3, &I2C3_config, SystemBusClock); // I2C的输入时钟为Bus Clock
  */
    
    gpio_pin_config_t GPIO_SCCB_config;
    
    GPIO_SCCB_config.pinDirection = kGPIO_DigitalOutput;  // GPIO方向
    GPIO_SCCB_config.outputLogic  = 1;
    
    GPIO_PinInit(GPIOE, 10u, &GPIO_SCCB_config);          // PORTE10 ~ SDA
    GPIO_PinInit(GPIOE, 11u, &GPIO_SCCB_config);          // PORTE11 ~ SCL 
}

void BOARD_InitSPI0(void)
{
    dspi_master_config_t  SPI0_config;
    
    SPI0_config.whichCtar = kDSPI_Ctar0;
    SPI0_config.ctarConfig.baudRate = 10*1000*1000U;                 // 设置SPI通信速率10M
    SPI0_config.ctarConfig.bitsPerFrame = 16;                        // 数据比特长度：16bit
    SPI0_config.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;     // CPOL=0
    SPI0_config.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;         // 第一个沿采集(CPOL=0时为上升沿）
    SPI0_config.ctarConfig.direction = kDSPI_MsbFirst;
    SPI0_config.ctarConfig.pcsToSckDelayInNanoSec = 100U;
    SPI0_config.ctarConfig.lastSckToPcsDelayInNanoSec = 100U;
    SPI0_config.ctarConfig.betweenTransferDelayInNanoSec = 50*1000U; // 50us
    
    SPI0_config.whichPcs = kDSPI_Pcs0;                           // 片选通道CS0
    SPI0_config.pcsActiveHighOrLow = kDSPI_PcsActiveLow;         // 低电平片选
    SPI0_config.enableContinuousSCK = false;                    // 时钟不连续
    SPI0_config.enableRxFifoOverWrite = true;                   // rx fifo覆写使能
    SPI0_config.enableModifiedTimingFormat = false;             // 禁止自定义时序格式
    SPI0_config.samplePoint = kDSPI_SckToSin0Clock;
    
    DSPI_MasterInit(SPI0, &SPI0_config, SystemFastPeripheralDriverClock);
    DSPI_SetFifoEnable(SPI0, false, false);
}

void BOARD_InitSPI1(void)
{
    dspi_master_config_t  SPI1_config;
    
    SPI1_config.whichCtar = kDSPI_Ctar0;
    SPI1_config.ctarConfig.baudRate = 100*1000U;                  // 设置SPI通信速率1M
    SPI1_config.ctarConfig.bitsPerFrame = 16;                        // 数据比特长度：16bit
    SPI1_config.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;    // CPOL=0
    SPI1_config.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;        // 第1个沿采集(CPOL=0时为上升沿）
    SPI1_config.ctarConfig.direction = kDSPI_MsbFirst;
    SPI1_config.ctarConfig.pcsToSckDelayInNanoSec = 30U;
    SPI1_config.ctarConfig.lastSckToPcsDelayInNanoSec = 30U;
    SPI1_config.ctarConfig.betweenTransferDelayInNanoSec = 50*1000U; // 50us
    
    SPI1_config.whichPcs = kDSPI_Pcs0;                           // 片选通道CS0
    SPI1_config.pcsActiveHighOrLow = kDSPI_PcsActiveLow;         // 低电平片选
    SPI1_config.enableContinuousSCK = false;                    // 时钟不连续
    SPI1_config.enableRxFifoOverWrite = false;                  // rx fifo覆写使能
    SPI1_config.enableModifiedTimingFormat = false;             // 禁止自定义时序格式
    SPI1_config.samplePoint = kDSPI_SckToSin0Clock;
    
    DSPI_MasterInit(SPI1, &SPI1_config, SystemFastPeripheralDriverClock);
    DSPI_SetFifoEnable(SPI1, false, false);
    
    /*
     * 以下代码初始化SPI1>CTAR1设置.
     */
    dspi_master_ctar_config_t SPI1_Ctar1;
    
    SPI1_Ctar1.baudRate = 1*1000*1000U;                // 设置SPI通信速率1M
    SPI1_Ctar1.bitsPerFrame = 8;                       // 数据比特长度：8bit
    SPI1_Ctar1.cpol = kDSPI_ClockPolarityActiveHigh;   // CPOL=0
    SPI1_Ctar1.cpha = kDSPI_ClockPhaseFirstEdge;      // 第1个沿采集
    SPI1_Ctar1.direction = kDSPI_MsbFirst;
    SPI1_Ctar1.pcsToSckDelayInNanoSec = 30U;
    SPI1_Ctar1.lastSckToPcsDelayInNanoSec = 30U;
    SPI1_Ctar1.betweenTransferDelayInNanoSec = 50*1000U; // 50us
    
    if (0 == DSPI_MasterSetBaudRate(SPI1, kDSPI_Ctar1, SPI1_Ctar1.baudRate, SystemFastPeripheralDriverClock))
    {
        assert(false);
    }
    
    uint32_t temp = SPI1->CTAR[kDSPI_Ctar1] &
        ~(SPI_CTAR_FMSZ_MASK | SPI_CTAR_CPOL_MASK | SPI_CTAR_CPHA_MASK | SPI_CTAR_LSBFE_MASK);
    
    SPI1->CTAR[kDSPI_Ctar1] = temp | 
        SPI_CTAR_FMSZ(SPI1_Ctar1.bitsPerFrame - 1) | SPI_CTAR_CPOL(SPI1_Ctar1.cpol) |
            SPI_CTAR_CPHA(SPI1_Ctar1.cpha) | SPI_CTAR_LSBFE(SPI1_Ctar1.direction);
    
    DSPI_MasterSetDelayTimes(SPI1, kDSPI_Ctar1, kDSPI_PcsToSck, SystemFastPeripheralDriverClock,
                             SPI1_Ctar1.pcsToSckDelayInNanoSec);
    DSPI_MasterSetDelayTimes(SPI1, kDSPI_Ctar1, kDSPI_LastSckToPcs, SystemFastPeripheralDriverClock,
                             SPI1_Ctar1.lastSckToPcsDelayInNanoSec);
    DSPI_MasterSetDelayTimes(SPI1, kDSPI_Ctar1, kDSPI_BetweenTransfer, SystemFastPeripheralDriverClock,
                             SPI1_Ctar1.betweenTransferDelayInNanoSec);
    
    // GPIO_PinInit(GPIOB, 10U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 1});       // SPI1_PCS0
    // GPIO_PinInit(GPIOB,  9U, &(gpio_pin_config_t){kGPIO_DigitalInput,  0});       // CC1101_GDO0
    // GPIO_PinInit(GPIOB,  8U, &(gpio_pin_config_t){kGPIO_DigitalInput,  0});       // CC1101_GDO2
    
    GPIO_PinInit(GPIOB, 10U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 1});       // SPI1_PCS0
    GPIO_PinInit(GPIOB,  9U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 0});       // NRF24L01_CE
    GPIO_PinInit(GPIOB,  8U, &(gpio_pin_config_t){kGPIO_DigitalInput,  0});       // NRF24L01_IRQ
    
    PORT_SetPinInterruptConfig(PORTB, 8U, kPORT_InterruptFallingEdge);            // PTB8 ~ NRF24L01_IRQ. 信号下降沿触发NRF中断
    EnableIRQ(PORTB_IRQn);
}

void BOARD_InitSPI2(void)
{
    dspi_master_config_t SPI2_config;
    
    SPI2_config.whichCtar = kDSPI_Ctar0;
    SPI2_config.ctarConfig.baudRate = 1*1000*1000U;                  // 设置SPI通信速率1M
    SPI2_config.ctarConfig.bitsPerFrame = 16;                        // 数据比特长度：16bit
    SPI2_config.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;     // CPOL=0
    SPI2_config.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;         // 第一个沿采集(CPOL=0时为上升沿）
    SPI2_config.ctarConfig.direction = kDSPI_MsbFirst;
    SPI2_config.ctarConfig.pcsToSckDelayInNanoSec = 100U;
    SPI2_config.ctarConfig.lastSckToPcsDelayInNanoSec = 100U;
    SPI2_config.ctarConfig.betweenTransferDelayInNanoSec = 50*1000U; // 50us
    
    SPI2_config.whichPcs = kDSPI_Pcs0;                           // 片选通道CS0
    SPI2_config.pcsActiveHighOrLow = kDSPI_PcsActiveLow;         // 低电平片选
    SPI2_config.enableContinuousSCK = false;                    // 时钟不连续
    SPI2_config.enableRxFifoOverWrite = true;                   // rx fifo覆写使能
    SPI2_config.enableModifiedTimingFormat = false;             // 禁止自定义时序格式
    SPI2_config.samplePoint = kDSPI_SckToSin0Clock;
    
    DSPI_MasterInit(SPI2, &SPI2_config, SystemFastPeripheralDriverClock);
    DSPI_SetFifoEnable(SPI2, false, false);
    
    /*
     * 以下代码初始化SPI2->CTAR1设置.
     */
    dspi_master_ctar_config_t SPI2_Ctar1;
    
    SPI2_Ctar1.baudRate = 1*1000*1000U;                  // 设置SPI通信速率1M
    SPI2_Ctar1.bitsPerFrame = 8;                         // 数据比特长度：8bit
    SPI2_Ctar1.cpol = kDSPI_ClockPolarityActiveHigh;     // CPOL=0
    SPI2_Ctar1.cpha = kDSPI_ClockPhaseFirstEdge;         // 第一个沿采集(CPOL=0时为上升沿）
    SPI2_Ctar1.direction = kDSPI_MsbFirst;
    SPI2_Ctar1.pcsToSckDelayInNanoSec = 100U;
    SPI2_Ctar1.lastSckToPcsDelayInNanoSec = 100U;
    SPI2_Ctar1.betweenTransferDelayInNanoSec = 50*1000U; // 50us
    
    if (0 == DSPI_MasterSetBaudRate(SPI2, kDSPI_Ctar1, SPI2_Ctar1.baudRate, SystemFastPeripheralDriverClock))
    {
        assert(false);
    }
    
    uint32_t temp = SPI2->CTAR[kDSPI_Ctar1] &
                    ~(SPI_CTAR_FMSZ_MASK | SPI_CTAR_CPOL_MASK | SPI_CTAR_CPHA_MASK | SPI_CTAR_LSBFE_MASK);

    SPI2->CTAR[kDSPI_Ctar1] = temp | 
                              SPI_CTAR_FMSZ(SPI2_Ctar1.bitsPerFrame - 1) | SPI_CTAR_CPOL(SPI2_Ctar1.cpol) |
                              SPI_CTAR_CPHA(SPI2_Ctar1.cpha) | SPI_CTAR_LSBFE(SPI2_Ctar1.direction);

    DSPI_MasterSetDelayTimes(SPI2, kDSPI_Ctar1, kDSPI_PcsToSck, SystemFastPeripheralDriverClock,
                             SPI2_Ctar1.pcsToSckDelayInNanoSec);
    DSPI_MasterSetDelayTimes(SPI2, kDSPI_Ctar1, kDSPI_LastSckToPcs, SystemFastPeripheralDriverClock,
                             SPI2_Ctar1.lastSckToPcsDelayInNanoSec);
    DSPI_MasterSetDelayTimes(SPI2, kDSPI_Ctar1, kDSPI_BetweenTransfer, SystemFastPeripheralDriverClock,
                             SPI2_Ctar1.betweenTransferDelayInNanoSec);
}

void BOARD_InitSDHC(void)
{
    // sdhc_config_t SDHC_config;
    // 
    // SDHC_config.cardDetectDat3      = false;
    // SDHC_config.dmaMode             = kSDHC_DmaModeNo;      // DMA关闭
    // SDHC_config.endianMode          = kSDHC_EndianModeLittle;    // 小端模式
    // SDHC_config.readWatermarkLevel  = 0x80U;
    // SDHC_config.writeWatermarkLevel = 0x80U;
    // 
    // SDHC_Init(SDHC, &SDHC_config);
}

void BOARD_InitFTM0(void)
{
    ftm_config_t FTM0Info;
    ftm_chnl_pwm_signal_param_t ftmParam[4];
    
    /* Configures the FTM parameters with frequency 20 kHZ */
    ftmParam[0].chnlNumber = kFTM_Chnl_0;       // PWM通道
    ftmParam[0].level = kFTM_HighTrue;          // PWM有效信号(高电平有效)
    ftmParam[0].dutyCyclePrecision = 10*1000U;  // PWM精度(10*1000)
    ftmParam[0].dutyCyclePercent = 0U;          // PWM初始化duty(0)
    ftmParam[0].firstEdgeDelayPercent = 0U;     // PWM第一时钟沿延时(0)
    
    ftmParam[1].chnlNumber = kFTM_Chnl_1;       // PWM通道
    ftmParam[1].level = kFTM_HighTrue;          // PWM有效信号(高电平有效)
    ftmParam[1].dutyCyclePrecision = 10*1000U;  // PWM精度(10*1000)
    ftmParam[1].dutyCyclePercent = 0U;          // PWM初始化duty(0)
    ftmParam[1].firstEdgeDelayPercent = 0U;     // PWM第一时钟沿延时(0)
    
    ftmParam[2].chnlNumber = kFTM_Chnl_2;       // PWM通道
    ftmParam[2].level = kFTM_HighTrue;          // PWM有效信号(高电平有效)
    ftmParam[2].dutyCyclePrecision = 10*1000U;  // PWM精度(10*1000)
    ftmParam[2].dutyCyclePercent = 0U;          // PWM初始化duty(20)
    ftmParam[2].firstEdgeDelayPercent = 0U;     // PWM第一时钟沿延时(0)
    
    ftmParam[3].chnlNumber = kFTM_Chnl_3;       // PWM通道
    ftmParam[3].level = kFTM_HighTrue;          // PWM有效信号(高电平有效)
    ftmParam[3].dutyCyclePrecision = 10*1000U;  // PWM精度(10*1000)
    ftmParam[3].dutyCyclePercent = 0U;          // PWM初始化duty(0)
    ftmParam[3].firstEdgeDelayPercent = 0U;     // PWM第一时钟沿延时(0)
    
    
    FTM_GetDefaultConfig(&FTM0Info);
    //FTM0Info.prescale = kFTM_Prescale_Divide_4;
    //FTM0Info.pwmSyncMode  = kFTM_HardwareTrigger_0;
    FTM0Info.reloadPoints = kFTM_CntMax;
    FTM0Info.chnlPolarity = 0;
    
    /* Initializes the FTM module. */
    FTM_Init(FTM0, &FTM0Info);
    FTM_SetupPwm(FTM0, ftmParam, sizeof(ftmParam)/sizeof(ftm_chnl_pwm_signal_param_t), kFTM_EdgeAlignedPwm, 20*1000U, SystemFastPeripheralDriverClock);    // 频率20K
    FTM_StartTimer(FTM0, kFTM_SystemClock);
    
    //FTM_UpdatePwmDutycycle(FTM0, kFTM_Chnl_0, kFTM_EdgeAlignedPwm, 10*1000U, 50);
    //FTM_UpdatePwmDutycycle(FTM0, kFTM_Chnl_1, kFTM_EdgeAlignedPwm, 10*1000U, 50);
    //FTM_UpdatePwmDutycycle(FTM0, kFTM_Chnl_2, kFTM_EdgeAlignedPwm, 10*1000U, 50);
    //FTM_UpdatePwmDutycycle(FTM0, kFTM_Chnl_3, kFTM_EdgeAlignedPwm, 10*1000U, 50);
}

void BOARD_InitFTM1(void)
{
    // ftm_config_t FTM1Info;
    ftm_phase_params_t PhAParams, PhBParams;
    
    // FTM_GetDefaultConfig(&FTM1Info);
    
    PhAParams.enablePhaseFilter = false;
    PhAParams.phaseFilterVal = 0x00U;                      // Filter Val应小于0x0F
    PhAParams.phasePolarity = kFTM_QuadPhaseNormal;
    
    PhBParams.enablePhaseFilter = false;
    PhBParams.phaseFilterVal = 0x00U;                      // Filter Val应小于0x0F
    PhBParams.phasePolarity = kFTM_QuadPhaseNormal;
    
    // FTM_Init(FTM2, &FTM2Info);
    CLOCK_EnableClock(kCLOCK_Ftm1);
    FTM_SetupQuadDecode(FTM1, &PhAParams, &PhBParams, kFTM_QuadCountAndDir);
    FTM_ClearQuadDecoderCounterValue(FTM1);
}

void BOARD_InitFTM2(void)
{
    // ftm_config_t FTM2Info;
    ftm_phase_params_t PhAParams, PhBParams;
    
    // FTM_GetDefaultConfig(&FTM2Info);
    
    PhAParams.enablePhaseFilter = false;
    PhAParams.phaseFilterVal = 0x00U;                      // Filter Val应小于0x0F
    PhAParams.phasePolarity = kFTM_QuadPhaseNormal;
    
    PhBParams.enablePhaseFilter = false;
    PhBParams.phaseFilterVal = 0x00U;                      // Filter Val应小于0x0FcxCXzf
    PhBParams.phasePolarity = kFTM_QuadPhaseNormal;
    
    // FTM_Init(FTM2, &FTM2Info);
    CLOCK_EnableClock(kCLOCK_Ftm2);
    FTM_SetupQuadDecode(FTM2, &PhAParams, &PhBParams, kFTM_QuadCountAndDir);
    FTM_ClearQuadDecoderCounterValue(FTM2);
}

void BOARD_InitFTM3(void)
{
    ftm_config_t FTM3Info;
    ftm_chnl_pwm_signal_param_t ftmParam[2];
    
    /* Configures the FTM parameters with frequency 100 HZ */
    ftmParam[0].chnlNumber = kFTM_Chnl_2;
    ftmParam[0].level = kFTM_HighTrue;
    ftmParam[0].dutyCyclePrecision = 10*1000U;
    ftmParam[0].dutyCyclePercent = 750U;//750U;                // 712
    ftmParam[0].firstEdgeDelayPercent = 0U;

    ftmParam[1].chnlNumber = kFTM_Chnl_3;
    ftmParam[1].level = kFTM_HighTrue;
    ftmParam[1].dutyCyclePrecision = 10*1000U;
    ftmParam[1].dutyCyclePercent = 0U;
    ftmParam[1].firstEdgeDelayPercent = 0U;
    
    
    FTM_GetDefaultConfig(&FTM3Info);
    // FTM3Info.chnlInitState = kFTM_Prescale_Divide_2;
    //FTM3Info.pwmSyncMode  = kFTM_SoftwareTrigger;
    FTM3Info.reloadPoints = kFTM_CntMax;
    FTM3Info.chnlPolarity = 0;
    // FTM3Info.pwmSyncMode = kFTM_HardwareTrigger_2;
    
    /* Initializes the FTM module. */
    FTM_Init(FTM3, &FTM3Info);
    FTM_SetupPwm(FTM3, ftmParam, sizeof(ftmParam)/sizeof(ftm_chnl_pwm_signal_param_t), kFTM_EdgeAlignedPwm, 50, SystemMCGFFClock);    // 频率50
    FTM_StartTimer(FTM3, kFTM_FixedClock);
    
    // FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_2, kFTM_EdgeAlignedPwm, 10*1000U, 8000U);
    // FTM_UpdatePwmDutycycle(FTM3, kFTM_Chnl_3, kFTM_EdgeAlignedPwm, 10*1000U, 0U);
    
    
}

void BOARD_InitPIT0(void)
{
    pit_config_t PIT_config;
    // PIT_GetDefaultConfig(&PIT_config);
    PIT_config.enableRunInDebug = true;
    
    /* Init pit module */
    PIT_Init(PIT, &PIT_config);
    /* Set Timer Chain Mode */
    PIT_SetTimerChainMode(PIT, kPIT_Chnl_0, false);
    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, MSEC_TO_COUNT(20U, SystemBusClock));         // 定时周期 T0 = 20ms
    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(PIT0_IRQn);
    /* Start channel 0 */
     //PIT_StartTimer(PIT, kPIT_Chnl_0);
}

void BOARD_InitPIT1(void)
{
    pit_config_t PIT_config;
    // PIT_GetDefaultConfig(&PIT_config);
    PIT_config.enableRunInDebug = true;
    
    /* Init pit module */
    PIT_Init(PIT, &PIT_config);
    /* Set Timer Chain Mode */
    PIT_SetTimerChainMode(PIT, kPIT_Chnl_1, false);
    /* Set timer period for channel 1 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, MSEC_TO_COUNT(15U, SystemBusClock));         // 定时周期 T1 = 15ms
    /* Enable timer interrupts for channel 1 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(PIT1_IRQn);
    /* Start channel 1 */
    //PIT_StartTimer(PIT, kPIT_Chnl_1);
}

void BOARD_InitPIT2(void)
{
    pit_config_t PIT_config;
    // PIT_GetDefaultConfig(&PIT_config);
    PIT_config.enableRunInDebug = true;
    
    /* Init pit module */
    PIT_Init(PIT, &PIT_config);
    /* Set Timer Chain Mode */
    PIT_SetTimerChainMode(PIT, kPIT_Chnl_2, false);
    /* Set timer period for channel 2 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, MSEC_TO_COUNT(1U, SystemBusClock));         // 定时周期 T2 = 1000ms = 1s
    /* Enable timer interrupts for channel 2 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(PIT2_IRQn);
    /* Start channel 2 */
    //PIT_StartTimer(PIT, kPIT_Chnl_2);
}

void BOARD_InitPIT3(void)
{
    pit_config_t PIT_config;
    // PIT_GetDefaultConfig(&PIT_config);
    PIT_config.enableRunInDebug = true;
    
    /* Init pit module */
    PIT_Init(PIT, &PIT_config);
    /* Set Timer Chain Mode */
    PIT_SetTimerChainMode(PIT, kPIT_Chnl_3, false);
    /* Set timer period for channel 3 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_3, MSEC_TO_COUNT(1000U, SystemBusClock));         // 定时周期 T3 = 1000ms = 1s
    /* Enable timer interrupts for channel 3 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(PIT3_IRQn);
    /* Start channel 3 */
    //PIT_StartTimer(PIT, kPIT_Chnl_3);
}

void BOARD_InitLPTMR(void)
{
    lptmr_config_t LPTMR0_config;
    
    LPTMR_GetDefaultConfig(&LPTMR0_config);
    //LPTMR0_config.timerMode = kLPTMR_TimerModeTimeCounter;              // lptmr为计时模式
    //LPTMR0_config.prescalerClockSource = kLPTMR_PrescalerClock_1;       // 选择lptmr时钟源为LPO
    //LPTMR0_config.enableFreeRunning = true;                            // 设置lptmr满计数自动复位
    
    LPTMR_Init(LPTMR0, &LPTMR0_config);
    
    LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(1000000U, 1000));        // LPO频率为1khz
}

void BOARD_InitGPIO(void)
{
    gpio_pin_config_t gpio_config;
    
    gpio_config.pinDirection = kGPIO_DigitalOutput;  // GPIO方向
    gpio_config.outputLogic = 1;                    // 默认输出电平，1=HIGH，0=LOW，设置为Input时无效
    /*  Sets the configuration */
    GPIO_PinInit(GPIOA, 17U, &gpio_config);      // LED
    GPIO_PinInit(GPIOC,  0U, &gpio_config);      // LED
    GPIO_PinInit(GPIOD, 15U, &gpio_config);      // LED
    GPIO_PinInit(GPIOE, 29U, &gpio_config);      // LED
    
    GPIO_PinInit(GPIOC, 12U, &gpio_config);     // LCD_RD
    GPIO_PinInit(GPIOD,  4U, &gpio_config);     // LCD_CS
    
    gpio_config.pinDirection = kGPIO_DigitalOutput;
    gpio_config.outputLogic = 0;
    
    GPIO_PinInit(GPIOC, 13U, &gpio_config);      // 蜂鸣器
    
    GPIO_PinInit(GPIOC, 18U, &gpio_config);     // LCD_WR
    GPIO_PinInit(GPIOC, 19U, &gpio_config);     // LCD_RS
    GPIO_PinInit(GPIOD,  5U, &gpio_config);     // LCD_RST
    
    GPIO_PinInit(GPIOD,  9U, &gpio_config);     // LCD_D7
    GPIO_PinInit(GPIOD,  8U, &gpio_config);     // LCD_D6
    GPIO_PinInit(GPIOD, 11U, &gpio_config);     // LCD_D5
    GPIO_PinInit(GPIOD, 10U, &gpio_config);     // LCD_D4
    GPIO_PinInit(GPIOD, 13U, &gpio_config);     // LCD_D3
    GPIO_PinInit(GPIOD, 12U, &gpio_config);     // LCD_D2
    GPIO_PinInit(GPIOD, 15U, &gpio_config);     // LCD_D1
    GPIO_PinInit(GPIOD, 14U, &gpio_config);     // LCD_D0
    
    gpio_config.pinDirection = kGPIO_DigitalInput;
    gpio_config.outputLogic = 0;
    GPIO_PinInit(GPIOA, 14U, &gpio_config);     // PTA14 ~ DKEY1
    GPIO_PinInit(GPIOA, 15U, &gpio_config);     // PTA15 ~ DKEY2
    GPIO_PinInit(GPIOA, 16U, &gpio_config);     // PTA16 ~ DKEY3
    GPIO_PinInit(GPIOA, 17U, &gpio_config);     // PTA17 ~ DKEY4
    // GPIO_PinInit(GPIOA, 18U, &gpio_config);     // PTA18   DKEY5
    GPIO_PinInit(GPIOA, 19U, &gpio_config);     // PTA19 ~ DKEY6
    
    GPIO_PinInit(GPIOC, 15U, &gpio_config);     // PTC15 ~ KEY_UP
    GPIO_PinInit(GPIOD,  6U, &gpio_config);     // PTD6  ~ KEY_DOWN
    GPIO_PinInit(GPIOC, 14U, &gpio_config);     // PTC14 ~ KEY_LEFT
    GPIO_PinInit(GPIOC,  6U, &gpio_config);     // PTC6  ~ KEY_RIGHT
    GPIO_PinInit(GPIOD,  7U, &gpio_config);     // PTD7  ~ KEY_SELECT
}

void BOARD_InitGPIO_Camera(void)
{
    gpio_pin_config_t GPIO_Camera_config;
    
    GPIO_Camera_config.pinDirection = kGPIO_DigitalInput;  // GPIO方向
    GPIO_Camera_config.outputLogic  = 0;
    
    GPIO_PinInit(GPIOB,  0U, &GPIO_Camera_config);          // PTB0 ~ D7
    GPIO_PinInit(GPIOB,  1U, &GPIO_Camera_config);          // PTB1 ~ D6
    GPIO_PinInit(GPIOB,  2U, &GPIO_Camera_config);          // PTB2 ~ D5
    GPIO_PinInit(GPIOB,  3U, &GPIO_Camera_config);          // PTB3 ~ D4
    GPIO_PinInit(GPIOB,  4U, &GPIO_Camera_config);          // PTB4 ~ D3
    GPIO_PinInit(GPIOB,  5U, &GPIO_Camera_config);          // PTB5 ~ D2
    GPIO_PinInit(GPIOB,  6U, &GPIO_Camera_config);          // PTB6 ~ D1
    GPIO_PinInit(GPIOB,  7U, &GPIO_Camera_config);          // PTB7 ~ D0
    
    GPIO_PinInit(GPIOA,  7U, &GPIO_Camera_config);          // PTA7 ~ 拨码开关1号
    
    GPIO_PinInit(GPIOA, 24U, &GPIO_Camera_config);          // PTA24 ~ PCLK
    GPIO_PinInit(GPIOA, 25U, &GPIO_Camera_config);          // PTA25 ~ VSYNC
    GPIO_PinInit(GPIOA, 26U, &GPIO_Camera_config);          // PTA26 ~ HREF
}

void BOARD_InitDMA(void)
{
    edma_config_t DMA_config;
    DMA_config.enableContinuousLinkMode = false;
    DMA_config.enableDebugMode = false;
    DMA_config.enableHaltOnError = false;
    DMA_config.enableRoundRobinArbitration = false;
    
    EDMA_Init(DMA0, &DMA_config);
}

void BOARD_InitXBARA(void)
{
    XBARA_Init(XBARA);
    //XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn4, kXBARA_OutputXbOut10);
    //XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn5, kXBARA_OutputXbOut11);
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputAndOrInvert0, kXBARA_OutputDmamux18);         // AOI_Event0 ~ DMAMUX18
}

void BOARD_InitXBARB(void)
{
    XBARB_Init(XBARB);
    XBARB_SetSignalsConnection(XBARB, kXBARB_InputFtm0Match, kXBARB_OutputAoiIn0);      // XB_IN4 ~ XB_OUT0 ~ AOI_IN0
    XBARB_SetSignalsConnection(XBARB, kXBARB_InputFtm0Extrig, kXBARB_OutputAoiIn1);     // XB_IN5 ~ XB_OUT1 ~ AOI_IN1
}

void BORAD_InitAOI(void)
{
    /*
     kAOI_LogicZero = 0x0U,      // Forces the input to logical zero
     kAOI_InputSignal = 0x1U,    // Passes the input signal
     kAOI_InvInputSignal = 0x2U, // Inverts the input signal
     kAOI_LogicOne = 0x3U        // Forces the input to logical one
     */
    aoi_event_config_t PIXCLK_Event_config;
    PIXCLK_Event_config.PT0AC = kAOI_InputSignal;
    PIXCLK_Event_config.PT0BC = kAOI_LogicOne;
    PIXCLK_Event_config.PT0CC = kAOI_LogicOne;
    PIXCLK_Event_config.PT0DC = kAOI_LogicOne;
    
    PIXCLK_Event_config.PT1AC = kAOI_LogicOne;
    PIXCLK_Event_config.PT1BC = kAOI_InputSignal;
    PIXCLK_Event_config.PT1CC = kAOI_LogicOne;
    PIXCLK_Event_config.PT1DC = kAOI_LogicOne;
    
    PIXCLK_Event_config.PT2AC = kAOI_LogicOne;
    PIXCLK_Event_config.PT2BC = kAOI_LogicOne;
    PIXCLK_Event_config.PT2CC = kAOI_LogicOne;
    PIXCLK_Event_config.PT2DC = kAOI_LogicOne;
    
    PIXCLK_Event_config.PT3AC = kAOI_LogicOne;
    PIXCLK_Event_config.PT3BC = kAOI_LogicOne;
    PIXCLK_Event_config.PT3CC = kAOI_LogicOne;
    PIXCLK_Event_config.PT3DC = kAOI_LogicOne;
    
    AOI_Init(AOI0);
    AOI_SetEventLogicConfig(AOI0, kAOI_Event0, &PIXCLK_Event_config);
}

void BOARD_InitALL(void)
{
    BOARD_InitPins();
    
    //BOARD_InitUART0();
    BOARD_InitUART4();
    //BOARD_InitADC1();
    //BOARD_InitI2C1();
    BOARD_InitI2C3();
    //BOARD_InitSPI0();
    //BOARD_InitSPI1();
    //BOARD_InitSPI2();
    //BOARD_InitFTM0();
    //BOARD_InitFTM1();
    //BOARD_InitFTM2();
    //BOARD_InitFTM3();
    //BOARD_InitPIT0();
    //BOARD_InitPIT1();
    //BOARD_InitPIT2();
    //BOARD_InitPIT3();
    //BOARD_InitLPTMR();
    BOARD_InitGPIO();
    BOARD_InitGPIO_Camera();
    
    BOARD_InitXBARA();
    BOARD_InitXBARB();
    BORAD_InitAOI();
    
    BOARD_InitDMA();
    DMAMUX_Init(DMAMUX);
}