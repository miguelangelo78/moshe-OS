#include "driver_man.h"
#include "memory\heap\kheap.h"

void initialize_drivers() {
	// Initialize all drivers:
	dr_floppy_initialize(0x8000); // FIXME: Don't use a fixed address!!!!
}