#include "debug\terminal.h"
#include "type\types.h"

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

static unsigned color = VIDCOLOR_DEFAULT; // Current color

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
	if(reset_cursor)
		console_cursor.X = console_cursor.Y = 0;
}

void d_putc(uint8_t c) {
	d_putc(c, color);
}

void d_putc(uint8_t c, uint8_t color) {
	if (c == 0) return;

	// Backspace:
	if (console_cursor.X) // If cursor.X > 0
		switch (c) {
			case KEY_BACKSPACE: {// Backspace
				console_cursor.X--;
				uint32_t loc = VID_CALC_POS(console_cursor.X, console_cursor.Y);

				vid_ptr[loc * 2] = ' ';
				vid_ptr[loc * 2 + 1] = color;
				break;
			}
			case KEY_TAB: // Tab
				console_cursor.X = (console_cursor.X + 8) & ~(8 - 1);
				break; 
			case KEY_NEWLINE: // New line
				console_cursor.X = 0;
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
	if (console_cursor.X >= console_size.X) {
		console_cursor.X = 0;
		console_cursor.Y++;
	}
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

void d_gotoxy(unsigned x, unsigned y) {
	console_cursor.X = x;
	console_cursor.Y = y;
}

uint8_t get_color(uint8_t back, uint8_t front) {
	return (back << 4) | front;
}