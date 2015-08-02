#include "system\system.h"

void kmain(multiboot_header_t* MultibootStructure, uint32_t initial_stack) {
	system_initialize(MultibootStructure, initial_stack);
	
	welcome_terminal();
	
	d_printf(" Floppy drive: %s\n", dr_floppy_detect_drives());

	dump_ramdisk();

	system_shutdown();
}
