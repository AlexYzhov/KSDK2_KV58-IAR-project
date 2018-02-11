#ifndef _PART_TERMINAL_
#define _PART_TERMINAL_

#include "PART_ASCII.h"
#include "ALEX_LCD_ST7735.h"

typedef int (*PUTCHAR_FUNC)(int a);

typedef struct
{
    uint8_t Row;   /* 0 <= Row <=16 */
    uint8_t Col;   /* 0 <= Col <=7 */
    
    Point_t XY;
}TCursor_t;

extern int DbgConsole_PrintfFormattedData(PUTCHAR_FUNC func_ptr, const char *fmt, va_list ap);
int T_Printf(const char *fmt_s, ...);
int T_Putchar(int ch);

#endif