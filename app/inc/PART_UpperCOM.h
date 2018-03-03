#ifndef _PART_UPPERCOM_
#define _PART_UPPERCOM_

#define UpperCOM UART4

extern __ramfunc void UpperCOM_PutBuff(uint8_t *buff, uint32_t len);
extern __ramfunc void UpperCOM_SendImg(void *ImgAddr, uint32_t ImgSize);
extern __ramfunc void UpperCOM_SendWare(void *WareAddr, uint32_t WareSize);

#endif