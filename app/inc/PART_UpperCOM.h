#ifndef _PART_UPPERCOM_
#define _PART_UPPERCOM_

#define UpperCOM UART4

extern void UpperCOM_PutBuff(uint8_t *buff, uint32_t len);
extern void UpperCOM_SendImg(void *ImgAddr, uint32_t ImgSize);
extern void UpperCOM_SendWare(void *WareAddr, uint32_t WareSize);

#endif