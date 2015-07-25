#include "debug\terminal.h"
#include "hal\descriptor_tables.h"

void kmain(void* MultibootStructure) {
	init_descriptor_tables();

	d_clrscr();
	d_gotoxy(VID_CENTER_W - 12, VID_CENTER_H);
	d_printf("Hello %s\n", "Kernel World");

	
	d_printf("Nyess");
}
