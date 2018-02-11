#ifndef _PART_OLED_
#define _PART_OLED_

#define _LPLD

#include "include.h"

#define OLED_CMD  0U	//Ð´ÃüÁî
#define OLED_DATA 1U	//Ð´Êý¾Ý

#define OLED_PLOT  0U	//Ìî³ä
#define OLED_ERASE 1U	//²Á³ý

extern unsigned char ASCII_Eng[95][5];

typedef struct OLEDCOLOR
{
    GPIO_Type *ptx;
    uint8 SCK;
    uint8 SDA;
    uint8 CS;
    uint8 DC;
    uint8 RES;
}OLEDn;

//void OLED_SCK_H(GPIO_Type *ptx, uint8 SCK);
//void OLED_SCK_L(GPIO_Type *ptx, uint8 SCK);
//void OLED_SDA_H(GPIO_Type *ptx, uint8 SCK);
//void OLED_SDA_L(GPIO_Type *ptx, uint8 SCK);

extern void Show_String(OLEDn *OLED, uint8 *Data_Pointer, uint8 MSB_Format, uint8 LSB_Format, uint8 X, uint8 Y);
extern void Fill_RAM(OLEDn *OLED, uint8 a, uint8 b);
extern void OLED_Init(OLEDn *OLED);

#define OLED_SCK_H PTB10_O=1
#define OLED_SCK_L PTB10_O=0
#define OLED_SDA_H PTB11_O=1
#define OLED_SDA_L PTB11_O=0
//#define OLED_CS_H PTB2_O=1
//#define OLED_CS_L PTB2_O=0

#endif




