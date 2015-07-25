#include "error\panic.h"

void kernel_panic(uint32_t code) {
	uint8_t BSOD_COLOR = get_color(VIDBlue, VIDWhite);
	d_setcolor(BSOD_COLOR);
	d_clrscr();

	// DRAW SQUARE:
	int sq_width = strlen(error_msgs[code]);
	int sq_height = 4;
	d_setcolor(get_color(VIDRed, VIDWhite));
	for (int i = VID_CENTER_H - sq_height / 2; i < VID_CENTER_H + sq_height / 2 + 1; i++)
		for (int j = VID_CENTER_W - sq_width - 5; j < VID_CENTER_W + sq_width; j++) {
			d_gotoxy(j, i);
			d_putc(' ');
		}

	// Show BSOD Message:
	d_gotoxy(VID_CENTER_W - sq_width, VID_CENTER_H - 1);
	d_printf("OOPS! :(");
	d_gotoxy(VID_CENTER_W - sq_width, VID_CENTER_H + 1);
	d_printf("ERROR %d: %s", code, error_msgs[code]);

	hal_halt();
}