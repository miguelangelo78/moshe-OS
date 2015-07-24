#pragma once

#include "defs\video.h"
#include "stdarg.h"
#include "stdlib.h"
#include "string\string.h"

extern void d_clrscr();
extern void d_clrscr_rst(bool reset_cursor);
extern void d_clrscr(const unsigned short color);
extern void d_clrscr(const unsigned short color, bool reset_cursor);
extern void d_putc(uint8_t c, uint8_t color);
extern void d_putc(uint8_t c);
extern void d_puts(char* str);
extern int d_printf(const char* str, ...);
extern unsigned d_setcolor(const unsigned c);
extern void d_gotoxy(unsigned x, unsigned y);
extern uint8_t get_color(uint8_t back, uint8_t front);
