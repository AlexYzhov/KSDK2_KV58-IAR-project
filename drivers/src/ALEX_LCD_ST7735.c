#include "include.h"

LCD_Data LCD_BYTE;

void LCD_Write_Data(uint8_t data)
{
#ifdef CPU_MK66FX1M0VLQ18   // Cortex M4支持BITBAND_REG
    LCD_RD_OUT = 1;
    LCD_RS_OUT = 1;
    LCD_CS_OUT = 0;
    
    LCD_D7_OUT = (data&0x80)>>7;
    LCD_D6_OUT = (data&0x40)>>6;
    LCD_D5_OUT = (data&0x20)>>5;
    LCD_D4_OUT = (data&0x10)>>4;
    LCD_D3_OUT = (data&0x08)>>3;
    LCD_D2_OUT = (data&0x04)>>2;
    LCD_D1_OUT = (data&0x02)>>1;
    LCD_D0_OUT = (data&0x01)>>0;
    
    // ALEX_lptmr_DelayMs(1);
    
    LCD_WR_OUT = 0;
    LCD_WR_OUT = 1;
    LCD_CS_OUT = 1;
#else                       // Cortex M7不支持BITBAND_REG
    GPIO_WritePinOutput(GPIOC, 12U, 1);
    GPIO_WritePinOutput(GPIOC, 19U, 1);
    GPIO_WritePinOutput(GPIOD,  4U, 0);
    
    // LCD_BYTE.bit.D7 = (data&0x80)>>7;
    // LCD_BYTE.bit.D6 = (data&0x40)>>6;
    // LCD_BYTE.bit.D5 = (data&0x20)>>5;
    // LCD_BYTE.bit.D4 = (data&0x10)>>4;
    // LCD_BYTE.bit.D3 = (data&0x08)>>3;
    // LCD_BYTE.bit.D2 = (data&0x04)>>2;
    // LCD_BYTE.bit.D1 = (data&0x02)>>1;
    // LCD_BYTE.bit.D0 = (data&0x01)>>0;

    GPIOD->PDOR &= 0xFFFF00FF;
    // GPIOD->PDOR |= (LCD_BYTE.DATA<<8);
    GPIOD->PDOR |= ((uint16_t)data)<<8;
    
    GPIO_WritePinOutput(GPIOC, 18U, 0);
    GPIO_WritePinOutput(GPIOC, 18U, 1);
    GPIO_WritePinOutput(GPIOD,  4U, 1);
#endif
}

void LCD_Write_CMD(uint8_t cmd)
{
#ifdef CPU_MK66FX1M0VLQ18
    LCD_RD_OUT = 1;     // 读禁能
    LCD_RS_OUT = 0;     // cmd
    LCD_CS_OUT = 0;     // 片选使能
    
    LCD_D7_OUT = (cmd&0x80)>>7;
    LCD_D6_OUT = (cmd&0x40)>>6;
    LCD_D5_OUT = (cmd&0x20)>>5;
    LCD_D4_OUT = (cmd&0x10)>>4;
    LCD_D3_OUT = (cmd&0x08)>>3;
    LCD_D2_OUT = (cmd&0x04)>>2;
    LCD_D1_OUT = (cmd&0x02)>>1;
    LCD_D0_OUT = (cmd&0x01)>>0;
    
    // ALEX_lptmr_DelayMs(1);
    
    LCD_WR_OUT = 0;     // 写禁能
    LCD_WR_OUT = 1;     // 写使能
    LCD_CS_OUT = 1;     // 片选禁能
#else
    GPIO_WritePinOutput(GPIOC, 12U, 1);
    GPIO_WritePinOutput(GPIOC, 19U, 0);
    GPIO_WritePinOutput(GPIOD,  4U, 0);
    
    // LCD_BYTE.bit.D7 = (cmd&0x80)>>7;
    // LCD_BYTE.bit.D6 = (cmd&0x40)>>6;
    // LCD_BYTE.bit.D5 = (cmd&0x20)>>5;
    // LCD_BYTE.bit.D4 = (cmd&0x10)>>4;
    // LCD_BYTE.bit.D3 = (cmd&0x08)>>3;
    // LCD_BYTE.bit.D2 = (cmd&0x04)>>2;
    // LCD_BYTE.bit.D1 = (cmd&0x02)>>1;
    // LCD_BYTE.bit.D0 = (cmd&0x01)>>0;

    GPIOD->PDOR &= 0xFFFF00FF;
    // GPIOD->PDOR |= (LCD_BYTE.DATA<<8);
    GPIOD->PDOR |= ((uint16_t)cmd)<<8;
    
    GPIO_WritePinOutput(GPIOC, 18U, 0);
    GPIO_WritePinOutput(GPIOC, 18U, 1);
    GPIO_WritePinOutput(GPIOD,  4U, 1);
#endif
}

void LCD_Dir(uint8_t option)
{
    //st7735r_dir = option;
    switch(option)
    {
        case 0:
            LCD_Write_CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
            LCD_Write_Data(0xc8);//0xc8  0xA8 0x08 0x68
    
            LCD_Write_CMD(0x2a);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x00);
            LCD_Write_Data(128);
    
            LCD_Write_CMD(0x2B);
            LCD_Write_Data(0x00);
            LCD_Write_Data(50);
            LCD_Write_Data(0x00);
            LCD_Write_Data(128+50);
    
            // st7735r_h   = ST7735R_H;
            // st7735r_w   = ST7735R_W;
            break;
        case 1:
            LCD_Write_CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
            LCD_Write_Data(0xA8);//0xc8  0xA8 0x08 0x68
    
            LCD_Write_CMD(0x2a);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x03);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x82);
    
            LCD_Write_CMD(0x2B);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x02);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x81);
    
            // st7735r_h   = ST7735R_W;
            // st7735r_w   = ST7735R_H;
            break;
        case 2:
            LCD_Write_CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
            LCD_Write_Data(0x08);//0xc8  0xA8 0x08 0x68
    
            LCD_Write_CMD(0x2a);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x02);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x81);
    
            LCD_Write_CMD(0x2B);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x03);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x82);
    
            // st7735r_h   = ST7735R_H;
            // st7735r_w   = ST7735R_W;
            break;
        case 3:
            LCD_Write_CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
            LCD_Write_Data(0x68);//0xc8  0xA8 0x08 0x68
    
            LCD_Write_CMD(0x2a);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x03);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x82);
    
            LCD_Write_CMD(0x2B);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x00);
            LCD_Write_Data(0x00);
            LCD_Write_Data(128);
    
            // st7735r_h   = ST7735R_W;
            // st7735r_w   = ST7735R_H;
            break;
        default:
            break;
    }

}

void LCD_Init(void)
{
#ifdef CPU_MK66FX1M0VLQ18
    //初始化总线
    LCD_RST_OUT = 0;
    ALEX_lptmr_DelayMs(5);
    LCD_RST_OUT = 1;
    ALEX_lptmr_DelayMs(50);     //上电给足够时间
#else
    //初始化总线
    GPIO_WritePinOutput(GPIOD,  5U, 0);
    ALEX_lptmr_DelayMs(5);
    GPIO_WritePinOutput(GPIOD,  5U, 1);
    ALEX_lptmr_DelayMs(50);     //上电给足够时间
#endif
    
    
    LCD_Write_CMD(0x11);        //Sleep out   退出睡眠模式
    ALEX_lptmr_DelayMs(50);     //Delay 120ms
    //------------------------------------ST7735R Frame Rate-----------------------------------------//
    /* Set the frame frequency of the full colors normal mode. */
    LCD_Write_CMD(0xB1);        //In Normal Mode (Full Colors)    全屏模式
    LCD_Write_Data(0x05);       //设置 RTNA Set 1-line  Period  一行周期
    LCD_Write_Data(0x3A);       //设置 FPA: Front Porch
    LCD_Write_Data(0x3A);       //设置 BPA: Back Porch
                                //Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
                                //其中 fosc = 850kHz
    
    /* Set the frame frequency of the Idle mode.  */
    LCD_Write_CMD(0xB2);        //In Idle Mode (8-colors)
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x3A);
    LCD_Write_Data(0x3A);
    
    /* Set the frame frequency of the Partial mode/ full colors. */
    LCD_Write_CMD(0xB3);
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x3A);
    LCD_Write_Data(0x3A);
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x3A);
    LCD_Write_Data(0x3A);
    
    //------------------------------------End ST7735R Frame Rate-----------------------------------------//
    LCD_Write_CMD(0xB4);        //Display Inversion Control  显示反转控制
    
    LCD_Write_Data(0x07);       //LCD_Write_Data(0x03); 原来为3，改为7
                                // 低三位从高到低，分别为  full colors normal mode 、Idle mode 、
                                // full Colors partial mode  的 点反转 或 列反转 ，1为列反转
    
    LCD_Write_CMD(0xC0);        //Power Control Setting  电源控制设置
    LCD_Write_Data(0x28);
    LCD_Write_Data(0x08);
    LCD_Write_Data(0x84);
    LCD_Write_CMD(0xC1);
    LCD_Write_Data(0XC0);
    LCD_Write_CMD(0xC2);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x00);
    LCD_Write_CMD(0xC3);
    LCD_Write_Data(0x8C);
    LCD_Write_Data(0x2A);
    LCD_Write_CMD(0xC4);
    LCD_Write_Data(0x8A);
    LCD_Write_Data(0xEE);
    //---------------------------------End ST7735R Power Sequence-------------------------------------//
    LCD_Write_CMD(0xC5);        //  VCOM 电压配置
    LCD_Write_Data(0x0C);       //  -0.725
    //------------------------------------ST7735R Gamma Sequence-----------------------------------------//
    LCD_Write_CMD(0xE0);
    LCD_Write_Data(0x05);
    LCD_Write_Data(0x1A);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x0E);
    LCD_Write_Data(0x3A);
    LCD_Write_Data(0x34);
    LCD_Write_Data(0x2D);
    LCD_Write_Data(0x2F);
    LCD_Write_Data(0x2D);
    LCD_Write_Data(0x2A);
    LCD_Write_Data(0x2F);
    LCD_Write_Data(0x3C);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x01);
    LCD_Write_Data(0x02);
    LCD_Write_Data(0x10);
    
    LCD_Write_CMD(0xE1);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x1B);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x0E);
    LCD_Write_Data(0x2D);
    LCD_Write_Data(0x29);
    LCD_Write_Data(0x24);
    LCD_Write_Data(0x29);
    LCD_Write_Data(0x28);
    LCD_Write_Data(0x26);
    LCD_Write_Data(0x31);
    LCD_Write_Data(0x3B);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x03);
    LCD_Write_Data(0x12);
    //------------------------------------End ST7735R Gamma Sequence-----------------------------------------//
    
    LCD_Write_CMD(0x3A);        //65k mode
    LCD_Write_Data(0x05);
    LCD_Write_CMD(0x29);        //开显示 Display on
    LCD_Write_CMD(0x2C);        //写内存
    
    LCD_Dir(LCD_DIR_LEFT);
    
    LCD_Clear(0x0000);          // 清屏成黑色
}

void LCD_PTLON(Point_t site, Size_t size)
{

    if(LCD_DIR_LEFT&0x01)
    {
        site.x += 32;     //液晶需要偏移一下，避免四周看不到的行
        //site.y += 2;
    }
    else
    {
        //site.x += 2;     //液晶需要偏移一下，避免四周看不到的行
        site.y += 32;
    }

    LCD_Write_CMD(0x2A);   //Partial Mode On  局部模式
    LCD_Write_Data((uint8_t)(site.x >> 8)); //高8位 Sx
    LCD_Write_Data((uint8_t)site.x);   //低8位 Sx
    LCD_Write_Data((uint8_t)((site.x + size.W - 1) >> 8));
    LCD_Write_Data((uint8_t)(site.x + size.W - 1));

    LCD_Write_CMD(0x2B);//Row Address Set  行地址设置
    LCD_Write_Data((uint8_t)(site.y >> 8));
    LCD_Write_Data((uint8_t)site.y);
    LCD_Write_Data((uint8_t)((site.y + size.H - 1) >> 8));
    LCD_Write_Data((uint8_t)(site.y + size.H - 1));

}

void LCD_DrawPoint(Point_t site, uint16_t rgb565)
{
    Size_t size = {1, 1};
    LCD_PTLON(site, size);
    LCD_Write_CMD(0x2C);                   // 写内存
    LCD_Write_Data(rgb565>>8);             // 写数据
    LCD_Write_Data(rgb565&0x00FF);
}

void LCD_DrawPoints(Point_t* site, uint8_t cnt, uint16_t rgb565)
{
    for(int i=0; i<cnt; i++)
    {
        LCD_DrawPoint((*site), rgb565);         //画点
        site += 1;
    }
}

void LCD_DrawRectangle(Point_t site, Size_t size, uint16_t rgb565)
{
    uint32_t n, temp = (uint32_t)size.W * size.H;

    LCD_PTLON(site, size);              // 开窗

    LCD_Write_CMD(0x2C);                // 写内存
    for(n = 0; n < temp; n++)
    {
        LCD_Write_Data(rgb565>>8);    // 写数据
        LCD_Write_Data(rgb565&0x00FF);
    }
}

void LCD_Clear(uint16_t rgb565)
{
    Point_t site = {0,0};
    Size_t size;
    size.H = LCD_H;
    size.W = LCD_W;

    LCD_DrawRectangle(site, size, rgb565);
}

void LCD_ClearTerminal(uint16_t rgb565)
{
    Point_t site = {0,0};
    Size_t size;
    size.H = LCD_H-ASCII_HEIGHT;
    size.W = LCD_W;

    LCD_DrawRectangle(site, size, rgb565);
}

void LCD_Img_Gray(Point_t site, Size_t size, uint8_t *img)
{
    uint32_t     total = (size.H * size.W);
    uint16_t     imgtemp;
    uint8_t     *pimg = (uint8_t *)img;
    
    LCD_PTLON(site, size);                      //开窗
    LCD_Write_CMD(0x2C);                       //写内存
    
    while(total--)
    {
        imgtemp     = (uint16_t) * (pimg++);
        imgtemp = GRAY_2_RGB565(imgtemp);
        LCD_Write_Data(imgtemp>>8);              //写图像数据
        LCD_Write_Data(imgtemp&0x00FF);
    }
}

void LCD_Img_Gray_ZOOM(Point_t site, Size_t size, uint8_t *img, Size_t imgsize)
{

    uint32_t temp, tempY;
    uint16_t x, y;
    uint16_t X, Y;
    uint8_t *pimg = (uint8_t *)img;
    uint16_t rgb;

    LCD_PTLON(site, size);                      //开窗
    LCD_Write_CMD(0x2C);                       //写内存
    
    for(y = 0; y < size.H; y++)
    {
        Y = ( (  y * imgsize.H   ) / size.H) ;
        tempY = Y * imgsize.W ;

        for(x = 0; x < size.W; x++)
        {
            X = ( x * imgsize.W  ) / size.W ;
            temp = tempY + X;
            rgb = GRAY_2_RGB565(pimg[temp]);    
            if(pimg[temp] == 50)
            {
              rgb = 0XF800;              //显示红点
            }
            else if(pimg[temp] == 60)
            {
              rgb = GRAY;               //显示蓝点
            }
            LCD_Write_Data(rgb>>8);
            LCD_Write_Data(rgb&0x00FF);
        }
    }
}

void LCD_Char(Point_t site, uint8_t ascii, uint16_t Color, uint16_t bkColor)
{
#define MAX_CHAR_POSX (LCD_W-8)
#define MAX_CHAR_POSY (LCD_H-16)

    uint8_t temp, t, pos;
    Size_t size = { 8, 16 };

    if(site.x > MAX_CHAR_POSX || site.y > MAX_CHAR_POSY)
    {
        return;
    }

    LCD_PTLON(site, size);

    LCD_Write_CMD(0x2C);                       //写内存

    for (pos = 0; pos < 16; pos++)
    {
        temp = ASCII_8x16[((ascii-0x20)*16)+pos];

        for(t = 0; t < 8; t++)
        {

            if(temp & 0x80)
            {
                LCD_Write_Data(Color>>8);
                LCD_Write_Data(Color&0x00FF);
            }
            else
            {
                LCD_Write_Data(bkColor>>8);
                LCD_Write_Data(bkColor&0x00FF);
            }
            temp <<= 1;
        }
    }
    return;
#undef MAX_CHAR_POSX
#undef MAX_CHAR_POSY
}

void LCD_String(Point_t site, uint8_t *Str, uint16_t Color, uint16_t bkColor)
{
#define MAX_CHAR_POSX (LCD_W-8)
#define MAX_CHAR_POSY (LCD_H-16)
    while(*Str != '\0')
    {
        if(site.x > MAX_CHAR_POSX )
        {
            //换行
            site.x = 0;
            site.y += 16;
        }
        if(site.y > MAX_CHAR_POSY )
        {
            //一屏
            site.x = 0;
            site.y = 0;
        }

        LCD_Char(site, *Str, Color, bkColor);
        site.x += 8;
        Str ++ ;
    }
#undef MAX_CHAR_POSX
#undef MAX_CHAR_POSY
}



/*==================================
 *显示整数  by RUS
 *value 要显示的值
 *places 位数
===================================*/

void LCD_Int16(Point_t site, int16_t value, uint8_t places, uint16_t Color, uint16_t bkColor)
{
  uint8_t *CharValue = malloc(sizeof(uint8_t)*(places+2));
  int16_t sum = 0;
  int16_t placesValue = 0;
  int16_t i = 0;
  if ( value > 0 )
  {
    CharValue[0] = '+';
  }
  else
  {
    CharValue[0] = '-';
    value = -value;
  }
  CharValue[places+1] = '\0';
    
  for( i=1;i<=places;i++)
  {
    placesValue = (int16_t)pow(10,(places-i));
    CharValue[i] = (value-sum) / placesValue; 
    sum = sum + CharValue[i] * placesValue;
    CharValue[i] += 48;
  } 
  LCD_String( site, CharValue, Color, bkColor);
  free(CharValue);
}


/*==================================
 *显示两位小数 by RUS
 *value 要显示的值
 *places 整数位数
===================================*/

void LCD_Float32(Point_t site, float32_t value, uint8_t places, uint16_t Color, uint16_t bkColor)
{
  uint8_t *CharValue = malloc(sizeof(uint8_t)*(places+5));
  uint8_t *CharValueStart = CharValue;
  int16_t sum = 0;
  int16_t placesValue = 0;
  uint8_t i = 0;
  int16_t floatValue = 0;
  int16_t intValue = 0;
  
  
  if ( value > 0 )
  {
    *CharValue = '+';
  }
  else
  {
    *CharValue = '-';
    value = -value;
  }
  CharValue++;
 
 
  intValue = (int16_t)value;
  floatValue = (int16_t)((value - intValue)*100);
    
  for( i=1;i<=places;i++)
  {
    placesValue = (int16_t)pow(10,(places-i));
    *CharValue = (intValue-sum) / placesValue; 
    sum = sum + (*CharValue) * placesValue;
    *CharValue += 48;
    CharValue++;
  }
  *CharValue = '.';
  CharValue++;

    
  sum = 0;
  for( i=1;i<=2;i++)
  {
    placesValue = (int16_t)pow(10,(2-i));
    *CharValue = (floatValue-sum) / placesValue; 
    sum = sum + (*CharValue) * placesValue;
    *CharValue += 48;
    CharValue++;
  }
  *CharValue = '\0';
  
  CharValue = CharValueStart;
  LCD_String( site, CharValue, Color, bkColor);
  free(CharValue);
}