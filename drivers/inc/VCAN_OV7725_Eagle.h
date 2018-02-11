/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_OV7725_Eagle.h
 * @brief      鹰眼OV7725驱动代码
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-07
 */

#ifndef _VCAN_OV7725_EAGLE_H_
#define _VCAN_OV7725_EAGLE_H_

#include "VCAN_OV7725_REG.h"

#if ( USE_CAMERA == CAMERA_OV7725_EAGLE )
//配置摄像头的特性
#define CAMERA_USE_HREF     0               //是否使用 行中断 (0 为 不使用，1为使用)
#define CAMERA_COLOR        0               //摄像头输出颜色 ， 0 为 黑白二值化图像 ，1 为 灰度 图像 ，2 为 RGB565 图像
#define CAMERA_POWER        0               //摄像头 电源选择， 0 为 3.3V ,1 为 5V

//配置摄像头顶层接口
#define Camera_Init(ImgAddr)    OV7725_Eagle_Init(ImgAddr)
#define Camera_Get_Img()        OV7725_Eagle_Get_Img()

#define Camera_VSYNC(IMG)       OV7725_Eagle_VSYNC(IMG)
#define Camera_HREF()           //OV7725_Eagle_HREF()
#define Camera_DMA()            OV7725_Eagle_DMA()

//配置 摄像头 参数
#define CAMERA_DMA_CH       OV7725_EAGLE_DMA_CH         //定义摄像头的DMA采集通道
#define CAMERA_W            OV7725_EAGLE_W              //定义摄像头图像宽度
#define CAMERA_H            OV7725_EAGLE_H              //定义摄像头图像高度

#define CAMERA_SIZE         OV7725_EAGLE_SIZE           //图像占用空间大小
#define CAMERA_DMA_NUM      OV7725_EAGLE_DMA_NUM        //DMA采集次数

#endif  //#if ( USE_CAMERA == CAMERA_OV7725_EAGLE )

//配置摄像头 属性
#define OV7725_EAGLE_DMA_CH       DMA0_DMA16_IRQn                       //定义摄像头的DMA采集通道
#define OV7725_EAGLE_W            160                                    //定义摄像头图像宽度 80
#define OV7725_EAGLE_H            120                                    //定义摄像头图像高度 60
#define OV7725_EAGLE_SIZE         (OV7725_EAGLE_W * OV7725_EAGLE_H/8)   //图像占用空间大小(OV7725_EAGLE_W * OV7725_EAGLE_H/8 )
#define OV7725_EAGLE_DMA_NUM      (OV7725_EAGLE_SIZE)                   //DMA采集次数


typedef struct
{
    uint8_t Addr;                 /*寄存器地址*/
    uint8_t val;                   /*寄存器值*/
} reg_s;

//定义图像采集状态
typedef enum
{
    IMG_NOTINIT = 0,
    IMG_FINISH,             //图像采集完毕
    IMG_FAIL,               //图像采集失败(采集行数少了)
    IMG_GATHER,             //图像采集中
    IMG_START,              //开始采集图像
    IMG_STOP,               //禁止图像采集
} IMG_STATUS_e;

extern reg_s   OV7725_Eagle_reg[];
extern uint8_t OV7725_Eagle_cfgnum;

extern  uint8_t   OV7725_Eagle_Init(uint8_t* ImgAddr);
extern  void    OV7725_Eagle_Get_Img(void);

extern  void    OV7725_Eagle_VSYNC(uint8_t* IMG);
extern  void    OV7725_Eagle_DMA(void);

extern void Img_Extract(void *dst, void *src, uint32_t srclen);

#endif  //_VCAN_OV7725_EAGLE_H_


