#ifndef _ALEX_LCD_ST7735_
#define _ALEX_LCD_ST7735_

#include "include.h"

/******常用颜色*****/
#define RGB(r,g,b) ( (r>>3)<<11|(g>>2)<<5|(b>>3) ) 

#define RED     0XF800    //红色
#define GREEN   0X07E0    //绿色
#define BLUE    0X001F    //蓝色
#define BRED    0XF81F
#define GRED    0XFFE0    //灰色
#define GBLUE   0X07FF    //
#define BLACK   0X0000    //黑色
#define GRAY    RGB(192, 192, 192)  // 灰色
#define WHITE   0XFFFF    //白色
#define YELLOW  0xFFE0    //黄色

#define FCOLOUR         BLUE    //定义字体颜色
#define BCOLOUR         RED     //定义背景颜色

//提取RGB的3元素，rgb565必须是16位
#define RGB565_R(rgb565)                ((uint8_t)(((uint16_t)(rgb565)>>11) &0x1F))
#define RGB565_G(rgb565)                ((uint8_t)(((uint16_t)(rgb565)>> 5) &0x3F))
#define RGB565_B(rgb565)                ((uint8_t)( (uint16_t)(rgb565)      &0x1F))


#define GRAY_2_RGB565(gray)             ((uint16_t)((((uint8_t)(gray)>>3)<<11)|(((uint8_t)(gray)>>2)<<5)|((uint8_t)(gray)>>3)))

#define RGB565_2_GRAY(rgb565)           ((uint8_t)(((RGB565_R(rgb565)*235+RGB565_G(rgb565)*613+RGB565_B(rgb565)*625)+1)>>8))  //  31*235+63*613+31*625+1  = 255*256

#define LCD_H 128
#define LCD_W 128
#define LCD_DIR_LEFT 0x01

// ------------------------------------------------- //
#define LCD_RD_def  GPIOC12  // GPIOC12
#define LCD_RS_def  GPIOC19  // GPIOC19
#define LCD_WR_def  GPIOC18  // GPIOC18
#define LCD_CS_def  GPIOD4   // GPIOD 4
#define LCD_RST_def GPIOD5   // GPIOD 5

#define LCD_D7_def  GPIOD9   // GPIOD 9
#define LCD_D6_def  GPIOD8   // GPIOD 8
#define LCD_D5_def  GPIOD11  // GPIOD11
#define LCD_D4_def  GPIOD10  // GPIOD10
#define LCD_D3_def  GPIOD13  // GPIOD13
#define LCD_D2_def  GPIOD12  // GPIOD12
#define LCD_D1_def  GPIOD15  // GPIOD15
#define LCD_D0_def  GPIOD14  // GPIOD 0
// ------------------------------------------------- //
#define LCD_RD_OUT   BITBAND_REG(GPIOC->PDOR, 12)
#define LCD_RS_OUT   BITBAND_REG(GPIOC->PDOR, 19)
#define LCD_WR_OUT   BITBAND_REG(GPIOC->PDOR, 18)
#define LCD_CS_OUT   BITBAND_REG(GPIOD->PDOR,  4)
#define LCD_RST_OUT  BITBAND_REG(GPIOD->PDOR,  5)

#define LCD_D7_OUT   BITBAND_REG(GPIOD->PDOR,  9)
#define LCD_D6_OUT   BITBAND_REG(GPIOD->PDOR,  8)
#define LCD_D5_OUT   BITBAND_REG(GPIOD->PDOR, 11)
#define LCD_D4_OUT   BITBAND_REG(GPIOD->PDOR, 10)
#define LCD_D3_OUT   BITBAND_REG(GPIOD->PDOR, 13)
#define LCD_D2_OUT   BITBAND_REG(GPIOD->PDOR, 12)
#define LCD_D1_OUT   BITBAND_REG(GPIOD->PDOR, 15)
#define LCD_D0_OUT   BITBAND_REG(GPIOD->PDOR, 14)
// ------------------------------------------------- //
#define LCD_D7_IN   BITBAND_REG(GPIOD->PDIR,  9)
#define LCD_D6_IN   BITBAND_REG(GPIOD->PDIR,  8)
#define LCD_D5_IN   BITBAND_REG(GPIOD->PDIR, 11)
#define LCD_D4_IN   BITBAND_REG(GPIOD->PDIR, 10)
#define LCD_D3_IN   BITBAND_REG(GPIOD->PDIR, 13)
#define LCD_D2_IN   BITBAND_REG(GPIOD->PDIR, 12)
#define LCD_D1_IN   BITBAND_REG(GPIOD->PDIR, 15)
#define LCD_D0_IN   BITBAND_REG(GPIOD->PDIR, 14)
// ------------------------------------------------- //
#define LCD_D7_DIR   BITBAND_REG(GPIOD->PDDR,  9)
#define LCD_D6_DIR   BITBAND_REG(GPIOD->PDDR,  8)
#define LCD_D5_DIR   BITBAND_REG(GPIOD->PDDR, 11)
#define LCD_D4_DIR   BITBAND_REG(GPIOD->PDDR, 10)
#define LCD_D3_DIR   BITBAND_REG(GPIOD->PDDR, 13)
#define LCD_D2_DIR   BITBAND_REG(GPIOD->PDDR, 12)
#define LCD_D1_DIR   BITBAND_REG(GPIOD->PDDR, 15)
#define LCD_D0_DIR   BITBAND_REG(GPIOD->PDDR, 14)
// ------------------------------------------------- //

typedef union
{
    struct
    {
        uint8_t D6:1;   // PTD 8
        uint8_t D7:1;   // PTD 9
        uint8_t D4:1;   // PTD10
        uint8_t D5:1;   // PTD11
        uint8_t D2:1;   // PTD12
        uint8_t D3:1;   // PTD13
        uint8_t D0:1;   // PTD14
        uint8_t D1:1;   // PTD15
    }bit;
    uint8_t DATA;
}LCD_Data;

typedef struct
{
    uint16_t x;
    uint16_t y;
}Point_t;

typedef struct
{
    uint16_t W;
    uint16_t H;
}Size_t;
    
void LCD_GPIOLON(Point_t site, Size_t size);

extern void LCD_Init(void);
extern void LCD_DrawPoint(Point_t site, uint16_t rgb565);
extern void LCD_DrawPoints(Point_t* site, uint8_t cnt, uint16_t rgb565);
extern void LCD_DrawRectangle(Point_t site, Size_t size, uint16_t rgb565);
extern void LCD_Clear(uint16_t rgb565);
extern void LCD_ClearTerminal(uint16_t rgb565);

extern void LCD_Img_Gray(Point_t site, Size_t size, uint8_t *img);
extern void LCD_Img_Gray_ZOOM(Point_t site, Size_t size, uint8_t *img, Size_t imgsize);

extern void LCD_Char(Point_t site, uint8_t ascii, uint16_t Color, uint16_t bkColor);
extern void LCD_String(Point_t site, uint8_t *Str, uint16_t Color, uint16_t bkColor);
extern void LCD_Int16(Point_t site, int16_t value,uint8_t places, uint16_t Color, uint16_t bkColor);
extern void LCD_Float32(Point_t site, float32_t value, uint8_t places, uint16_t Color, uint16_t bkColor);

#endif