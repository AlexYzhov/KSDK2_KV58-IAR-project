#include "include.h"
#include "fsl_debug_console.h"

TCursor_t T_Cursor =
{
    .Row = 0,
    .Col = 0,
    .XY.x = 0,
    .XY.y = 0,
};

int T_Printf(const char *fmt_s, ...)
{
    T_Putchar('>');
    T_Putchar('>');
    
    va_list ap;
    int result;

    va_start(ap, fmt_s);
    result = DbgConsole_PrintfFormattedData(T_Putchar, fmt_s, ap);
    va_end(ap);
    
    T_Cursor.Col = 0;
    T_Cursor.XY.x = T_Cursor.Col*ASCII_WIDTH;
    T_Cursor.Row += 1;
    T_Cursor.XY.y = T_Cursor.Row*ASCII_HEIGHT;

    return result;
}

int T_Putchar(int ch)
{
    if(T_Cursor.XY.x>LCD_W-ASCII_WIDTH)           // ����һ��(�����곬��128-8)
    {
        T_Cursor.Col = 0;
        T_Cursor.XY.x = T_Cursor.Col*ASCII_WIDTH;
        
        T_Cursor.Row += 1;
        T_Cursor.XY.y = T_Cursor.Row*ASCII_HEIGHT;
    }
    else if(T_Cursor.XY.y>LCD_H-2*ASCII_HEIGHT)   // ����һҳ(�����곬��128-16�����������һ��)
    {
        T_Cursor.Col = 0;
        T_Cursor.XY.x = T_Cursor.Col*ASCII_WIDTH;
        
        T_Cursor.Row = 0;
        T_Cursor.XY.y = T_Cursor.Row*ASCII_HEIGHT;
    }
    
    LCD_Char(T_Cursor.XY, ch, GRAY, BLACK);
    
    T_Cursor.Col += 1;
    T_Cursor.XY.x = T_Cursor.Col*ASCII_WIDTH;
    
    return 1;
}