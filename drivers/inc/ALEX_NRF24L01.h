#ifndef _ALEX_NRF24L01_
#define _ALEX_NRF24L01_

#include "include.h"

#define NRF24L01_DelayMs(n)    ALEX_lptmr_DelayMs(n)
#define NRF24L01_Delay500Ms(n) ALEX_lptmr_DelayMs(n*500)
#define NRF24L01_Delay(n)      ALEX_CPU_Delay(n)

#define NRF24L01_CS    PTB10
#define NRF24L01_CS_L  GPIOB->PDOR &= ~(1U<<10U)
#define NRF24L01_CS_H  GPIOB->PDOR |= (1U<<10U)
// #define NRF24L01_CE   (GPIOB->PDIR&(1U<<9U))
#define NRF24L01_CE_L  GPIOB->PDOR &= ~(1U<<9U)
#define NRF24L01_CE_H  GPIOB->PDOR |= (1U<<9U)
#define NRF24L01_IRQ  (GPIOB->PDIR&(1U<<8U))

//////////////////////////////////////////////////////////////////////////////////////////////////////////
extern volatile uint8_t NRF24L01_CONFIG;

typedef enum
{
	MODE_TX = 0,
	MODE_RX
}nRf24l01ModeType;

typedef enum
{
	SPEED_250K = 0,
	SPEED_1M,
	SPEED_2M
}nRf24l01SpeedType;

typedef enum
{
	POWER_F18DBM = 0,
	POWER_F12DBM,
	POWER_F6DBM,
	POWER_0DBM
}nRf24l01PowerType;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/** NRF配置和选项定义 */
#define DYNAMIC_PACKET      1 		//1:动态数据包, 0:固定
#define FIXED_PACKET_LEN    32		//包长度
#define REPEAT_CNT          12		//重复次数
#define INIT_ADDR           0x15,0x26,0x16,0x23,0x26
// #define INIT_ADDR           0xE7,0xE7,0xE7,0xE7,0xE7
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//寄存器操作命令
#define NRF_READ_REG    0x00	//读配置寄存器，低5位为寄存器地址
#define NRF_WRITE_REG   0x20	//写配置寄存器，低5位为寄存器地址
#define RD_RX_PLOAD     0x61	//读RX有效数据，1~32字节
#define WR_TX_PLOAD     0xA0	//写TX有效数据，1~32字节
#define FLUSH_TX        0xE1	//清除TX FIFO寄存器，发射模式下使用
#define FLUSH_RX        0xE2	//清除RX FIFO寄存器，接收模式下使用
#define REUSE_TX_PL     0xE3	//重新使用上一包数据，CE为高，数据包被不断发送
#define R_RX_PL_WID     0x60
#define NOP             0xFF	//空操作，可以用来读状态寄存器
#define W_ACK_PLOAD		0xA8
#define WR_TX_PLOAD_NACK 0xB0
//SPI(NRF24L01)寄存器地址
#define CONFIG          0x00	//配置寄存器地址，bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;
							    //bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能	
#define EN_AA           0x01	//使能自动应答功能 bit0~5 对应通道0~5
#define EN_RXADDR       0x02	//接收地址允许 bit0~5 对应通道0~5
#define SETUP_AW        0x03	//设置地址宽度(所有数据通道) bit0~1: 00,3字节 01,4字节, 02,5字节
#define SETUP_RETR      0x04	//建立自动重发;bit0~3:自动重发计数器;bit4~7:自动重发延时 250*x+86us
#define RF_CH           0x05	//RF通道,bit0~6工作通道频率
#define RF_SETUP        0x06	//RF寄存器，bit3:传输速率( 0:1M 1:2M);bit1~2:发射功率;bit0:噪声放大器增益
#define STATUS          0x07	//状态寄存器;bit0:TX FIFO满标志;bit1~3:接收数据通道号(最大:6);bit4:达到最多次重发次数
								//bit5:数据发送完成中断;bit6:接收数据中断
#define MAX_TX  	0x10	//达到最大发送次数中断
#define TX_OK   	0x20	//TX发送完成中断
#define RX_OK   	0x40	//接收到数据中断

#define OBSERVE_TX      0x08	//发送检测寄存器,bit7~4:数据包丢失计数器;bit3~0:重发计数器
#define CD              0x09	//载波检测寄存器,bit0:载波检测
#define RX_ADDR_P0      0x0A	//数据通道0接收地址，最大长度5个字节，低字节在前
#define RX_ADDR_P1      0x0B	//数据通道1接收地址，最大长度5个字节，低字节在前
#define RX_ADDR_P2      0x0C	//数据通道2接收地址，最低字节可设置，高字节，必须同RX_ADDR_P1[39:8]相等
#define RX_ADDR_P3      0x0D	//数据通道3接收地址，最低字节可设置，高字节，必须同RX_ADDR_P1[39:8]相等
#define RX_ADDR_P4      0x0E	//数据通道4接收地址，最低字节可设置，高字节，必须同RX_ADDR_P1[39:8]相等
#define RX_ADDR_P5      0x0F	//数据通道5接收地址，最低字节可设置，高字节，必须同RX_ADDR_P1[39:8]相等
#define TX_ADDR         0x10	//发送地址(低字节在前),ShockBurstTM模式下，RX_ADDR_P0与地址相等
#define RX_PW_P0        0x11	//接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1        0x12	//接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2        0x13	//接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3        0x14	//接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4        0x15	//接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5        0x16	//接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define NRF_FIFO_STATUS 0x17	//FIFO状态寄存器;bit0:RX FIFO寄存器空标志;bit1:RX FIFO满标志;bit2~3保留
								//bit4:TX FIFO 空标志;bit5:TX FIFO满标志;bit6:1,循环发送上一数据包.0,不循环								
#define DYNPD			0x1C
#define FEATRUE			0x1D
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//位定义
#define MASK_RX_DR   	6U 
#define MASK_TX_DS   	5U 
#define MASK_MAX_RT  	4U 
#define EN_CRC       	3U 
#define CRCO         	2U 
#define PWR_UP       	1U 
#define PRIM_RX      	0U 

#define ENAA_P5      	5U 
#define ENAA_P4      	4U 
#define ENAA_P3      	3U 
#define ENAA_P2      	2U 
#define ENAA_P1      	1U 
#define ENAA_P0      	0U 

#define ERX_P5       	5U 
#define ERX_P4       	4U 
#define ERX_P3       	3U 
#define ERX_P2      	2U 
#define ERX_P1       	1U 
#define ERX_P0       	0U 

#define AW_RERSERVED 	0x00 
#define AW_3BYTES    	0x01
#define AW_4BYTES    	0x02
#define AW_5BYTES    	0x03

#define ARD_250US    	(0x00<<4U)
#define ARD_500US    	(0x01<<4U)
#define ARD_750US    	(0x02<<4U)
#define ARD_1000US   	(0x03<<4U)
#define ARD_2000US   	(0x07<<4U)
#define ARD_4000US   	(0x0F<<4U)
#define ARC_DISABLE   	0x00
#define ARC_15        	0x0F

#define CONT_WAVE     	7U 
#define RF_DR_LOW     	5U 
#define PLL_LOCK      	4U 
#define RF_DR_HIGH    	3U 
//bit2-bit1:
#define PWR_18DB  		(0x00<<1U)
#define PWR_12DB  		(0x01<<1U)
#define PWR_6DB   		(0x02<<1U)
#define PWR_0DB   		(0x03<<1U)

#define RX_DR         	6U 
#define TX_DS         	5U 
#define MAX_RT        	4U 
//for bit3-bit1, 
#define TX_FULL_0     	0U

#define RPD           	0U

#define TX_REUSE      	6U
#define TX_FULL_1     	5U
#define TX_EMPTY      	4U
//bit3-bit2, reserved, only '00'
#define RX_FULL       	1U
#define RX_EMPTY      	0U

#define DPL_P5        	5U
#define DPL_P4        	4U
#define DPL_P3        	3U
#define DPL_P2        	2U
#define DPL_P1        	1U
#define DPL_P0        	0U

#define EN_DPL        	2U
#define EN_ACK_PAY    	1U
#define EN_DYN_ACK    	0U
#define IRQ_ALL  ((1<<RX_DR)|(1<<TX_DS)|(1<<MAX_RT))
//////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t NRF24L01_ReadReg(uint8_t reg);
void NRF24L01_WriteReg(uint8_t reg, uint8_t wdata);
void NRF24L01_BurstRead(uint8_t reg, uint8_t *pBuff, uint8_t len);
void NRF24L01_BurstWrite(uint8_t reg, uint8_t *pBuff, uint8_t len);
inline void NRF24L01_FlushRxFIFO(void);
inline void NRF24L01_FlushTxFIFO(void);
inline void NRF24L01_ReuseLastTxPayload(void);
inline void NRF24L01_NOP(void);
uint8_t NRF24L01_GetStatusRegister();
uint8_t NRF24L01_GetFIFOWidth();
inline uint8_t NRF24L01_GetIRQnStatus();
uint8_t NRF24L01_ClearIRQnFlag(uint8_t IRQ_Source);
uint8_t NRF24L01_ReadRxPayload(uint8_t *pBuff);
void NRF24L01_WriteTxPayload_ACK(uint8_t *pBuff, uint8_t len);
void NRF24L01_WriteTxPayload_NOACK(uint8_t *pBuff, uint8_t len);
void NRF24L01_WriteTxPayload_InRx_ACK(uint8_t *pBuff, uint8_t len);
inline void NRF24L01_SetTxAddr(uint8_t *pBuff, uint8_t len);
inline void NRF24L01_SetRxAddr(uint8_t PipeNum, uint8_t *pBuff, uint8_t len);
void NRF24L01_SetBaudrate(nRf24l01SpeedType BaudRate);
void NRF24L01_SetPower(nRf24l01PowerType Power);
inline void RF24LL01_SetHoppingPoint(uint8_t FreqPoint);


extern uint8_t NRF24L01_TxPacket(void *txbuf, uint8_t Length);
extern uint8_t NRF24L01_RxPacket(void *rxbuf);
extern void NRF24L01_TxRxISR(void);
extern void NRF24L01_Check(void);
extern void NRF24L01_Init(void);
extern void NRF24L01_SetMode(nRf24l01ModeType Mode);
#endif