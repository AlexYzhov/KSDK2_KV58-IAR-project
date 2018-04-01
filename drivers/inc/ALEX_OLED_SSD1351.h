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
    uint8_t SCK;
    uint8_t SDA;
    uint8_t CS;
    uint8_t DC;
    uint8_t RES;
}OLEDn;

//void OLED_SCK_H(GPIO_Type *ptx, uint8 SCK);
//void OLED_SCK_L(GPIO_Type *ptx, uint8 SCK);
//void OLED_SDA_H(GPIO_Type *ptx, uint8 SCK);
//void OLED_SDA_L(GPIO_Type *ptx, uint8 SCK);

extern void Show_String(OLEDn *OLED, uint8_t *Data_Pointer, uint8_t MSB_Format, uint8_t LSB_Format, uint8_t X, uint8_t Y);
extern void Fill_RAM(OLEDn *OLED, uint8_t a, uint8_t b);
extern void OLED_Init(OLEDn *OLED);

#define OLED_SCK_H GPIOB->PSOR|=(1U<<10U)//PTB10_O=1
#define OLED_SCK_L GPIOB->PCOR|=(1U<<10U)//PTB10_O=0
#define OLED_SDA_H GPIOB->PSOR|=(1U<<11U)//PTB11_O=1
#define OLED_SDA_L GPIOB->PCOR|=(1U<<11U)//PTB11_O=0
//#define OLED_CS_H PTB2_O=1
//#define OLED_CS_L PTB2_O=0

#endif




