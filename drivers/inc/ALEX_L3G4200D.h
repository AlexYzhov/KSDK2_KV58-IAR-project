#ifndef _ALEX_L3G4200D_
#define _ALEX_L3G4200D_

#include "PART_Parameter.h"
#include "include.h"

/*!
 * @brief GYRO数据结构体
 */
typedef struct
{
    volatile int16_t XAxis;
    volatile int16_t YAxis;
    volatile int16_t ZAxis;
}GYRO_t; 

//Definitions of L3G4200D inner registers
#define WHO_AM_I                    0x0F
#define CTRL_REG1                   0x20
#define CTRL_REG2                   0x21
#define CTRL_REG3                   0x22
#define CTRL_REG4                   0x23
#define CTRL_REG5                   0x24
#define REFERENCE                   0x25
#define OUT_TEMP                    0x26
#define STATUS_REG                  0x27
#define OUT_X_L                     0x28
#define OUT_X_H                     0x29
#define OUT_Y_L                     0x2A
#define OUT_Y_H                     0x2B
#define OUT_Z_L                     0x2C
#define OUT_Z_H                     0x2D
#define FIFO_CTRL_REG               0x2E
#define FIFO_SRC_REG                0x2F
#define INT1_CFG                    0x30
#define INT1_SRC                    0x31
#define INT1_TSH_XH                 0x32
#define INT1_TSH_XL                 0x33
#define INT1_TSH_YH                 0x34
#define INT1_TSH_YL                 0x35
#define INT1_TSH_ZH                 0x36
#define INT1_TSH_ZL                 0x37
#define INT1_DURATION               0x38

//Definitions of L3G4200D status registers
#define L3G4200_STATUS_X_READY      0x01
#define L3G4200_STATUS_Y_READY      0x02
#define L3G4200_STATUS_Z_READY      0x04
#define L3G4200_STATUS_XYZ_READY    0x08
#define L3G4200_STATUS_X_OW         0x10
#define ML3G4200_STATUS_Y_OW        0x20
#define L3G4200_STATUS_Z_OW         0x40
#define L3G4200_STATUS_XYZ_OW       0x80

//Definitions of L3G4200D common configs
#define CTRL_REG1_800HZ     0xC0
#define CTRL_REG1_400HZ     0x80
#define CTRL_REG1_200HZ     0x40
#define CTRL_REG1_100HZ     0x00
#define CTRL_REG1_BW11      0x30    // Bandwidth Select. 100HZ:25,   200HZ:70,   400HZ:110, 800HZ:110
#define CTRL_REG1_BW10      0x20    // Bandwidth Select. 100HZ:12.5, 200HZ:12.5, 400HZ:50,  800HZ:50
#define CTRL_REG1_BW01      0x10    // Bandwidth Select. 100HZ:25,   200HZ:25,   400HZ:25,  800HZ:35
#define CTRL_REG1_BW00      0x00    // Bandwidth Select. 100HZ:25,   200HZ:50,   400HZ:20,  800HZ:30
#define CTRL_REG1_PD0       0x00    // Power down Mode
#define CTRL_REG1_PD1       0x08    // Normal Mode/Sleep Mode
#define CTRL_REG1_XEN       0x04    // Enable XAxis
#define CTRL_REG1_YEN       0x02    // Enable YAxis
#define CTRL_REG1_ZEN       0x01    // Enable ZAxis

// High pass filter CUT OFF FREQ configuration. CTRL_REG2[5:4] Configures the reference signal source of HPF, defaults in [0,0]
#define CTRL_REG2_HPCF0000  0x00    // ODR=100HZ:8HZ,    200HZ:15HZ,   400HZ:30HZ,   800HZ:56HZ
#define CTRL_REG2_HPCF0001  0x01    // ODR=100HZ:4HZ,    200HZ:8HZ,    400HZ:15HZ,   800HZ:30HZ
#define CTRL_REG2_HPCF0010  0x02    // ODR=100HZ:2HZ,    200HZ:4HZ,    400HZ:8HZ,    800HZ:15HZ
#define CTRL_REG2_HPCF0011  0x03    // ODR=100HZ:1HZ,    200HZ:2HZ,    400HZ:4HZ,    800HZ:8HZ
#define CTRL_REG2_HPCF0100  0x04    // ODR=100HZ:0.5HZ,  200HZ:1HZ,    400HZ:2HZ,    800HZ:4HZ
#define CTRL_REG2_HPCF0101  0x05    // ODR=100HZ:0.2HZ,  200HZ:0.5HZ,  400HZ:1HZ,    800HZ:2HZ
#define CTRL_REG2_HPCF0110  0x06    // ODR=100HZ:0.1HZ,  200HZ:0.2HZ,  400HZ:0.5HZ,  800HZ:1HZ
#define CTRL_REG2_HPCF0111  0x07    // ODR=100HZ:0.05HZ, 200HZ:0.1HZ,  400HZ:0.2HZ,  800HZ:0.5HZ
#define CTRL_REG2_HPCF1000  0x08    // ODR=100HZ:0.02HZ, 200HZ:0.05HZ, 400HZ:0.1HZ,  800HZ:0.2HZ
#define CTRL_REG2_HPCF1001  0x09    // ODR=100HZ:0.01HZ, 200HZ:0.02HZ, 400HZ:0.05HZ, 800HZ:0.1HZ

#define CTRL_REG3_DEFAULT   0x08    // CTRL_REG3 configures interrupts, defaults in 0x08
                                    // Configures No interrupts, Data ready and no FIFO

#define CTRL_REG4_250DPS    0x00    // CTRL_REG4[7:6]=01, Configures continuesly data refreshing and Data MSB/LSB. Defaults in continues mode and LSB @lower address
#define CTRL_REG4_500DPS    0x10    // CTRL_REG4[5:4], Configures Full Scale speed
#define CTRL_REG4_2000DPS   0x30    // CTRL_REG4[3:0]=x000, Configures Normal self test mode, and 4 wire spi

#define CTRL_REG5_DEFAULT   0x00    // CTRL_REG5 mainly configures peripheral devices. CTRL_REG5[4]=0 disables the HIGH-PASS filter

#define XAXIS 1
#define YAXIS 2
#define ZAXIS 3

extern uint8_t GYRO_SPI_ReadReg(uint8_t reg);
extern void  GYRO_SPI_WriteReg(uint8_t reg, uint8_t wdata);

extern void  GYRO_Init(void);
extern int16_t GYRO_GetData(GYRO_t* gyro, uint8_t Axis);

#endif