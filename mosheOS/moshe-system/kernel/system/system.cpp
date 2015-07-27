#include "system.h"

void system_initialize() {
	hal_initialize();
	initialize_paging();
}

void system_shutdown() {
	hal_shutdown();
}

void system_halt() {
	hal_halt();
}

