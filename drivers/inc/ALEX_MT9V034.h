#ifndef _ALEX_MT9V034_H_
#define _ALEX_MT9V034_H_

#include "stdint.h"
#include "ALEX_MT9V034_REG.h"

#define MT9V034_H                       (120)
#define MT9V034_W                       (188)
#define MT9V034_SIZE                    (MT9V034_H*MT9V034_W)
#define MT9V034_DMA_CHANNEL             (DMA0_DMA16_IRQn)

#define MT9V034_Delay(ms)    ALEX_lptmr_DelayMs(ms)
///////////////////////////////////////////////////////////
extern int16_t SCCB_WriteByte_soft(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data);
extern int16_t SCCB_ReadByte_soft(uint8_t SlaveAddress, uint8_t REG_Address);
extern void SCCB_Stop(void);
///////////////////////////////////////////////////////////
extern void MT9V034_WriteReg(uint8_t SlaveAddr, uint8_t RegAddr, uint16_t RegVal);
extern uint16_t MT9V034_ReadReg(uint8_t SlaveAddr, uint8_t RegAddr);
///////////////////////////////////////////////////////////
extern void MT9V034_DMA_Init(uint8_t* pMT9V032_IMG_Buff);
extern bool MT9V034_Reg_Init(uint8_t MT9V034_I2C_ADDR);
extern void MT9V034_Init(void);
///////////////////////////////////////////////////////////
extern void MT9V034_Reset(uint8_t MT9V034_I2C_ADDR);
extern void MT9V034_SetReservedReg(uint8_t MT9V034_I2C_ADDR);
extern void MT9V034_SetFrameRate(uint8_t MT9V034_I2C_ADDR, uint8_t frameRate);
extern void MT9V034_SetFrameResolution(uint8_t MT9V034_I2C_ADDR, uint16_t width, uint16_t height);
extern void MT9V034_SetAutoExposure(uint8_t MT9V034_I2C_ADDR, bool enable);
///////////////////////////////////////////////////////////
extern __ramfunc void MT9V034_FrameValid_Callback(uint32_t ISFR_FLAG);
extern __ramfunc void MT9V034_LineValid_Callback(uint16_t line);
extern __ramfunc void MT9V034_DMA_Callback(void);

#endif