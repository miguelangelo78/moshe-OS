#include "debug\terminal.h"
#include "io\stdio.h"

void kmain(void* MultibootStructure) {
	d_clrscr();
	char buff[] = "Kernel World";
	d_gotoxy(VID_CENTER_W-strlen(buff)+1, VID_CENTER_H);
	d_printf("Hello %s", buff);
	d_putc(0x08);
	d_putc(0x08);
	d_putc(0x08);
	d_putc(0x09);
	d_putc('*'); d_putc('*'); d_putc('*');
}
