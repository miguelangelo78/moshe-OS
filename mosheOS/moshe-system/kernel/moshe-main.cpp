#include "system\system.h"

const char OS_NAME[] = "Moshe Operating System";
const char OS_VER[] = "0.1";

char welcome_msg[] =
"_______________________________________________________\n"
"    _   _                                 __       __  \n"
"    /  /|               /               /   ))   /    )\n"
"   /| / |    __   __   /__    __       /   //    \\     \n"
"  / |/  |  /   ) (_ ` /   ) /___)     /   //      \\    \n"
" /  /   | (___/ (__) /   / (___      (___//   (____/    \n"
"_______________________________________________________\n\n"
"           %s | Version: %s\n"
"_______________________________________________________";

void welcome_terminal() {
	// Print top screen:
	d_setcolor(get_color(VIDLightRed, VIDWhite));
	d_clrscr();
	d_term_lmargin(10);
	d_gotoxy(10, 0);
	d_printf(welcome_msg, OS_NAME, OS_VER);
	d_term_lmargin(0);
	d_printf("\n");

	// Print bottom screen:
	d_setcolor(get_color(VIDWhite, VIDBlack));

	UPoint oldPoint = d_getCursor();

	for (int i = 0; i < 14 * 80; i++)
		d_putc(' ');

	d_gotoxy(oldPoint.X + 1, oldPoint.Y + 1);
	d_printf("Welcome to my Operating System!\n\n Your CPU vendor is: %s", get_cpu_vender());
}

void kmain(void* MultibootStructure) {
	system_initialize();

	welcome_terminal();

	system_shutdown();
}

