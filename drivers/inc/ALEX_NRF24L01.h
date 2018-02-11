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
/** NRF���ú�ѡ��� */
#define DYNAMIC_PACKET      1 		//1:��̬���ݰ�, 0:�̶�
#define FIXED_PACKET_LEN    32		//������
#define REPEAT_CNT          12		//�ظ�����
#define INIT_ADDR           0x15,0x26,0x16,0x23,0x26
// #define INIT_ADDR           0xE7,0xE7,0xE7,0xE7,0xE7
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//�Ĵ�����������
#define NRF_READ_REG    0x00	//�����üĴ�������5λΪ�Ĵ�����ַ
#define NRF_WRITE_REG   0x20	//д���üĴ�������5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61	//��RX��Ч���ݣ�1~32�ֽ�
#define WR_TX_PLOAD     0xA0	//дTX��Ч���ݣ�1~32�ֽ�
#define FLUSH_TX        0xE1	//���TX FIFO�Ĵ���������ģʽ��ʹ��
#define FLUSH_RX        0xE2	//���RX FIFO�Ĵ���������ģʽ��ʹ��
#define REUSE_TX_PL     0xE3	//����ʹ����һ�����ݣ�CEΪ�ߣ����ݰ������Ϸ���
#define R_RX_PL_WID     0x60
#define NOP             0xFF	//�ղ���������������״̬�Ĵ���
#define W_ACK_PLOAD		0xA8
#define WR_TX_PLOAD_NACK 0xB0
//SPI(NRF24L01)�Ĵ�����ַ
#define CONFIG          0x00	//���üĴ�����ַ��bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
							    //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��	
#define EN_AA           0x01	//ʹ���Զ�Ӧ���� bit0~5 ��Ӧͨ��0~5
#define EN_RXADDR       0x02	//���յ�ַ���� bit0~5 ��Ӧͨ��0~5
#define SETUP_AW        0x03	//���õ�ַ���(��������ͨ��) bit0~1: 00,3�ֽ� 01,4�ֽ�, 02,5�ֽ�
#define SETUP_RETR      0x04	//�����Զ��ط�;bit0~3:�Զ��ط�������;bit4~7:�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05	//RFͨ��,bit0~6����ͨ��Ƶ��
#define RF_SETUP        0x06	//RF�Ĵ�����bit3:��������( 0:1M 1:2M);bit1~2:���书��;bit0:�����Ŵ�������
#define STATUS          0x07	//״̬�Ĵ���;bit0:TX FIFO����־;bit1~3:��������ͨ����(���:6);bit4:�ﵽ�����ط�����
								//bit5:���ݷ�������ж�;bit6:���������ж�
#define MAX_TX  	0x10	//�ﵽ����ʹ����ж�
#define TX_OK   	0x20	//TX��������ж�
#define RX_OK   	0x40	//���յ������ж�

#define OBSERVE_TX      0x08	//���ͼ��Ĵ���,bit7~4:���ݰ���ʧ������;bit3~0:�ط�������
#define CD              0x09	//�ز����Ĵ���,bit0:�ز����
#define RX_ADDR_P0      0x0A	//����ͨ��0���յ�ַ����󳤶�5���ֽڣ����ֽ���ǰ
#define RX_ADDR_P1      0x0B	//����ͨ��1���յ�ַ����󳤶�5���ֽڣ����ֽ���ǰ
#define RX_ADDR_P2      0x0C	//����ͨ��2���յ�ַ������ֽڿ����ã����ֽڣ�����ͬRX_ADDR_P1[39:8]���
#define RX_ADDR_P3      0x0D	//����ͨ��3���յ�ַ������ֽڿ����ã����ֽڣ�����ͬRX_ADDR_P1[39:8]���
#define RX_ADDR_P4      0x0E	//����ͨ��4���յ�ַ������ֽڿ����ã����ֽڣ�����ͬRX_ADDR_P1[39:8]���
#define RX_ADDR_P5      0x0F	//����ͨ��5���յ�ַ������ֽڿ����ã����ֽڣ�����ͬRX_ADDR_P1[39:8]���
#define TX_ADDR         0x10	//���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ�£�RX_ADDR_P0���ַ���
#define RX_PW_P0        0x11	//��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12	//��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13	//��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14	//��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15	//��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16	//��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_FIFO_STATUS 0x17	//FIFO״̬�Ĵ���;bit0:RX FIFO�Ĵ����ձ�־;bit1:RX FIFO����־;bit2~3����
								//bit4:TX FIFO �ձ�־;bit5:TX FIFO����־;bit6:1,ѭ��������һ���ݰ�.0,��ѭ��								
#define DYNPD			0x1C
#define FEATRUE			0x1D
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//λ����
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