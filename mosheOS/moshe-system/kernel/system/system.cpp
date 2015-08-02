#include "system.h"

void system_initialize(multiboot_header_t* multiboot_ptr, uint32_t stack_ptr) {
	d_clrscr_rst(true);
	d_printf("Initializing Moshe OS . . .");

	set_multiboot_header(multiboot_ptr);
	heap_set_esp(stack_ptr);
	
	hal_initialize();
	
	uint32_t initrd_loc = mem_initrd_location();
	placement_address = mem_initrd_location_end();
	
	initialize_paging();
	
	// Initialize ramdisk and file system!
	
	fs_root = initialize_initrd(initrd_loc);

	// Load modules, configurations and drivers from the ram disk through the variable 'fs_root'

	// then, after everything is loaded and configured, set fs_root to the real file system on the hard disk

	initialize_drivers();
}

void system_shutdown() {
	hal_shutdown();
}

void system_halt() {
	hal_halt();
}

const char OS_NAME[] = "Moshe Operating System";
const char OS_VER[] = "0.2";

#pragma region WELCOME

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
	d_setcolor(get_color(VIDGreen, VIDWhite));
	d_clrscr();
	d_term_lmargin(10);
	d_gotoxy(10, 0);
	d_printf(welcome_msg, OS_NAME, OS_VER);
	d_term_lmargin(0);
	d_printf("\n");

	// Print bottom screen:
	d_setcolor(get_color(VIDBlack, VIDLightGray));

	UPoint oldPoint = d_getCursor();

	for (int i = 0; i < 14 * 80; i++)
		d_putc(' ');

	d_gotoxy(oldPoint.X + 1, oldPoint.Y + 1);
	d_printf("Welcome to my Operating System!\n\n Your CPU vendor is: %s\n", get_cpu_vender());
}

#pragma endregion
