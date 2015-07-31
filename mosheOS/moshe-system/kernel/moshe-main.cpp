#include "system\system.h"

void kmain(multiboot_header_t* MultibootStructure, uint32_t initial_stack) {
	system_initialize(MultibootStructure, initial_stack);
	
	welcome_terminal();
	
	d_printf("Floppy drive: %s", dr_floppy_detect_drives());

	char * buff = (char*)kmalloc(FLOPPY_DMA_BUFFER_SIZE);
	floppy_read_s(buff,0);

	for (int i = 0; i < FLOPPY_DMA_BUFFER_SIZE; i++)
		d_printf("%c ", buff[i]);

	system_shutdown();
}
