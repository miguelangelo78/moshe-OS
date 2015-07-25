#include "debug\terminal.h"
#include "hal\hal.h"

void kmain(void* MultibootStructure) {
	hal_initialize();

	d_clrscr();
	char buff[] = "CPU: %s";
	char * cpuid = get_cpu_vender();
	d_gotoxy(VID_CENTER_W - 12, VID_CENTER_H);
	d_printf(buff, cpuid);


	hal_shutdown();
}

