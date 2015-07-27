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
extern UPoint d_gotoxy(unsigned x, unsigned y);
extern uint8_t get_color(uint8_t back, uint8_t front);

extern void d_scroll(bool force);
extern void d_scroll(bool force, uint16_t amount);

extern void d_term_lmargin(uint32_t margin);
extern void d_term_rmargin(uint32_t margin);
extern UPoint d_getCursor();