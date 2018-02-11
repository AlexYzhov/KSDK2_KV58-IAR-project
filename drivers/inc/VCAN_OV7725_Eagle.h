/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_OV7725_Eagle.h
 * @brief      ӥ��OV7725��������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-07
 */

#ifndef _VCAN_OV7725_EAGLE_H_
#define _VCAN_OV7725_EAGLE_H_

#include "VCAN_OV7725_REG.h"

#if ( USE_CAMERA == CAMERA_OV7725_EAGLE )
//��������ͷ������
#define CAMERA_USE_HREF     0               //�Ƿ�ʹ�� ���ж� (0 Ϊ ��ʹ�ã�1Ϊʹ��)
#define CAMERA_COLOR        0               //����ͷ�����ɫ �� 0 Ϊ �ڰ׶�ֵ��ͼ�� ��1 Ϊ �Ҷ� ͼ�� ��2 Ϊ RGB565 ͼ��
#define CAMERA_POWER        0               //����ͷ ��Դѡ�� 0 Ϊ 3.3V ,1 Ϊ 5V

//��������ͷ����ӿ�
#define Camera_Init(ImgAddr)    OV7725_Eagle_Init(ImgAddr)
#define Camera_Get_Img()        OV7725_Eagle_Get_Img()

#define Camera_VSYNC(IMG)       OV7725_Eagle_VSYNC(IMG)
#define Camera_HREF()           //OV7725_Eagle_HREF()
#define Camera_DMA()            OV7725_Eagle_DMA()

//���� ����ͷ ����
#define CAMERA_DMA_CH       OV7725_EAGLE_DMA_CH         //��������ͷ��DMA�ɼ�ͨ��
#define CAMERA_W            OV7725_EAGLE_W              //��������ͷͼ����
#define CAMERA_H            OV7725_EAGLE_H              //��������ͷͼ��߶�

#define CAMERA_SIZE         OV7725_EAGLE_SIZE           //ͼ��ռ�ÿռ��С
#define CAMERA_DMA_NUM      OV7725_EAGLE_DMA_NUM        //DMA�ɼ�����

#endif  //#if ( USE_CAMERA == CAMERA_OV7725_EAGLE )

//��������ͷ ����
#define OV7725_EAGLE_DMA_CH       DMA0_DMA16_IRQn                       //��������ͷ��DMA�ɼ�ͨ��
#define OV7725_EAGLE_W            160                                    //��������ͷͼ���� 80
#define OV7725_EAGLE_H            120                                    //��������ͷͼ��߶� 60
#define OV7725_EAGLE_SIZE         (OV7725_EAGLE_W * OV7725_EAGLE_H/8)   //ͼ��ռ�ÿռ��С(OV7725_EAGLE_W * OV7725_EAGLE_H/8 )
#define OV7725_EAGLE_DMA_NUM      (OV7725_EAGLE_SIZE)                   //DMA�ɼ�����


typedef struct
{
    uint8_t Addr;                 /*�Ĵ�����ַ*/
    uint8_t val;                   /*�Ĵ���ֵ*/
} reg_s;

//����ͼ��ɼ�״̬
typedef enum
{
    IMG_NOTINIT = 0,
    IMG_FINISH,             //ͼ��ɼ����
    IMG_FAIL,               //ͼ��ɼ�ʧ��(�ɼ���������)
    IMG_GATHER,             //ͼ��ɼ���
    IMG_START,              //��ʼ�ɼ�ͼ��
    IMG_STOP,               //��ֹͼ��ɼ�
} IMG_STATUS_e;

extern reg_s   OV7725_Eagle_reg[];
extern uint8_t OV7725_Eagle_cfgnum;

extern  uint8_t   OV7725_Eagle_Init(uint8_t* ImgAddr);
extern  void    OV7725_Eagle_Get_Img(void);

extern  void    OV7725_Eagle_VSYNC(uint8_t* IMG);
extern  void    OV7725_Eagle_DMA(void);

extern void Img_Extract(void *dst, void *src, uint32_t srclen);

#endif  //_VCAN_OV7725_EAGLE_H_


