#include "include.h"

#define _3wirespi

#define Max_Column	0x7f			// 128-1
#define Max_Row		0x7f			// 128-1
#define	Brightness	0x0F

void OLED_WriteByte(OLEDn *OLED, uint8 cmd, uint8 byte) //Write_Command/Write_Data
{
    #ifdef _3wirespi
    #elif defined(_4wirespi)
    #endif
    
    OLED_SCK_L;
    asm("nop");
    
    if(cmd)
        OLED_SDA_H;
    else
        OLED_SDA_L;
    asm("nop");
    
    OLED_SCK_H;
    asm("nop");

    for (uint8 i=0; i<8; i++)
    {
        OLED_SCK_L;
        asm("nop");
        
        if(byte&0x80)
          OLED_SDA_H;
	else 
          OLED_SDA_L;
        asm("nop");
        
        byte<<=1;
        
	OLED_SCK_H;
        asm("nop");
    }
    
}

//-=-=-=-=-=-=-=-=-=-=-=-=//
//  Instruction Setting   //
//-=-=-=-=-=-=-=-=-=-=-=-=//
void Set_Column_Address(OLEDn *OLED, uint8 start, uint8 end)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0x15);			// Set Column Address
	OLED_WriteByte(OLED, OLED_DATA, start);			// Default => 0x00 (Start Address)
	OLED_WriteByte(OLED, OLED_DATA, end);			// Default => 0x7F (End Address)
}


void Set_Row_Address(OLEDn *OLED, uint8 start, uint8 end)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0x75);			// Set Row Address
	OLED_WriteByte(OLED, OLED_DATA, start);			// Default => 0x00 (Start Address)
	OLED_WriteByte(OLED, OLED_DATA, end);			// Default => 0x7F (End Address)
}


void Set_Write_RAM(OLEDn *OLED)
{
	OLED_WriteByte(OLED, OLED_CMD, 0x5C);			// Enable MCU to Write into RAM
}


void Set_Read_RAM(OLEDn *OLED)
{
	OLED_WriteByte(OLED, OLED_CMD, 0x5D);			// Enable MCU to Read from RAM
}


void Set_Remap_Format(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xA0);			// Set Re-Map / Color Depth
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x40
						                            // Horizontal Address Increment
						                            // Column Address 0 Mapped to SEG0
						                            // Color Sequence: A => B => C
						                            // Scan from COM0 to COM[N-1]
						                            // Disable COM Split Odd Even
						                            // 65,536 Colors
}


void Set_Start_Line(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xA1);			// Set Vertical Scroll by RAM
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x00
}


void Set_Display_Offset(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xA2);			// Set Vertical Scroll by Row
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x60
}


void Set_Display_Mode(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED, OLED_CMD, 0xA4|data);		// Set Display Mode
						                            // Default => 0xA6
						                            // 0xA4 (0x00) => Entire Display Off, All Pixels Turn Off
						                            // 0xA5 (0x01) => Entire Display On, All Pixels Turn On at GS Level 63
						                            // 0xA6 (0x02) => Normal Display
						                            // 0xA7 (0x03) => Inverse Display
}


void Set_Function_Selection(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xAB);			// Function Selection
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x01
						                            // Enable Internal VDD Regulator
						                            // Select 8-bit Parallel Interface
}


void Set_Display_On_Off(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED, OLED_CMD, 0xAE|data);		// Set Display On/Off
						                            // Default => 0xAE
						                            // 0xAE (0x00) => Display Off (Sleep Mode On)
						                            // 0xAF (0x01) => Display On (Sleep Mode Off)
}


void Set_Phase_Length(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xB1);			// Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x82 (8 Display Clocks [Phase 2] / 5 Display Clocks [Phase 1])
						                            // D[3:0] => Phase 1 Period in 5~31 Display Clocks
						                            // D[7:4] => Phase 2 Period in 3~15 Display Clocks
}


void Set_Display_Enhancement(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xB2);			// Display Enhancement
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x00 (Normal)
	OLED_WriteByte(OLED, OLED_DATA, 0x00);
	OLED_WriteByte(OLED, OLED_DATA, 0x00);
}


void Set_Display_Clock(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xB3);			// Set Display Clock Divider / Oscillator Frequency
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x00
						                            // A[3:0] => Display Clock Divider
						                            // A[7:4] => Oscillator Frequency
}


void Set_VSL(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xB4);			// Set Segment Low Voltage
	OLED_WriteByte(OLED, OLED_DATA, 0xA0|data);		// Default => 0xA0
						                            // 0xA0 (0x00) => Enable External VSL
						                            // 0xA2 (0x02) => Enable Internal VSL (Kept VSL Pin N.C.)
	OLED_WriteByte(OLED, OLED_DATA, 0xB5);
	OLED_WriteByte(OLED, OLED_DATA, 0x55);
}


void Set_GPIO(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xB5);			// General Purpose IO
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x0A (GPIO Pins output Low Level.)
}


void Set_Precharge_Period(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xB6);			// Set Second Pre-Charge Period
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x08 (8 Display Clocks)
}


void Set_Precharge_Voltage(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xBB);			// Set Pre-Charge Voltage Level
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x17 (0.50*VCC)
}


void Set_VCOMH(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xBE);			// Set COM Deselect Voltage Level
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x05 (0.82*VCC)
}


void Set_Contrast_Color(OLEDn *OLED, uint8 a, uint8 b, uint8 c)
{
	OLED_WriteByte(OLED, OLED_CMD, 0xC1);			// Set Contrast Current for Color A, B, C
	OLED_WriteByte(OLED, OLED_DATA, a);				// Default => 0x8A (Color A)
	OLED_WriteByte(OLED, OLED_DATA, b);				// Default => 0x51 (Color B)
	OLED_WriteByte(OLED, OLED_DATA, c);				// Default => 0x8A (Color C)
}


void Set_Master_Current(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xC7);			// Master Contrast Current Control
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x0F (Maximum)
}


void Set_Multiplex_Ratio(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xCA);			// Set Multiplex Ratio
	OLED_WriteByte(OLED, OLED_DATA, data);			// Default => 0x7F (1/128 Duty)
}


void Set_Command_Lock(OLEDn *OLED, uint8 data)
{
	OLED_WriteByte(OLED, 0u, 0xFD);			// Set Command Lock
	OLED_WriteByte(OLED, 1u, data);			// Default => 0x12
						                            // 0x12 => Driver IC interface is unlocked from entering command.
						                            // 0x16 => All Commands are locked except 0xFD.
						                            // 0xB0 => Command 0xA2, 0xB1, 0xB3, 0xBB & 0xBE are inaccessible.
						                            // 0xB1 => All Commands are accessible.
}

//-=-=-=-=-=-=-=-=-=-=-=-=//
//    Drawing Functions   //
//-=-=-=-=-=-=-=-=-=-=-=-=//

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//
//    a: RRRRRGGG
//    b: GGGBBBBB
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM(OLEDn *OLED, uint8 a, uint8 b)
{
    uint8 i=0,j=0;

	Set_Column_Address(OLED, 0x00,0x7F);
	Set_Row_Address(OLED, 0x00,0x7F);
	Set_Write_RAM(OLED);

	for(i=0;i<128;i++)
	{
            for(j=0;j<128;j++)
            {
		OLED_WriteByte(OLED, OLED_DATA, a);
		OLED_WriteByte(OLED, OLED_DATA, b);
            }
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Character (5x7)
//
//    Character : ASCII
//    MSB_Format: Data Format (MSB)
//                D[15:8]=[RRRRR:GGG] (65,536 Colors Mode)
//    LSB_Format: Data Format (LSB)
//                D[07:0]=[GGG:BBBBB] (65,536 Colors Mode)
//    X: Start X Address
//    Y: Start Y Address
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Font57(OLEDn *OLED, uint8 Character, uint8 MSB_Format, uint8 LSB_Format, uint8 X, uint8 Y)
{
    uint8 i,Font,Pick;
    uint8 *Src_Pointer = &ASCII_Eng[(Character-' ')][0];

	Set_Remap_Format(OLED, 0x75);

	for(i=0;i<=5;i++)
	{
		Pick=*Src_Pointer;
		Set_Column_Address(OLED, X+i,X+i);
		Set_Row_Address(OLED,Y,Y+7);
		Set_Write_RAM(OLED);

		Font=Pick&0x01;
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5)|(Font<<6)|(Font<<7);
		OLED_WriteByte(OLED, OLED_DATA, MSB_Format&Font);
		OLED_WriteByte(OLED, OLED_DATA, LSB_Format&Font);

		Font=(Pick&0x02)>>1;
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5)|(Font<<6)|(Font<<7);
		OLED_WriteByte(OLED, OLED_DATA, MSB_Format&Font);
		OLED_WriteByte(OLED, OLED_DATA, LSB_Format&Font);

		Font=(Pick&0x04)>>2;
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5)|(Font<<6)|(Font<<7);
		OLED_WriteByte(OLED, OLED_DATA, MSB_Format&Font);
		OLED_WriteByte(OLED, OLED_DATA, LSB_Format&Font);

		Font=(Pick&0x08)>>3;
		Font=Font|(Font<<1)|(Font<<2)|(Font<<3)|(Font<<4)|(Font<<5)|(Font<<6)|(Font<<7);
		OLED_WriteByte(OLED, OLED_DATA, MSB_Format&Font);
		OLED_WriteByte(OLED, OLED_DATA, LSB_Format&Font);

		Font=(Pick&0x10)<<3;
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5)|(Font>>6)|(Font>>7);
		OLED_WriteByte(OLED, OLED_DATA, MSB_Format&Font);
		OLED_WriteByte(OLED, OLED_DATA, LSB_Format&Font);

		Font=(Pick&0x20)<<2;
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5)|(Font>>6)|(Font>>7);
		OLED_WriteByte(OLED, OLED_DATA, MSB_Format&Font);
		OLED_WriteByte(OLED, OLED_DATA, LSB_Format&Font);

		Font=(Pick&0x40)<<1;
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5)|(Font>>6)|(Font>>7);
		OLED_WriteByte(OLED, OLED_DATA, MSB_Format&Font);
		OLED_WriteByte(OLED, OLED_DATA, LSB_Format&Font);

		Font=Pick&0x80;
		Font=Font|(Font>>1)|(Font>>2)|(Font>>3)|(Font>>4)|(Font>>5)|(Font>>6)|(Font>>7);
		OLED_WriteByte(OLED, OLED_DATA, MSB_Format&Font);
		OLED_WriteByte(OLED, OLED_DATA, LSB_Format&Font);
		Src_Pointer++;
	}
	Set_Column_Address(OLED, X+5,X+5);
	Set_Row_Address(OLED, Y,Y+7);	
	Set_Write_RAM(OLED);

	for(i=0;i<=8;i++)
	{
		OLED_WriteByte(OLED, OLED_DATA, 0x00);
		OLED_WriteByte(OLED, OLED_DATA, 0x00);
	}
	Set_Remap_Format(OLED, 0x74);
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show String
//
//    MSB_Format: Data Format (MSB)
//                D[15:8]=[RRRRR:GGG] (65,536 Colors Mode)
//    LSB_Format: Data Format (LSB)
//       D[7:0]=[GGG:BBBBB] (65,536 Colors Mode)
//    X: Start X Address
//    Y: Start Y Address
//    * Must write "0" in the end...
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_String(OLEDn *OLED, uint8 *Data_Pointer, uint8 MSB_Format, uint8 LSB_Format, uint8 X, uint8 Y)
{
    uint8 *Src_Pointer;

	Src_Pointer=Data_Pointer;
	Show_Font57(OLED, 10, MSB_Format,LSB_Format,X,Y);		// No-Break Space
						                                    //   Must be written first before the string start...

	while(1)
	{
            Show_Font57(OLED, *Src_Pointer,MSB_Format,LSB_Format,X,Y);
            X+=6;
            Src_Pointer++;
            if(*Src_Pointer == 0)
            {
                break;
            }
	}
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Gray_Scale_Table(OLEDn *OLED)
{
	OLED_WriteByte(OLED,  OLED_CMD, 0xB8);
	OLED_WriteByte(OLED, OLED_DATA, 0x02);			// Gray Scale Level 1
	OLED_WriteByte(OLED, OLED_DATA, 0x03);			// Gray Scale Level 2
	OLED_WriteByte(OLED, OLED_DATA, 0x04);			// Gray Scale Level 3
	OLED_WriteByte(OLED, OLED_DATA, 0x05);			// Gray Scale Level 4
	OLED_WriteByte(OLED, OLED_DATA, 0x06);			// Gray Scale Level 5
	OLED_WriteByte(OLED, OLED_DATA, 0x07);			// Gray Scale Level 6
	OLED_WriteByte(OLED, OLED_DATA, 0x08);			// Gray Scale Level 7
	OLED_WriteByte(OLED, OLED_DATA, 0x09);			// Gray Scale Level 8
	OLED_WriteByte(OLED, OLED_DATA, 0x0A);			// Gray Scale Level 9
	OLED_WriteByte(OLED, OLED_DATA, 0x0B);			// Gray Scale Level 10
	OLED_WriteByte(OLED, OLED_DATA, 0x0C);			// Gray Scale Level 11
	OLED_WriteByte(OLED, OLED_DATA, 0x0D);			// Gray Scale Level 12
	OLED_WriteByte(OLED, OLED_DATA, 0x0E);			// Gray Scale Level 13
	OLED_WriteByte(OLED, OLED_DATA, 0x0F);			// Gray Scale Level 14
	OLED_WriteByte(OLED, OLED_DATA, 0x10);			// Gray Scale Level 15
	OLED_WriteByte(OLED, OLED_DATA, 0x11);			// Gray Scale Level 16
	OLED_WriteByte(OLED, OLED_DATA, 0x12);			// Gray Scale Level 17
	OLED_WriteByte(OLED, OLED_DATA, 0x13);			// Gray Scale Level 18
	OLED_WriteByte(OLED, OLED_DATA, 0x15);			// Gray Scale Level 19
	OLED_WriteByte(OLED, OLED_DATA, 0x17);			// Gray Scale Level 20
	OLED_WriteByte(OLED, OLED_DATA, 0x19);			// Gray Scale Level 21
	OLED_WriteByte(OLED, OLED_DATA, 0x1B);			// Gray Scale Level 22
	OLED_WriteByte(OLED, OLED_DATA, 0x1D);			// Gray Scale Level 23
	OLED_WriteByte(OLED, OLED_DATA, 0x1F);			// Gray Scale Level 24
	OLED_WriteByte(OLED, OLED_DATA, 0x21);			// Gray Scale Level 25
	OLED_WriteByte(OLED, OLED_DATA, 0x23);			// Gray Scale Level 26
	OLED_WriteByte(OLED, OLED_DATA, 0x25);			// Gray Scale Level 27
	OLED_WriteByte(OLED, OLED_DATA, 0x27);			// Gray Scale Level 28
	OLED_WriteByte(OLED, OLED_DATA, 0x2A);			// Gray Scale Level 29
	OLED_WriteByte(OLED, OLED_DATA, 0x2D);			// Gray Scale Level 30
	OLED_WriteByte(OLED, OLED_DATA, 0x30);			// Gray Scale Level 31
	OLED_WriteByte(OLED, OLED_DATA, 0x33);			// Gray Scale Level 32
	OLED_WriteByte(OLED, OLED_DATA, 0x36);			// Gray Scale Level 33
	OLED_WriteByte(OLED, OLED_DATA, 0x39);			// Gray Scale Level 34
	OLED_WriteByte(OLED, OLED_DATA, 0x3C);			// Gray Scale Level 35
	OLED_WriteByte(OLED, OLED_DATA, 0x3F);			// Gray Scale Level 36
	OLED_WriteByte(OLED, OLED_DATA, 0x42);			// Gray Scale Level 37
	OLED_WriteByte(OLED, OLED_DATA, 0x45);			// Gray Scale Level 38
	OLED_WriteByte(OLED, OLED_DATA, 0x48);			// Gray Scale Level 39
	OLED_WriteByte(OLED, OLED_DATA, 0x4C);			// Gray Scale Level 40
	OLED_WriteByte(OLED, OLED_DATA, 0x50);			// Gray Scale Level 41
	OLED_WriteByte(OLED, OLED_DATA, 0x54);			// Gray Scale Level 42
	OLED_WriteByte(OLED, OLED_DATA, 0x58);			// Gray Scale Level 43
	OLED_WriteByte(OLED, OLED_DATA, 0x5C);			// Gray Scale Level 44
	OLED_WriteByte(OLED, OLED_DATA, 0x60);			// Gray Scale Level 45
	OLED_WriteByte(OLED, OLED_DATA, 0x64);			// Gray Scale Level 46
	OLED_WriteByte(OLED, OLED_DATA, 0x68);			// Gray Scale Level 47
	OLED_WriteByte(OLED, OLED_DATA, 0x6C);			// Gray Scale Level 48
	OLED_WriteByte(OLED, OLED_DATA, 0x70);			// Gray Scale Level 49
	OLED_WriteByte(OLED, OLED_DATA, 0x74);			// Gray Scale Level 50
	OLED_WriteByte(OLED, OLED_DATA, 0x78);			// Gray Scale Level 51
	OLED_WriteByte(OLED, OLED_DATA, 0x7D);			// Gray Scale Level 52
	OLED_WriteByte(OLED, OLED_DATA, 0x82);			// Gray Scale Level 53
	OLED_WriteByte(OLED, OLED_DATA, 0x87);			// Gray Scale Level 54
	OLED_WriteByte(OLED, OLED_DATA, 0x8C);			// Gray Scale Level 55
	OLED_WriteByte(OLED, OLED_DATA, 0x91);			// Gray Scale Level 56
	OLED_WriteByte(OLED, OLED_DATA, 0x96);			// Gray Scale Level 57
	OLED_WriteByte(OLED, OLED_DATA, 0x9B);			// Gray Scale Level 58
	OLED_WriteByte(OLED, OLED_DATA, 0xA0);			// Gray Scale Level 59
	OLED_WriteByte(OLED, OLED_DATA, 0xA5);			// Gray Scale Level 60
	OLED_WriteByte(OLED, OLED_DATA, 0xAA);			// Gray Scale Level 61
	OLED_WriteByte(OLED, OLED_DATA, 0xAF);			// Gray Scale Level 62
	OLED_WriteByte(OLED, OLED_DATA, 0xB4);			// Gray Scale Level 63
}

//-=-=-=-=-=-=-=-=-=-=-=-=//
//    Initializing OLED   //
//-=-=-=-=-=-=-=-=-=-=-=-=//

void OLED_Init(OLEDn *OLED)
{
        Set_Command_Lock(OLED, 0x12);			// Unlock Driver IC (0x12/0x16/0xB0/0xB1)
	Set_Command_Lock(OLED, 0xB1);			// Unlock All Commands (0x12/0x16/0xB0/0xB1)
	Set_Display_On_Off(OLED, 0x00);		// Display Off (0x00/0x01)
	Set_Display_Clock(OLED, 0xF1);		// Set Clock as 90 Frames/Sec
	Set_Multiplex_Ratio(OLED, 0x7F);		// 1/128 Duty (0x0F~0x7F)
	Set_Display_Offset(OLED, 0x00);		// Shift Mapping RAM Counter (0x00~0x7F)
	Set_Start_Line(OLED, 0x00);			// Set Mapping RAM Display Start Line (0x00~0x7F)
	Set_Remap_Format(OLED, 0x74);			// Set Horizontal Address Increment
						//     Column Address 0 Mapped to SEG0
						//     Color Sequence D[15:0]=[RRRRR:GGGGGG:BBBBB]
						//     Scan from COM127 to COM0
						//     Enable COM Split Odd Even
						//     65,536 Colors Mode (0x74)
						//     * 262,144 Colors Mode (0xB4)
	Set_GPIO(OLED, 0x00);				// Disable GPIO Pins Input
	Set_Function_Selection(OLED, 0x01);		// Enable Internal VDD Regulator
						// Select 8-bit Parallel Interface
	Set_VSL(OLED, 0xA0);				// Enable External VSL
	Set_Contrast_Color(OLED, 0xC8,0x80,0xC8);	// Set Contrast of Color A (Red)
						// Set Contrast of Color B (Green)
						// Set Contrast of Color C (Blue)
	Set_Master_Current(OLED, Brightness);		// Set Scale Factor of Segment Output Current Control
	Set_Gray_Scale_Table(OLED);			// Set Pulse Width for Gray Scale Table
	Set_Phase_Length(OLED, 0x32);			// Set Phase 1 as 5 Clocks & Phase 2 as 3 Clocks
	Set_Precharge_Voltage(OLED, 0x17);		// Set Pre-Charge Voltage Level as 0.50*VCC
	Set_Display_Enhancement(OLED, 0xA4);		// Enhance Display Performance
	Set_Precharge_Period(OLED, 0x01);		// Set Second Pre-Charge Period as 1 Clock
	Set_VCOMH(OLED, 0x05);			// Set Common Pins Deselect Voltage Level as 0.82*VCC
	Set_Display_Mode(OLED, 0x02);			// Normal Display Mode (0x00/0x01/0x02/0x03)

	Fill_RAM(OLED, 0x00,0x00);			// Clear Screen

	Set_Display_On_Off(OLED, 0x01);		// Display On (0x00/0x01)
}

//void OLED_SCK_H(GPIO_Type *ptx, uint8 SCK)
//{
//    ptx->PDOR = (ptx->PDOR & ~(0x01L<<SCK)) | (uint32)1<<SCK;
//}
//void OLED_SCK_L(GPIO_Type *ptx, uint8 SCK)
//{
//    ptx->PDOR = (ptx->PDOR & ~(0x01L<<SCK)) | (uint32)0<<SCK;
//}
//void OLED_SDA_H(GPIO_Type *ptx, uint8 SDA)
//{
//    ptx->PDOR = (ptx->PDOR & ~(0x01L<<SDA)) | (uint32)1<<SDA;
//}
//void OLED_SDA_L(GPIO_Type *ptx, uint8 SDA)
//{
//    ptx->PDOR = (ptx->PDOR & ~(0x01L<<SDA)) | (uint32)0<<SDA;
//}
unsigned char ASCII_Eng[95][5]={		         // Refer to "Times New Roman" Font Database...
	{0x00,0x00,0x00,0x00,0x00},		//
	{0x00,0x00,0x4F,0x00,0x00},		//   (  1)  ! - 0x0021 Exclamation Mark
	{0x00,0x07,0x00,0x07,0x00},		//   (  2)  " - 0x0022 Quotation Mark
	{0x14,0x7F,0x14,0x7F,0x14},		//   (  3)  # - 0x0023 Number Sign
	{0x24,0x2A,0x7F,0x2A,0x12},		//   (  4)  $ - 0x0024 Dollar Sign
	{0x23,0x13,0x08,0x64,0x62},		//   (  5)  % - 0x0025 Percent Sign
	{0x36,0x49,0x55,0x22,0x50},		//   (  6)  & - 0x0026 Ampersand
	{0x00,0x05,0x03,0x00,0x00},		//   (  7)  ' - 0x0027 Apostrophe
	{0x00,0x1C,0x22,0x41,0x00},		//   (  8)  ( - 0x0028 Left Parenthesis
	{0x00,0x41,0x22,0x1C,0x00},		//   (  9)  ) - 0x0029 Right Parenthesis
	{0x14,0x08,0x3E,0x08,0x14},		//   ( 10)  * - 0x002A Asterisk
	{0x08,0x08,0x3E,0x08,0x08},		//   ( 11)  + - 0x002B Plus Sign
	{0x00,0x50,0x30,0x00,0x00},		//   ( 12)  , - 0x002C Comma
	{0x08,0x08,0x08,0x08,0x08},		//   ( 13)  - - 0x002D Hyphen-Minus
	{0x00,0x60,0x60,0x00,0x00},		//   ( 14)  . - 0x002E Full Stop
	{0x20,0x10,0x08,0x04,0x02},		//   ( 15)  / - 0x002F Solidus
	{0x3E,0x51,0x49,0x45,0x3E},		//   ( 16)  0 - 0x0030 Digit Zero
	{0x00,0x42,0x7F,0x40,0x00},		//   ( 17)  1 - 0x0031 Digit One
	{0x42,0x61,0x51,0x49,0x46},		//   ( 18)  2 - 0x0032 Digit Two
	{0x21,0x41,0x45,0x4B,0x31},		//   ( 19)  3 - 0x0033 Digit Three
	{0x18,0x14,0x12,0x7F,0x10},		//   ( 20)  4 - 0x0034 Digit Four
	{0x27,0x45,0x45,0x45,0x39},		//   ( 21)  5 - 0x0035 Digit Five
	{0x3C,0x4A,0x49,0x49,0x30},		//   ( 22)  6 - 0x0036 Digit Six
	{0x01,0x71,0x09,0x05,0x03},		//   ( 23)  7 - 0x0037 Digit Seven
	{0x36,0x49,0x49,0x49,0x36},		//   ( 24)  8 - 0x0038 Digit Eight
	{0x06,0x49,0x49,0x29,0x1E},		//   ( 25)  9 - 0x0039 Dight Nine
	{0x00,0x36,0x36,0x00,0x00},		//   ( 26)  : - 0x003A Colon
	{0x00,0x56,0x36,0x00,0x00},		//   ( 27)  ; - 0x003B Semicolon
	{0x08,0x14,0x22,0x41,0x00},		//   ( 28)  < - 0x003C Less-Than Sign
	{0x14,0x14,0x14,0x14,0x14},		//   ( 29)  = - 0x003D Equals Sign
	{0x00,0x41,0x22,0x14,0x08},		//   ( 30)  > - 0x003E Greater-Than Sign
	{0x02,0x01,0x51,0x09,0x06},		//   ( 31)  ? - 0x003F Question Mark
	{0x32,0x49,0x79,0x41,0x3E},		//   ( 32)  @ - 0x0040 Commercial At
	{0x7E,0x11,0x11,0x11,0x7E},		//   ( 33)  A - 0x0041 Latin Capital Letter A
	{0x7F,0x49,0x49,0x49,0x36},		//   ( 34)  B - 0x0042 Latin Capital Letter B
	{0x3E,0x41,0x41,0x41,0x22},		//   ( 35)  C - 0x0043 Latin Capital Letter C
	{0x7F,0x41,0x41,0x22,0x1C},		//   ( 36)  D - 0x0044 Latin Capital Letter D
	{0x7F,0x49,0x49,0x49,0x41},		//   ( 37)  E - 0x0045 Latin Capital Letter E
	{0x7F,0x09,0x09,0x09,0x01},		//   ( 38)  F - 0x0046 Latin Capital Letter F
	{0x3E,0x41,0x49,0x49,0x7A},		//   ( 39)  G - 0x0047 Latin Capital Letter G
	{0x7F,0x08,0x08,0x08,0x7F},		//   ( 40)  H - 0x0048 Latin Capital Letter H
	{0x00,0x41,0x7F,0x41,0x00},		//   ( 41)  I - 0x0049 Latin Capital Letter I
	{0x20,0x40,0x41,0x3F,0x01},		//   ( 42)  J - 0x004A Latin Capital Letter J
	{0x7F,0x08,0x14,0x22,0x41},		//   ( 43)  K - 0x004B Latin Capital Letter K
	{0x7F,0x40,0x40,0x40,0x40},		//   ( 44)  L - 0x004C Latin Capital Letter L
	{0x7F,0x02,0x0C,0x02,0x7F},		//   ( 45)  M - 0x004D Latin Capital Letter M
	{0x7F,0x04,0x08,0x10,0x7F},		//   ( 46)  N - 0x004E Latin Capital Letter N
	{0x3E,0x41,0x41,0x41,0x3E},		//   ( 47)  O - 0x004F Latin Capital Letter O
	{0x7F,0x09,0x09,0x09,0x06},		//   ( 48)  P - 0x0050 Latin Capital Letter P
	{0x3E,0x41,0x51,0x21,0x5E},		//   ( 49)  Q - 0x0051 Latin Capital Letter Q
	{0x7F,0x09,0x19,0x29,0x46},		//   ( 50)  R - 0x0052 Latin Capital Letter R
	{0x46,0x49,0x49,0x49,0x31},		//   ( 51)  S - 0x0053 Latin Capital Letter S
	{0x01,0x01,0x7F,0x01,0x01},		//   ( 52)  T - 0x0054 Latin Capital Letter T
	{0x3F,0x40,0x40,0x40,0x3F},		//   ( 53)  U - 0x0055 Latin Capital Letter U
	{0x1F,0x20,0x40,0x20,0x1F},		//   ( 54)  V - 0x0056 Latin Capital Letter V
	{0x3F,0x40,0x38,0x40,0x3F},		//   ( 55)  W - 0x0057 Latin Capital Letter W
	{0x63,0x14,0x08,0x14,0x63},		//   ( 56)  X - 0x0058 Latin Capital Letter X
	{0x07,0x08,0x70,0x08,0x07},		//   ( 57)  Y - 0x0059 Latin Capital Letter Y
	{0x61,0x51,0x49,0x45,0x43},		//   ( 58)  Z - 0x005A Latin Capital Letter Z
	{0x00,0x7F,0x41,0x41,0x00},		//   ( 59)  [ - 0x005B Left Square Bracket
	{0x02,0x04,0x08,0x10,0x20},		//   ( 60)  \ - 0x005C Reverse Solidus
	{0x00,0x41,0x41,0x7F,0x00},		//   ( 61)  ] - 0x005D Right Square Bracket
	{0x04,0x02,0x01,0x02,0x04},		//   ( 62)  ^ - 0x005E Circumflex Accent
	{0x40,0x40,0x40,0x40,0x40},		//   ( 63)  _ - 0x005F Low Line
	{0x01,0x02,0x04,0x00,0x00},		//   ( 64)  ` - 0x0060 Grave Accent
	{0x20,0x54,0x54,0x54,0x78},		//   ( 65)  a - 0x0061 Latin Small Letter A
	{0x7F,0x48,0x44,0x44,0x38},		//   ( 66)  b - 0x0062 Latin Small Letter B
	{0x38,0x44,0x44,0x44,0x20},		//   ( 67)  c - 0x0063 Latin Small Letter C
	{0x38,0x44,0x44,0x48,0x7F},		//   ( 68)  d - 0x0064 Latin Small Letter D
	{0x38,0x54,0x54,0x54,0x18},		//   ( 69)  e - 0x0065 Latin Small Letter E
	{0x08,0x7E,0x09,0x01,0x02},		//   ( 70)  f - 0x0066 Latin Small Letter F
	{0x06,0x49,0x49,0x49,0x3F},		//   ( 71)  g - 0x0067 Latin Small Letter G
	{0x7F,0x08,0x04,0x04,0x78},		//   ( 72)  h - 0x0068 Latin Small Letter H
	{0x00,0x44,0x7D,0x40,0x00},		//   ( 73)  i - 0x0069 Latin Small Letter I
	{0x20,0x40,0x44,0x3D,0x00},		//   ( 74)  j - 0x006A Latin Small Letter J
	{0x7F,0x10,0x28,0x44,0x00},		//   ( 75)  k - 0x006B Latin Small Letter K
	{0x00,0x41,0x7F,0x40,0x00},		//   ( 76)  l - 0x006C Latin Small Letter L
	{0x7C,0x04,0x18,0x04,0x7C},		//   ( 77)  m - 0x006D Latin Small Letter M
	{0x7C,0x08,0x04,0x04,0x78},		//   ( 78)  n - 0x006E Latin Small Letter N
	{0x38,0x44,0x44,0x44,0x38},		//   ( 79)  o - 0x006F Latin Small Letter O
	{0x7C,0x14,0x14,0x14,0x08},		//   ( 80)  p - 0x0070 Latin Small Letter P
	{0x08,0x14,0x14,0x18,0x7C},		//   ( 81)  q - 0x0071 Latin Small Letter Q
	{0x7C,0x08,0x04,0x04,0x08},		//   ( 82)  r - 0x0072 Latin Small Letter R
	{0x48,0x54,0x54,0x54,0x20},		//   ( 83)  s - 0x0073 Latin Small Letter S
	{0x04,0x3F,0x44,0x40,0x20},		//   ( 84)  t - 0x0074 Latin Small Letter T
	{0x3C,0x40,0x40,0x20,0x7C},		//   ( 85)  u - 0x0075 Latin Small Letter U
	{0x1C,0x20,0x40,0x20,0x1C},		//   ( 86)  v - 0x0076 Latin Small Letter V
	{0x3C,0x40,0x30,0x40,0x3C},		//   ( 87)  w - 0x0077 Latin Small Letter W
	{0x44,0x28,0x10,0x28,0x44},		//   ( 88)  x - 0x0078 Latin Small Letter X
	{0x0C,0x50,0x50,0x50,0x3C},		//   ( 89)  y - 0x0079 Latin Small Letter Y
	{0x44,0x64,0x54,0x4C,0x44},		//   ( 90)  z - 0x007A Latin Small Letter Z
	{0x00,0x08,0x36,0x41,0x00},		//   ( 91)  { - 0x007B Left Curly Bracket
	{0x00,0x00,0x7F,0x00,0x00},		//   ( 92)  | - 0x007C Vertical Line
	{0x00,0x41,0x36,0x08,0x00},		//   ( 93)  } - 0x007D Right Curly Bracket
	{0x02,0x01,0x02,0x04,0x02},		//   ( 94)  ~ - 0x007E Tilde
};