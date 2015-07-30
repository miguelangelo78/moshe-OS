#include "system\system.h"

void kmain(multiboot_header_t* MultibootStructure, uint32_t initial_stack) {
	system_initialize(MultibootStructure, initial_stack);
	
	welcome_terminal();

	uint8_t * sector = dr_floppy_read_sector(0);

	if (sector != 0) {
		int i = 0;
		for (int c = 0; c < 4; c++) {
			for (int j = 0; j < 128; j++)
				d_printf("0x%x ", sector[i + j]);
			i += 128;
		}
	}
	else d_printf("Oops!");

	//dump_ramdisk();

	system_shutdown();
}
