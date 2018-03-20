#include "include.h"

LCD_Data LCD_BYTE;

__ramfunc void LCD_Write_Data(uint8_t data)
{
#ifdef CPU_MK66FX1M0VLQ18   // Cortex M4֧��BITBAND_REG
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
#else                       // Cortex M7��֧��BITBAND_REG
    //GPIO_WritePinOutput(GPIOC, 12U, 1);    // RD ~ C12
    //GPIO_WritePinOutput(GPIOC, 19U, 1);    // RS ~ RS
    //GPIO_WritePinOutput(GPIOD,  4U, 0);    // CS ~ D4
    GPIOC->PSOR |= (1U<<12U);
    GPIOC->PSOR |= (1U<<19U);
    GPIOD->PCOR |= (1U<<4U);
    
    LCD_BYTE.bit.D7 = (data>>1)&0x01;      //(data&0x80)>>7;
    LCD_BYTE.bit.D6 = (data>>0)&0x01;      //(data&0x40)>>6;
    LCD_BYTE.bit.D5 = (data>>3)&0x01;      //(data&0x20)>>5;
    LCD_BYTE.bit.D4 = (data>>2)&0x01;      //(data&0x10)>>4;
    LCD_BYTE.bit.D3 = (data>>5)&0x01;      //(data&0x08)>>3;
    LCD_BYTE.bit.D2 = (data>>4)&0x01;      //(data&0x04)>>2;
    LCD_BYTE.bit.D1 = (data>>7)&0x01;      //(data&0x02)>>1;
    LCD_BYTE.bit.D0 = (data>>6)&0x01;      //(data&0x01)>>0;

    GPIOD->PDOR &= 0xFFFF00FF;
    GPIOD->PDOR |= (uint32_t)(LCD_BYTE.DATA<<8);
    
    //GPIO_WritePinOutput(GPIOC, 18U, 0);    // WR ~ C18
    //GPIO_WritePinOutput(GPIOC, 18U, 1);    // WR ~ C18
    //GPIO_WritePinOutput(GPIOD,  4U, 1);    // CS ~ D4
    
    GPIOC->PCOR |= (1U<<18U);
    GPIOC->PSOR |= (1U<<18U);
    GPIOD->PSOR |= (1U<<4U);
#endif
}

__ramfunc void LCD_Write_CMD(uint8_t cmd)
{
#ifdef CPU_MK66FX1M0VLQ18
    LCD_RD_OUT = 1;     // ������
    LCD_RS_OUT = 0;     // cmd
    LCD_CS_OUT = 0;     // Ƭѡʹ��
    
    LCD_D7_OUT = (cmd&0x80)>>7;
    LCD_D6_OUT = (cmd&0x40)>>6;
    LCD_D5_OUT = (cmd&0x20)>>5;
    LCD_D4_OUT = (cmd&0x10)>>4;
    LCD_D3_OUT = (cmd&0x08)>>3;
    LCD_D2_OUT = (cmd&0x04)>>2;
    LCD_D1_OUT = (cmd&0x02)>>1;
    LCD_D0_OUT = (cmd&0x01)>>0;
    
    // ALEX_lptmr_DelayMs(1);
    
    LCD_WR_OUT = 0;     // д����
    LCD_WR_OUT = 1;     // дʹ��
    LCD_CS_OUT = 1;     // Ƭѡ����
#else
    //GPIO_WritePinOutput(GPIOC, 12U, 1);    // RD ~ C12
    //GPIO_WritePinOutput(GPIOC, 19U, 0);    // RS ~ RS
    //GPIO_WritePinOutput(GPIOD,  4U, 0);    // CS ~ D4
    
    GPIOC->PSOR |= (1U<<12U);
    GPIOC->PCOR |= (1U<<19U);
    GPIOD->PCOR |= (1U<<4U);
    
    LCD_BYTE.bit.D7 = (cmd>>1)&0x01;      //(cmd&0x80)>>7;
    LCD_BYTE.bit.D6 = (cmd>>0)&0x01;      //(cmd&0x40)>>6;
    LCD_BYTE.bit.D5 = (cmd>>3)&0x01;      //(cmd&0x20)>>5;
    LCD_BYTE.bit.D4 = (cmd>>2)&0x01;      //(cmd&0x10)>>4;
    LCD_BYTE.bit.D3 = (cmd>>5)&0x01;      //(cmd&0x08)>>3;
    LCD_BYTE.bit.D2 = (cmd>>4)&0x01;      //(cmd&0x04)>>2;
    LCD_BYTE.bit.D1 = (cmd>>7)&0x01;      //(cmd&0x02)>>1;
    LCD_BYTE.bit.D0 = (cmd>>6)&0x01;      //(cmd&0x01)>>0;

    GPIOD->PDOR &= 0xFFFF00FF;
    GPIOD->PDOR |= (uint32_t)(LCD_BYTE.DATA<<8);
    
    //GPIO_WritePinOutput(GPIOC, 18U, 0);    // WR ~ C18
    //GPIO_WritePinOutput(GPIOC, 18U, 1);    // WR ~ C18
    //GPIO_WritePinOutput(GPIOD,  4U, 1);    // CS ~ D4
    
    GPIOC->PCOR |= (1U<<18U);
    GPIOC->PSOR |= (1U<<18U);
    GPIOD->PSOR |= (1U<<4U);
#endif
}

void LCD_Dir(uint8_t option)
{
    //st7735r_dir = option;
    switch(option)
    {
        case 0:
            LCD_Write_CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ��MV ���н��� ��ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
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
            LCD_Write_CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ��MV ���н��� ��ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
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
            LCD_Write_CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ��MV ���н��� ��ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
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
            LCD_Write_CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ��MV ���н��� ��ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
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
    //��ʼ������
    LCD_RST_OUT = 0;
    ALEX_lptmr_DelayMs(5);
    LCD_RST_OUT = 1;
    ALEX_lptmr_DelayMs(50);     //�ϵ���㹻ʱ��
#else
    //��ʼ������
    GPIO_WritePinOutput(GPIOD,  5U, 0);
    ALEX_lptmr_DelayMs(5);
    GPIO_WritePinOutput(GPIOD,  5U, 1);
    ALEX_lptmr_DelayMs(50);     //�ϵ���㹻ʱ��
#endif
    
    
    LCD_Write_CMD(0x11);        //Sleep out   �˳�˯��ģʽ
    ALEX_lptmr_DelayMs(50);     //Delay 120ms
    //------------------------------------ST7735R Frame Rate-----------------------------------------//
    /* Set the frame frequency of the full colors normal mode. */
    LCD_Write_CMD(0xB1);        //In Normal Mode (Full Colors)    ȫ��ģʽ
    LCD_Write_Data(0x05);       //���� RTNA Set 1-line  Period  һ������
    LCD_Write_Data(0x3A);       //���� FPA: Front Porch
    LCD_Write_Data(0x3A);       //���� BPA: Back Porch
                                //Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
                                //���� fosc = 850kHz
    
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
    LCD_Write_CMD(0xB4);        //Display Inversion Control  ��ʾ��ת����
    
    LCD_Write_Data(0x07);       //LCD_Write_Data(0x03); ԭ��Ϊ3����Ϊ7
                                // ����λ�Ӹߵ��ͣ��ֱ�Ϊ  full colors normal mode ��Idle mode ��
                                // full Colors partial mode  �� �㷴ת �� �з�ת ��1Ϊ�з�ת
    
    LCD_Write_CMD(0xC0);        //Power Control Setting  ��Դ��������
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
    LCD_Write_CMD(0xC5);        //  VCOM ��ѹ����
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
    LCD_Write_CMD(0x29);        //����ʾ Display on
    LCD_Write_CMD(0x2C);        //д�ڴ�
    
    LCD_Dir(LCD_DIR_LEFT);
    
    LCD_Clear(GRAY);          // �����ɻ�ɫ
}

__ramfunc void LCD_PTLON(Point_t site, Size_t size)
{

    if(LCD_DIR_LEFT&0x01)
    {
        site.x += 32;     //Һ����Ҫƫ��һ�£��������ܿ���������
        
        //site.x += 3;     //Һ����Ҫƫ��һ�£��������ܿ���������
        //site.y += 2;
    }
    else
    {
        //site.x += 2;     //Һ����Ҫƫ��һ�£��������ܿ���������
        site.y += 32;
    }

    LCD_Write_CMD(0x2A);   //Partial Mode On  �ֲ�ģʽ
    LCD_Write_Data((uint8_t)(site.x >> 8)); //��8λ Sx
    LCD_Write_Data((uint8_t)site.x);   //��8λ Sx
    LCD_Write_Data((uint8_t)((site.x + size.W - 1) >> 8));
    LCD_Write_Data((uint8_t)(site.x + size.W - 1));

    LCD_Write_CMD(0x2B);//Row Address Set  �е�ַ����
    LCD_Write_Data((uint8_t)(site.y >> 8));
    LCD_Write_Data((uint8_t)site.y);
    LCD_Write_Data((uint8_t)((site.y + size.H - 1) >> 8));
    LCD_Write_Data((uint8_t)(site.y + size.H - 1));

}

void LCD_DrawPoint(Point_t site, uint16_t rgb565)
{
    Size_t size = {1, 1};
    LCD_PTLON(site, size);
    LCD_Write_CMD(0x2C);                   // д�ڴ�
    LCD_Write_Data(rgb565>>8);             // д����
    LCD_Write_Data(rgb565&0x00FF);
}

void LCD_DrawPoints(Point_t* site, uint8_t cnt, uint16_t rgb565)
{
    for(int i=0; i<cnt; i++)
    {
        LCD_DrawPoint((*site), rgb565);         //����
        site += 1;
    }
}

void LCD_DrawRectangle(Point_t site, Size_t size, uint16_t rgb565)
{
    uint32_t n, temp = (uint32_t)size.W * size.H;

    LCD_PTLON(site, size);              // ����

    LCD_Write_CMD(0x2C);                // д�ڴ�
    for(n = 0; n < temp; n++)
    {
        LCD_Write_Data(rgb565>>8);    // д����
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

__ramfunc void LCD_Img_Gray(Point_t site, Size_t size, uint8_t *img)
{
    uint32_t     total = (size.H * size.W);
    uint16_t     imgtemp;
    uint8_t     *pimg = (uint8_t *)img;
    
    LCD_PTLON(site, size);                      //����
    LCD_Write_CMD(0x2C);                       //д�ڴ�
    
    while(total--)
    {
        imgtemp     = (uint16_t) * (pimg++);
        imgtemp = GRAY_2_RGB565(imgtemp);
        LCD_Write_Data(imgtemp>>8);              //дͼ������
        LCD_Write_Data(imgtemp&0x00FF);
    }
}

__ramfunc void LCD_Img_Gray_ZOOM(Point_t site, Size_t size, uint8_t *img, Size_t imgsize)
{

    uint32_t temp, tempY;
    uint16_t x, y;
    uint16_t X, Y;
    uint8_t *pimg = (uint8_t *)img;
    uint16_t rgb;

    LCD_PTLON(site, size);                      //����
    LCD_Write_CMD(0x2C);                       //д�ڴ�
    
    for(y = 0; y < size.H; y++)
    {
        Y = ( (  y * imgsize.H   ) / size.H) ;
        tempY = Y * imgsize.W ;

        for(x = 0; x < size.W; x++)
        {
            X = ( x * imgsize.W  ) / size.W ;
            temp = tempY + X;
            rgb = GRAY_2_RGB565(pimg[temp]);
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

    LCD_Write_CMD(0x2C);                       //д�ڴ�

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
            //����
            site.x = 0;
            site.y += 16;
        }
        if(site.y > MAX_CHAR_POSY )
        {
            //һ��
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
 *��ʾ����  by RUS
 *value Ҫ��ʾ��ֵ
 *places λ��
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
 *��ʾ��λС�� by RUS
 *value Ҫ��ʾ��ֵ
 *places ����λ��
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