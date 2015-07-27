#include "system\system.h"

void kmain(void* MultibootStructure) {
	system_initialize((multiboot_header_t*)MultibootStructure);
	
	welcome_terminal();

	dump_ramdisk();

	system_shutdown();
}
