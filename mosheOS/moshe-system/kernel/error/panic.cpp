#include "error\panic.h"

void kernel_panic(uint32_t code) {
	kernel_panic(code, (char*)""); // Unknown cause, call CSI for this one
}

void kernel_panic(uint32_t code, char * cause) {
	uint8_t BSOD_COLOR = get_color(VIDBlue, VIDWhite);
	d_setcolor(BSOD_COLOR);
	d_clrscr();

	// DRAW SQUARE:
	int sq_width = strlen(error_msgs[code]);
	
	int sq_height = 12;
	d_setcolor(get_color(VIDRed, VIDWhite));
	for (int i = VID_CENTER_H-2; i < VID_CENTER_H + sq_height / 2 + 1; i++)
		for (int j = VID_CENTER_W - sq_width - 5; j < VID_CENTER_W + sq_width; j++) {
			d_gotoxy(j, i);
			d_putc(' ');
		}

	// Show proper BSOD Message:
	d_gotoxy(VID_CENTER_W - sq_width, VID_CENTER_H - 1);
	d_printf("OOPS! :(");
	d_gotoxy(VID_CENTER_W - sq_width, VID_CENTER_H + 1);
	d_term_lmargin(VID_CENTER_W - sq_width);
	d_term_rmargin(30);

	d_printf("ERROR %d: %s\n\nCAUSE: ", code, error_msgs[code]);
	
	d_term_lmargin(VID_CENTER_W - sq_width-4);
	d_printf("%s", cause);
	d_term_lmargin(0);
	d_term_rmargin(0);

	hal_halt();
}