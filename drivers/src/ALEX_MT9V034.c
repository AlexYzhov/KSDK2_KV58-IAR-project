#include "include.h"

void MT9V034_WriteReg(uint8_t SlaveAddr, uint8_t RegAddr, uint16_t RegVal)
{
    SCCB_WriteByte_soft(SlaveAddr, RegAddr, (uint8_t)(RegVal>>8));
    SCCB_WriteByte_soft(SlaveAddr, RegVal, (uint8_t)RegVal&0x00FF);
    SCCB_Stop();
}

uint16_t MT9V034_ReadReg(uint8_t SlaveAddr, uint8_t RegAddr)
{
    uint16_t val[2] = {0, 0};

    val[0] = SCCB_ReadByte_soft(SlaveAddr, RegAddr);
    val[1] = SCCB_ReadByte_soft(SlaveAddr, 0xF0);
    SCCB_Stop();
    
    return (uint16_t)((val[0]<<8)|val[1]);
}

///////////////////////////////////////////////////////////////////////////////

void MT9V034_DMA_Init(void)
{

}

bool MT9V034_Reg_Init(uint8_t MT9V034_I2C_ADDR)
{
    uint16_t id = MT9V034_ReadReg(MT9V034_I2C_ADDR, 0x00);
    if(MT9V034_DEFAULT_VERSION != id)
    {
        return false;
    }

    MT9V034_Reset(MT9V034_I2C_ADDR);
    MT9V034_SetFrameRate(MT9V034_I2C_ADDR, 50);
    MT9V034_SetFrameResolution(MT9V034_I2C_ADDR, 160, 120);
    MT9V034_SetAutoExposure(MT9V034_I2C_ADDR, true);

    return true;
}

void MT9V034_Init(void)
{

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
}

void MT9V034_SetFrameRate(uint8_t MT9V034_I2C_ADDR, uint8_t frameRate)
{

}

void MT9V034_SetFrameResolution(uint8_t MT9V034_I2C_ADDR, uint8_t width, uint8_t height)
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

    //data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COL_FLIP);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_READ_MODE, data);

    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_WINDOW_WIDTH,  width);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_WINDOW_HEIGHT, height);
    
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_COL_START, (MT9V034_MAX_WIDTH-width)/2+MT9V034_COL_START_MIN);
    MT9V034_WriteReg(MT9V034_I2C_ADDR, MT9V034_ROW_START, (MT9V034_MAX_HEIGHT-height)/2+MT9V034_ROW_START_MIN);
}

void MT9V034_SetAutoExposure(uint8_t MT9V034_I2C_ADDR, bool enable)
{
    uint16_t reg = 0;
    reg = MT9V034_ReadReg(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE);
}

///////////////////////////////////////////////////////////////////////////////

void MT9V034_VSYNC_Callback(void)
{

}

void MT9V034_DMA_Callback(void)
{

}