#include "ALEX_L3G4200D.h"

#define GYRO_SPI_PORT SPI0
#define GYRO_CSn  PTD0
#define GYRO_SCLK PTD1    //纯粹标记管脚信息
#define GYRO_MOSI PTD2    //纯粹标记管脚信息
#define GYRO_MISO PTD3    //纯粹标记管脚信息


const dspi_command_data_config_t SPI0_L3G_config =
{
    .isPcsContinuous = false,           // 发送结束后释放cs，否则持续拉低cs
    .whichCtar = kDSPI_Ctar0,
    .whichPcs = kDSPI_Pcs0,
    .clearTransferCount = false,
    .isEndOfQueue = false,
};

uint8_t GYRO_SPI_ReadReg(uint8_t reg)
{
    uint8_t rdata = 0;
    DSPI_MasterWriteDataBlocking(SPI0, &SPI0_L3G_config, ((reg|0x80)<<8)|0x00);
    rdata = (uint8_t)DSPI_ReadData(SPI0);
    DSPI_FlushFifo(SPI0, true, true);
    return rdata;
}

void GYRO_SPI_WriteReg(uint8_t reg, uint8_t wdata)
{
    DSPI_MasterWriteDataBlocking(SPI0, &SPI0_L3G_config, ((reg&(~0x80))<<8)|wdata);
}

void GYRO_Init(void)
{
    GYRO_SPI_WriteReg(CTRL_REG1, CTRL_REG1_800HZ|CTRL_REG1_BW00|CTRL_REG1_PD1|CTRL_REG1_YEN);
    ALEX_lptmr_DelayMs(5);
    GYRO_SPI_WriteReg(CTRL_REG2, CTRL_REG2_HPCF0000);
    ALEX_lptmr_DelayMs(5);
    GYRO_SPI_WriteReg(CTRL_REG3, CTRL_REG3_DEFAULT);
    ALEX_lptmr_DelayMs(5);
    GYRO_SPI_WriteReg(CTRL_REG4, CTRL_REG4_2000DPS);
    ALEX_lptmr_DelayMs(5);
    GYRO_SPI_WriteReg(CTRL_REG5, CTRL_REG5_DEFAULT);
}

int16_t GYRO_GetData(GYRO_t* gyro, uint8_t Axis)
{
    int16_t buf[2]={0,0};
    int16_t rate = 0;
    switch(Axis)
    {
        case XAXIS://read XAxis
            buf[0] = GYRO_SPI_ReadReg(OUT_X_L);
            buf[1] = GYRO_SPI_ReadReg(OUT_X_H);
            gyro->XAxis = (int16_t)((buf[1]<<8)|buf[0]);
            break;
        case YAXIS://read YAxis
            buf[0] = GYRO_SPI_ReadReg(OUT_Y_L);
            buf[1] = GYRO_SPI_ReadReg(OUT_Y_H);
            gyro->YAxis = (int16_t)((buf[1]<<8)|buf[0]);
            break;
        case ZAXIS://read ZAxis
            buf[0] = GYRO_SPI_ReadReg(OUT_Z_L);
            buf[1] = GYRO_SPI_ReadReg(OUT_Z_H);
            gyro->ZAxis = (int16_t)((buf[1]<<8)|buf[0]);
            break;
        default:
            return 0;
    }
    rate = (buf[1]<<8)|buf[0];
    return rate;
}