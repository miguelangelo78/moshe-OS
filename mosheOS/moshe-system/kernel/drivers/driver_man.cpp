#include "driver_man.h"
#include "memory\heap\kheap.h"

void initialize_drivers() {
	// Initialize all drivers:
	//dr_flpy_initialize(0x8000); // BUG: This floppy driver is broken, works on Virtualbox but not VMWare, why? -.-' FIXME: Don't use a fixed address!!!!
}