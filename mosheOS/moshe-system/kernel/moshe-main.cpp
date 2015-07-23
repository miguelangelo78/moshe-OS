#include "debug\terminal.h"
#include "defs\video.h"

char str[] = " banana";

void kmain(void* MultibootStructure) {
	d_clrscr(0x18);
	d_gotoxy(VID_CENTER_W, VID_CENTER_H);
	d_setcolor(0x17);
	d_printf("Hello %d%s", 10, str);
}
