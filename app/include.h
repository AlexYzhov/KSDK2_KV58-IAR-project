#ifndef _INCLUDE_ALL_
#define _INCLUDE_ALL_

typedef float float32_t;
/*
 *      MKV58F24寄存器信息
 */
#include "fsl_device_registers.h"  // MCU寄存器地址
#include "fsl_debug_console.h"     // debug console 不用的时候可以注释掉

/*
 *      Standard Library
 */
#include <stdio.h>                 // 标准输入输出库
#include <stdint.h>                // 常用数据类型定义(uint8_t_t etc.)
#include <stdlib.h>                // malloc等通用函数库
#include <stdarg.h>                // 变长参数库依赖
#include <string.h>                // 字符串处理库
#include <math.h>

/*
 *      fsl peripheral drivers
 */
#include "fsl_common.h"
#include "fsl_clock.h"             // MCG相关驱动，用于配置时钟
#include "fsl_smc.h"               // 系统模式控制器，用于功耗状态切换
#include "fsl_edma.h"              // edma驱动
#include "fsl_dmamux.h"            // dma mux相关驱动
#include "fsl_port.h"              // port驱动，用于管脚服用
                                      
#include "fsl_gpio.h"              // gpio驱动，用于操作GPIO
#include "fsl_adc16.h"             // adc驱动
#include "fsl_hsadc.h"             // 高速ADC驱动
#include "fsl_i2c.h"               // i2c驱动
#include "fsl_dspi.h"              // spi驱动
#include "fsl_uart.h"              // uart驱动
// #include "fsl_sdhc.h"              // sdhc驱动
#include "fsl_flash.h"             // flash驱动
#include "fsl_lptmr.h"             // lptmr驱动
#include "fsl_pit.h"               // pit驱动
#include "fsl_ftm.h"               // ftm驱动

/*
 *      fsl MK66F18 utilities
 */
#include "fsl_debug_console.h"

/*
 *      Board pre-Initialization & Sensor drivers
 */
#include "clock_config.h"          // 时钟配置文件
#include "board.h"                 // board信息
#include "pin_mux.h"               // 管脚复用文件

// #include "ALEX_TVP5150.h"          // PAL-BT656转换器
// #include "ALEX_L3G4200D.h"         // 陀螺仪
// #include "ALEX_NRF24L01.h"         // 无线通信
// #include "ALEX_VL53L0X.h"          // 激光
#include "ALEX_LCD_ST7735.h"       // 小液晶
// #include "VCAN_OV7725_Eagle.h"     // 山外鹰眼(OV7725)
#include "ALEX_MT9V034.h"
     
/*
 *      fsl middlewares
 */
// #include "fsl_card.h"             // fatfs依赖
// #include "fsl_sdmmc.h"            // fatfs依赖
// #include "fsl_sd_disk.h"          // fatfs依赖
// #include "fsl_mmc_disk.h"         // fatfs依赖
// #include "sdhc_config.h"          // fatfs依赖
// #include "diskio.h"               // fatfs依赖
// #include "ff.h"                   // fatfs接口

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
