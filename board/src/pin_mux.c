/* This is a template file for pins configuration */

#include "include.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initialize all pins used in UART0
 */
void InitPins_UART0_LPC824(void)
{
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt5);   // PTC6 ~ UART0_RX
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAsGpio); // PTC7 ~ Gpio
    
    SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK))) /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(0x00U)       /* UART 0 transmit data source select: UART0_TX pin */
      | SIM_SOPT5_UART0RXSRC(0x00U)       /* UART 0 receive data source select: UART0_RX pin */
    );
    
    GPIO_PinInit(PORTC, 7U, &(gpio_pin_config_t){kGPIO_DigitalOutput, 1});
}

/*!
 * @brief Initialize all pins used in UART4
 */
void InitPins_UART4_UpperCOM(void)
{
    PORT_SetPinMux(PORTE, 24U, kPORT_MuxAlt8);   // PTE24 ~ UART4_TX
    PORT_SetPinMux(PORTE, 25U, kPORT_MuxAlt8);   // PTE25 ~ UART4_RX
        
    // SIM->SOPT5 = ((SIM->SOPT5 &
    // (~(SIM_SOPT5_UART1TXSRC_MASK | SIM_SOPT5_UART1RXSRC_MASK))) /* Mask bits to zero which are setting */
    //   | SIM_SOPT5_UART1TXSRC(0x00U)       /* UART 1 transmit data source select: UART1_TX pin */
    //   | SIM_SOPT5_UART1RXSRC(0x00U)       /* UART 1 receive data source select: UART1_RX pin */
    // );
}

/*!
 * @brief Initialize all pins used in ADC1
 */
void InitPins_ADC1_AD8210(void)
{
    PORT_SetPinMux(PORTC,  8U, kPORT_PinDisabledOrAnalog);      // HSADC1b_Ch11
    PORT_SetPinMux(PORTC,  9U, kPORT_PinDisabledOrAnalog);      // HSADC1b_Ch12
}

/*!
 * @brief Initialize all pins used in I2C1
 */
void InitPins_I2C1_VL53L0X(void)
{
    const port_pin_config_t i2c1 = 
    {
        kPORT_PullUp,                           // 内部输入上拉
        kPORT_FastSlewRate,                     // 高速翻转模式
        kPORT_PassiveFilterDisable,             // 无源滤波关闭
        kPORT_OpenDrainEnable,                  // 开漏输出
        kPORT_LowDriveStrength,                 // 低驱动能力
        kPORT_MuxAlt2,                          // 复用成I2C1
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    PORT_SetPinConfig(PORTC, 10u, &i2c1);       // PTC10 ~ I2C1_SCL
    PORT_SetPinConfig(PORTC, 11u, &i2c1);       // PTC11 ~ I2C1_SDA
  
    //PORT_SetPinMux(PORTC,  10u, kPORT_MuxAlt2);
    //PORT_SetPinMux(PORTC,  11u, kPORT_MuxAlt2);
}

/*!
 * @brief Initialize all pins used in I2C3
 */
void InitPins_I2C3_SCCB(void)
{
    const port_pin_config_t i2c3 = 
    {
        kPORT_PullUp,                           // 内部输入上拉
        kPORT_SlowSlewRate,                     // 高速翻转模式
        kPORT_PassiveFilterEnable,              // 无源滤波关闭
        kPORT_OpenDrainEnable,                  // 开漏输出
        kPORT_LowDriveStrength,                 // 低驱动能力
        kPORT_MuxAsGpio,                        // 复用成GPIO，模拟IIC
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    PORT_SetPinConfig(PORTE, 10u, &i2c3);       // PTE10 ~ I2C3_SDA
    PORT_SetPinConfig(PORTE, 11u, &i2c3);       // PTE11 ~ I2C3_SCL
}

/*!
 * @brief Initialize all pins used in SPI0
 */
void InitPins_SPI0_L3G4200D_SCREEN(void)
{
    const port_pin_config_t spi0 = 
    {
        kPORT_PullUp,                           // 内部输入上拉
        kPORT_FastSlewRate,                     // 高速翻转模式
        kPORT_PassiveFilterEnable,              // 无源滤波开启
        kPORT_OpenDrainDisable,                 // 推挽输出
        kPORT_HighDriveStrength,                // 高驱动能力
        kPORT_MuxAlt2,                          // 复用成SPI0
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    PORT_SetPinConfig(PORTD,  0u, &spi0);       // PTD0 ~ SPI0_PCS0
    PORT_SetPinConfig(PORTD,  1u, &spi0);       // PTD1 ~ SPI0_SCLK
    PORT_SetPinConfig(PORTD,  2u, &spi0);       // PTD2 ~ SPI0_MOSI
    PORT_SetPinConfig(PORTD,  3u, &spi0);       // PTD3 ~ SPI0_MISO
    PORT_SetPinConfig(PORTD,  4u, &spi0);       // PTD4 ~ SPI0_PCS1
    PORT_SetPinConfig(PORTD,  5u, &spi0);       // PTD5 ~ SPI0_PCS2
}

/*!
 * @brief Initialize all pins used in SPI1
 */
void InitPins_SPI1_NRF2401(void)
{
    const port_pin_config_t spi1 = 
    {
        kPORT_PullUp,                           // 内部输入上拉
        kPORT_FastSlewRate,                     // 高速翻转模式
        kPORT_PassiveFilterDisable,             // 无源滤波关闭
        kPORT_OpenDrainDisable,                 // 推挽输出
        kPORT_LowDriveStrength,                 // 低驱动能力
        kPORT_MuxAlt2,                          // 复用成SPI1
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    // PORT_SetPinConfig(PORTB, 10u, &spi1);       // PTB10 ~ SPI1_PCS0
    PORT_SetPinConfig(PORTB, 11u, &spi1);       // PTB11 ~ SPI1_SCLK
    PORT_SetPinConfig(PORTB, 16u, &spi1);       // PTB16 ~ SPI1_MOSI
    PORT_SetPinConfig(PORTB, 17u, &spi1);       // PTB17 ~ SPI1_MISO
    
    PORT_SetPinMux(PORTB, 10u, kPORT_MuxAsGpio);  // PTB10 ~ SPI1_PCS0
    PORT_SetPinMux(PORTB,  9u, kPORT_MuxAsGpio);  // PTB9  ~ NRF24L01_CE
    PORT_SetPinMux(PORTB,  8u, kPORT_MuxAsGpio);  // PTB8  ~ NRF24L01_IRQ
}

/*!
 * @brief Initialize all pins used in SPI2
 */
void InitPins_SPI2_ADNS9818(void)
{
    // const port_pin_config_t spi2 = 
    // {
    //     kPORT_PullUp,                           // 内部输入上拉
    //     kPORT_FastSlewRate,                     // 高速翻转模式
    //     kPORT_PassiveFilterDisable,             // 无源滤波关闭
    //     kPORT_OpenDrainDisable,                 // 推挽输出
    //     kPORT_LowDriveStrength,                 // 低驱动能力
    //     kPORT_MuxAlt2,                          // 复用成SPI2
    //     kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    // };
    // PORT_SetPinConfig(PORTB, 20u, &spi2);       // PTB10 ~ SPI2_PCS0
    // PORT_SetPinConfig(PORTB, 21u, &spi2);       // PTB11 ~ SPI2_SCLK
    // PORT_SetPinConfig(PORTB, 22u, &spi2);       // PTB16 ~ SPI2_MOSI
    // PORT_SetPinConfig(PORTB, 23u, &spi2);       // PTB17 ~ SPI2_MISO
}

/*!
 * @brief Initialize all pins used in SDHC
 */
void InitPins_SDHC(void)
{    
    PORT_SetPinMux(PORTE, 0u, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PORTE, 1u, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PORTE, 2u, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PORTE, 3u, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PORTE, 4u, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PORTE, 5u, kPORT_PinDisabledOrAnalog);
}

/*!
 * @brief Initialize all pins used in FTMx_QD(Encoder)
 */
void InitPins_FTMx_QD_Encoder(void)
{
    // const port_pin_config_t FTMx_QD = 
    // {
    //     kPORT_PullUp,                           // 内部输入上拉
    //     kPORT_FastSlewRate,                     // 高速翻转模式
    //     kPORT_PassiveFilterDisable,             // 无源滤波关闭
    //     kPORT_OpenDrainEnable,                  // 开漏输出
    //     kPORT_LowDriveStrength,                 // 低驱动能力
    //     kPORT_MuxAlt6,                          // 复用成FTMx QD(正交解码)
    //     kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    // };
    // PORT_SetPinConfig(PORTA,  8u, &FTMx_QD);    // PTA8  ~ FTM1_QD_PHA
    // PORT_SetPinConfig(PORTA,  9u, &FTMx_QD);    // PTA9  ~ FTM1_QD_PHB
    // PORT_SetPinConfig(PORTA, 10u, &FTMx_QD);    // PTA10 ~ FTM2_QD_PHA
    // PORT_SetPinConfig(PORTA, 11u, &FTMx_QD);    // PTA11 ~ FTM2_QD_PHB
    
    PORT_SetPinMux(PORTA, 8u, kPORT_PinDisabledOrAnalog);
    PORT_SetPinMux(PORTA, 9u, kPORT_PinDisabledOrAnalog);
}

/*!
 * @brief Initialize all pins used in FTM0_CHx(Motor)
 */
void InitPins_FTM0CHx_Motor(void)
{
    const port_pin_config_t FTM0CHx = 
    {
        kPORT_PullUp,                           // 内部输入上拉
        kPORT_FastSlewRate,                     // 高速翻转模式
        kPORT_PassiveFilterDisable,             // 无源滤波关闭
        kPORT_OpenDrainDisable,                 // 推挽输出
        kPORT_HighDriveStrength,                // 高驱动能力
        kPORT_MuxAlt4,                          // 复用成FTM0CHx(PWM输出)
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    PORT_SetPinConfig(PORTC,  1u, &FTM0CHx);    // PTC1 ~  FTM0_CH0
    PORT_SetPinConfig(PORTC,  2u, &FTM0CHx);    // PTC2 ~  FTM0_CH1
    PORT_SetPinConfig(PORTC,  3u, &FTM0CHx);    // PTC3 ~  FTM0_CH2
    PORT_SetPinConfig(PORTC,  4u, &FTM0CHx);    // PTC4 ~  FTM0_CH3
}

/*!
 * @brief Initialize all pins used in FTM1_QuadDecoder
 */
void InitPins_FTM1_QuadDecoder(void)
{
    const port_pin_config_t FTM1CHx = 
    {
        kPORT_PullUp,                           // 内部输入上拉
        kPORT_SlowSlewRate,                     // 高速翻转模式
        kPORT_PassiveFilterDisable,             // 无源滤波关闭
        kPORT_OpenDrainEnable,                  // 推挽输出
        kPORT_HighDriveStrength,                // 高驱动能力
        kPORT_MuxAlt7,                          // 复用成FTM1_QD(正交解码)
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    PORT_SetPinConfig(PORTA, 12u, &FTM1CHx);    // PTA12 ~ FTM1_QD_PHA
    PORT_SetPinConfig(PORTA, 13u, &FTM1CHx);    // PTA13 ~ FTM1_QD_PHB
}
                                                                   
/*!
 * @brief Initialize all pins used in FTM2_QuadDecoder
 */
void InitPins_FTM2_QuadDecoder(void)
{
    const port_pin_config_t FTM2CHx = 
    {
        kPORT_PullUp,                           // 内部输入上拉
        kPORT_SlowSlewRate,                     // 低速翻转模式
        kPORT_PassiveFilterEnable,              // 无源滤波关开启
        kPORT_OpenDrainEnable,                  // 开漏输出
        kPORT_HighDriveStrength,                // 高驱动能力
        kPORT_MuxAlt6,                          // 复用成FTM2_QD(正交解码)
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    PORT_SetPinConfig(PORTA, 10u, &FTM2CHx);    // PTA10 ~ FTM2_QD_PHA
    PORT_SetPinConfig(PORTA, 11u, &FTM2CHx);    // PTA11 ~ FTM2_QD_PHB
}

/*!
 * @brief Initialize all pins used in FTM3_CHx(Servo)
 */
void InitPins_FTM3CHx_Servo(void)
{
    const port_pin_config_t FTM3CHx = 
    {
        kPORT_PullUp,                           // 内部输入上拉
        kPORT_FastSlewRate,                     // 高速翻转模式
        kPORT_PassiveFilterDisable,             // 无源滤波关闭
        kPORT_OpenDrainDisable,                 // 推挽输出
        kPORT_HighDriveStrength,                // 高驱动能力
        kPORT_MuxAlt6,                          // 复用成FTM3CHx(PWM输出)
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    PORT_SetPinConfig(PORTE,  7u, &FTM3CHx);    // PTE7 ~ FTM3_CH2 ~ Control
    PORT_SetPinConfig(PORTE,  8u, &FTM3CHx);    // PTE8 ~ FTM3_CH3
}

/*!
 * @brief Initialize ALL pins used by Camera
 */
void InitPins_GPIO_Camera(void)
{
    /* Input pin PORT configuration */
    port_pin_config_t gpio_input = 
    {
        kPORT_PullDisable,                           // 内部输入上拉
        kPORT_SlowSlewRate,                     // 低速翻转模式
        kPORT_PassiveFilterEnable,              // 无源滤波开启(信号频率f>2Mhz时就应关闭)
        kPORT_OpenDrainDisable,                 // 推挽输出
        kPORT_LowDriveStrength,                 // 低驱动能力
        kPORT_MuxAsGpio,                        // 复用成GPIO(输入)
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    PORT_SetPinConfig(PORTB,  0, &gpio_input);        // PTB0 ~ D7
    PORT_SetPinConfig(PORTB,  1, &gpio_input);        // PTB1 ~ D6
    PORT_SetPinConfig(PORTB,  2, &gpio_input);        // PTB2 ~ D5
    PORT_SetPinConfig(PORTB,  3, &gpio_input);        // PTB3 ~ D4
    PORT_SetPinConfig(PORTB,  4, &gpio_input);        // PTB4 ~ D3
    PORT_SetPinConfig(PORTB,  5, &gpio_input);        // PTB5 ~ D2
    PORT_SetPinConfig(PORTB,  6, &gpio_input);        // PTB6 ~ D4
    PORT_SetPinConfig(PORTB,  7, &gpio_input);        // PTB7 ~ D0
    
    gpio_input.pullSelect = kPORT_PullDown;
    
    PORT_SetPinConfig(PORTA, 24, &gpio_input);        // PTA24 ~ PCLK
    PORT_SetPinConfig(PORTA, 25, &gpio_input);        // PTA25 ~ VSYNC
    PORT_SetPinConfig(PORTA, 26, &gpio_input);        // PTA26 ~ HREF
}

/*!
 * @brief Initialize ALL pins used by KEYs & DKEYs
 */
void InitPins_GPIO_KEY(void)
{
    /* Output pin PORT configuration */
    port_pin_config_t gpio_KEY = 
    {
        kPORT_PullUp,                           // 内部输入上拉
        kPORT_SlowSlewRate,                     // 低速翻转模式
        kPORT_PassiveFilterDisable,             // 无源滤波关闭
        kPORT_OpenDrainDisable,                 // 推挽输出
        kPORT_HighDriveStrength,                // 强驱动能力
        kPORT_MuxAsGpio,                        // 复用成GPIO
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    PORT_SetPinConfig(PORTA, 14, &gpio_KEY);        // PTA14 ~ DKEY1
    PORT_SetPinConfig(PORTA, 15, &gpio_KEY);        // PTA15 ~ DKEY2
    PORT_SetPinConfig(PORTA, 16, &gpio_KEY);        // PTA16 ~ DKEY3
    PORT_SetPinConfig(PORTA, 17, &gpio_KEY);        // PTA17 ~ DKEY4
    // PORT_SetPinConfig(PORTA, 18, &gpio_KEY);        // PTA18   DKEY5
    PORT_SetPinConfig(PORTA, 19, &gpio_KEY);        // PTA19 ~ DKEY6
    
    PORT_SetPinConfig(PORTC, 15, &gpio_KEY);        // PTC15 ~ KEY_UP
    PORT_SetPinConfig(PORTD,  6, &gpio_KEY);        // PTD6  ~ KEY_DOWN
    PORT_SetPinConfig(PORTC, 14, &gpio_KEY);        // PTC14 ~ KEY_LEFT
    PORT_SetPinConfig(PORTC,  5, &gpio_KEY);        // PTC5  ~ KEY_RIGHT
    PORT_SetPinConfig(PORTD,  7, &gpio_KEY);        // PTD7  ~ KEY_SELECT
}


/*!
 * @brief Initialize ALL pins used by LEDs
 */
void InitPins_GPIO_LED(void)
{
    /* Output pin PORT configuration */
    port_pin_config_t gpio_leds = 
    {
        kPORT_PullDisable,                      // 内部输入不上拉
        kPORT_SlowSlewRate,                     // 低速翻转模式
        kPORT_PassiveFilterDisable,             // 无源滤波关闭
        kPORT_OpenDrainDisable,                 // 推挽输出
        kPORT_HighDriveStrength,                // 高驱动能力
        kPORT_MuxAsGpio,                        // 复用成GPIO(输出)
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    PORT_SetPinConfig(PORTA, 17, &gpio_leds);        // PTA17 ~ R
    PORT_SetPinConfig(PORTC,  0, &gpio_leds);        // PTC0  ~ B
    // PORT_SetPinConfig(PORTD, 15, &gpio_leds);        // PTD15 ~ G
    PORT_SetPinConfig(PORTE, 29, &gpio_leds);        // PTE29 ~ B
    
    PORT_SetPinConfig(PORTC, 13, &gpio_leds);         // PTC13 ~ 蜂鸣器
}

/*!
 * @brief Initialize ALL pins used by LCD
 */
void InitPins_GPIO_LCD(void)
{
    /* Output pin PORT configuration */
    port_pin_config_t gpio_lcd = 
    {
        kPORT_PullUp,                           // 内部输入上拉
        kPORT_FastSlewRate,                     // 高速翻转模式
        kPORT_PassiveFilterEnable,             // 无源滤波关闭
        kPORT_OpenDrainDisable,                 // 推挽输出
        kPORT_HighDriveStrength,                // 高驱动能力
        kPORT_MuxAsGpio,                        // 复用成GPIO(输出)
        kPORT_UnlockRegister,                   // 0 ~ kPORT_UnLockRegister, 1 ~ kPORT_LockRegister
    };
    
    PORT_SetPinConfig(PORTC, 12, &gpio_lcd);    // PTC12 ~ RD
    PORT_SetPinConfig(PORTC, 19, &gpio_lcd);    // PTC19 ~ RS
    PORT_SetPinConfig(PORTC, 18, &gpio_lcd);    // PTC18 ~ WR
    PORT_SetPinConfig(PORTD,  5, &gpio_lcd);    // PTD5  ~ RST
    PORT_SetPinConfig(PORTD,  4, &gpio_lcd);    // PTD4  ~ CS
    
    PORT_SetPinConfig(PORTD,  9, &gpio_lcd);    // PTD9  ~ D7
    PORT_SetPinConfig(PORTD,  8, &gpio_lcd);    // PTD8  ~ D6
    PORT_SetPinConfig(PORTD, 11, &gpio_lcd);    // PTD11 ~ D5
    PORT_SetPinConfig(PORTD, 10, &gpio_lcd);    // PTD10 ~ D4
    PORT_SetPinConfig(PORTD, 13, &gpio_lcd);    // PTD13 ~ D3
    PORT_SetPinConfig(PORTD, 12, &gpio_lcd);    // PTD12 ~ D2
    PORT_SetPinConfig(PORTD, 15, &gpio_lcd);    // PTD15 ~ D1
    PORT_SetPinConfig(PORTD, 14, &gpio_lcd);    // PTD14 ~ D0
}

/*!
 * @brief Initialize ALL pins used ON OUR BOARD
 */
void BOARD_InitPins(void)
{
    
    //  port_pin_config_t config = 
    //  {
    //      kPORT_PullUp,
    //      kPORT_FastSlewRate,
    //      kPORT_PassiveFilterDisable,
    //      kPORT_OpenDrainDisable,
    //      kPORT_LowDriveStrength,
    //      kPORT_MuxAlt7,
    //      0,
    //  };
    //  PORT_SetPinConfig(PORTA, 6, &config);
    // 复用PTA6查看FlexBus波形
    
    PORT_SetPinMux(PORTA,  4U, kPORT_MuxAsGpio);
    
    InitPins_UART0_LPC824();
    InitPins_UART4_UpperCOM();
    InitPins_ADC1_AD8210();
    InitPins_I2C1_VL53L0X();
    InitPins_I2C3_SCCB();
    InitPins_SPI0_L3G4200D_SCREEN();
    InitPins_SPI1_NRF2401();
    InitPins_SPI2_ADNS9818();
    InitPins_SDHC();
    InitPins_FTMx_QD_Encoder();
    InitPins_FTM0CHx_Motor();
    InitPins_FTM1_QuadDecoder();
    InitPins_FTM2_QuadDecoder();
    InitPins_FTM3CHx_Servo();
    InitPins_GPIO_Camera();
    InitPins_GPIO_KEY();
    InitPins_GPIO_LED();
    InitPins_GPIO_LCD();
}
