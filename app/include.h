#ifndef _INCLUDE_ALL_
#define _INCLUDE_ALL_

typedef float float32_t;
/*
 *      MKV58F24�Ĵ�����Ϣ
 */
#include "fsl_device_registers.h"  // MCU�Ĵ�����ַ
#include "fsl_debug_console.h"     // debug console ���õ�ʱ�����ע�͵�

/*
 *      Standard Library
 */
#include <stdio.h>                 // ��׼���������
#include <stdint.h>                // �����������Ͷ���(uint8_t_t etc.)
#include <stdlib.h>                // malloc��ͨ�ú�����
#include <stdarg.h>                // �䳤����������
#include <string.h>                // �ַ��������
#include <math.h>

/*
 *      fsl peripheral drivers
 */
#include "fsl_common.h"
#include "fsl_clock.h"             // MCG�����������������ʱ��
#include "fsl_smc.h"               // ϵͳģʽ�����������ڹ���״̬�л�
#include "fsl_xbara.h"             // �������A
#include "fsl_xbarb.h"             // �������B
#include "fsl_aoi.h"               //
#include "fsl_edma.h"              // edma����
#include "fsl_dmamux.h"            // dma mux�������
#include "fsl_port.h"              // port���������ڹܽŷ���
                                      
#include "fsl_gpio.h"              // gpio���������ڲ���GPIO
#include "fsl_adc16.h"             // adc����
#include "fsl_hsadc.h"             // ����ADC����
#include "fsl_i2c.h"               // i2c����
#include "fsl_dspi.h"              // spi����
#include "fsl_uart.h"              // uart����
// #include "fsl_sdhc.h"              // sdhc����
#include "fsl_flash.h"             // flash����
#include "fsl_lptmr.h"             // lptmr����
#include "fsl_pit.h"               // pit����
#include "fsl_ftm.h"               // ftm����

/*
 *      fsl MK66F18 utilities
 */
#include "fsl_debug_console.h"

/*
 *      Board pre-Initialization & Sensor drivers
 */
#include "clock_config.h"          // ʱ�������ļ�
#include "board.h"                 // board��Ϣ
#include "pin_mux.h"               // �ܽŸ����ļ�

// #include "ALEX_TVP5150.h"          // PAL-BT656ת����
// #include "ALEX_L3G4200D.h"         // ������
// #include "ALEX_NRF24L01.h"         // ����ͨ��
// #include "ALEX_VL53L0X.h"          // ����
#include "ALEX_LCD_ST7735.h"       // СҺ��
#include "ALEX_OLED_SSD1351.h"
// #include "VCAN_OV7725_Eagle.h"     // ɽ��ӥ��(OV7725)
#include "ALEX_MT9V034.h"
     
/*
 *      fsl middlewares
 */
// #include "fsl_card.h"             // fatfs����
// #include "fsl_sdmmc.h"            // fatfs����
// #include "fsl_sd_disk.h"          // fatfs����
// #include "fsl_mmc_disk.h"         // fatfs����
// #include "sdhc_config.h"          // fatfs����
// #include "diskio.h"               // fatfs����
// #include "ff.h"                   // fatfs�ӿ�

/*
 *      Self-Write middlewares
 */
#include "MW_ALEX_Terminal.h"

/*
 *      Upper Applications
 */
#include "PART_I2C&&SCCB.h"
#include "PART_UpperCOM.h"
     
extern void Init_ALL(void);
extern __ramfunc void ALEX_lptmr_DelayMs(uint32_t ms);
extern __ramfunc void ALEX_CPU_Delay(volatile uint32_t cnt);

#endif
