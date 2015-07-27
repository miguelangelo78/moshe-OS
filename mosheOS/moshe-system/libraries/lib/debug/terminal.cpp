#include "debug\terminal.h"
#include "type\types.h"
#include "hal\hal.h"

/*TEMPORARY KEYBOARD CONSTANTS*/
#define KEY_BACKSPACE 0x08
#define KEY_TAB 0x09
#define KEY_NEWLINE '\n'
#define KEY_CARRIAGE '\r'
#define PRINTABLE_CHAR(c) c>=' '

// Cursor variables:
UPoint console_cursor = { 0,0 };
UPoint console_size = { VID_WIDTH, VID_HEIGHT };
uint8_t * vid_ptr = (uint8_t*)VID_MEMORY;

uint32_t term_lmargin = 0; // In case the user wants to print without X returning to 0
uint32_t term_rmargin = VID_WIDTH; // In case the user wants to limit the length of a string and wrap it, 0 in this case is the actual desired margin

void d_term_lmargin(uint32_t margin) {
	if(margin <= console_size.X)
		term_lmargin = margin;
}

void d_term_rmargin(uint32_t margin) {
	if(margin <= console_size.X)
		term_rmargin = console_size.X - margin;
}

static unsigned color = VIDCOLOR_DEFAULT; // Current color

void d_cursor_goto(uint16_t x, uint16_t y) {
	uint16_t loc = VID_CALC_POS(x, y);
	outb(0x3D4, 14);
	outb(0x3D5, loc >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, loc);
}

void d_scroll(bool force, uint16_t amount) {
	// Scroll up
	if (console_cursor.Y >= console_size.Y || force) {
		for (uint32_t i = 0; i <= 25 * 80 * 2; i++) {
			vid_ptr[i] = vid_ptr[i + 80*2];
			vid_ptr[i + 80 * 2] = vid_ptr[i*80 + 80 * 2];
		}
		console_cursor.Y = console_size.Y - 1;
	}
}

void d_scroll(bool up) {
	d_scroll(up, 1);
}

void d_update_cursor() {
	d_scroll(false);
	d_cursor_goto(console_cursor.X, console_cursor.Y);
}

void d_clrscr() {
	d_clrscr(color); // Clear with current color
}

void d_clrscr_rst(bool reset_cursor) {
	d_clrscr(color, reset_cursor);
}

void d_clrscr(const unsigned short color) {
	d_clrscr(color, true);
}

void d_clrscr(const unsigned short color, bool reset_cursor) {
	for (uint32_t i = 0; i < VID_SIZE_CALC(); i += 2) {
		vid_ptr[i] = ' ';
		vid_ptr[i + 1] = color;
	}
	
	// Set cursor back to the start
	if (reset_cursor)
		d_gotoxy(0, 0);
}

void d_putc(uint8_t c) {
	d_putc(c, color);
}

void d_putc(uint8_t c, uint8_t color) {
	if (c == 0) return;

	switch (c) {
		case KEY_BACKSPACE: { // Backspace
			console_cursor.X--;
			if (console_cursor.X < 0) {
				console_cursor.X = 40;
				console_cursor.Y--;
			}
			uint32_t loc = VID_CALC_POS(console_cursor.X, console_cursor.Y);

			vid_ptr[loc * 2] = ' ';
			vid_ptr[loc * 2 + 1] = color;
			break;
		}
		case KEY_TAB: // Tab
			console_cursor.X = (console_cursor.X + 8) & ~(8 - 1);
			break; 
		case KEY_NEWLINE: // New line
			console_cursor.X = term_lmargin;
			console_cursor.Y++;
			break;
		case KEY_CARRIAGE: // Carriage return
			console_cursor.X = 0;
			break;
		default: // Normal printable character
			if (PRINTABLE_CHAR(c)) {
				uint32_t loc = VID_CALC_POS(console_cursor.X,console_cursor.Y);

				vid_ptr[loc * 2] = c;
				vid_ptr[loc * 2 + 1] = color;

				console_cursor.X++;
			}
			break;
	}

	// New line because it overflowed outside the window:
	if (console_cursor.X >= term_rmargin) {
		console_cursor.X = term_lmargin;
		console_cursor.Y++;
	}
	d_update_cursor();
}

void d_puts(char* str) {
	if (!str) return;

	for (size_t i = 0; i < strlen(str); i++)
		d_putc(str[i]);
}

int d_printf(const char* str, ...) {
	if (!str) return 0;

	va_list args;
	va_start(args, str);

	for (size_t i = 0; i < strlen(str); i++) {
		switch (str[i]) {
			case '%':
				switch (str[i + 1]) {
					case 'c': {
						char c = va_arg(args, char);
						d_putc(c);
						i++;
						break;
					}
					case 's':
						d_puts(va_arg(args, char*));
						i++;
						break;
					case 'p': {
						int c = (int&)va_arg(args, char);
						char str[32] = { 0 };
						itoa_s(c, 16, str);
						d_puts(str);
						i++;
						break;
					}
					case 'd':
					case 'i': {
						int d = va_arg(args, int);
						char str[32] = { 0 };
						itoa_s(d, 10, str);
						d_puts(str);
						i++;
						break;
					}
					case 'x':
					case 'X': {
						int c = va_arg(args, int);
						char str[32] = { 0 };
						itoa_s(c, 16, str);
						d_puts(str);
						i++;
						break;
					}
					default:
						va_end(args);
						return 1;
				}
				break;
			default: 
				d_putc(str[i]);
				break;
		}
	}

	va_end(args);
	return 0;
}

unsigned d_setcolor(const unsigned c) {
	unsigned old = color;
	color = c;
	return old;
}

UPoint d_gotoxy(unsigned x, unsigned y) {
	UPoint oldPoint = { console_cursor.X, console_cursor.Y };
	
	if(x<=console_size.X)
		console_cursor.X = x;
	if(y<=console_size.Y)
		console_cursor.Y = y;

	d_update_cursor();

	return oldPoint;
}

uint8_t get_color(uint8_t back, uint8_t front) {
	return (back << 4) | front;
}

UPoint d_getCursor() {
	return console_cursor;
}