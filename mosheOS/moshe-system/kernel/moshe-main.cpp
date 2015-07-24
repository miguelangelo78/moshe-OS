#include "debug\terminal.h"
#include "defs\video.h"

void kmain(void* MultibootStructure) {
	d_clrscr();

	d_gotoxy(0, 1);
	unsigned int old = d_setcolor(get_color(VIDGreen, VIDBlack));
	d_printf("Hello \n%s !", "World");
	d_clrscr_rst(false);
	d_setcolor(old);
	d_printf("Here i Am %s", "again!");
}
