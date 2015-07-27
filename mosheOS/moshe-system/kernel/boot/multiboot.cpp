#include "boot\multiboot.h"

multiboot_header_t* boot_multiheader;

void set_multiboot_header(multiboot_header_t* header) {
	boot_multiheader = header;
}

uint32_t mem_size() {
	// Calculate memory (RAM in Kb) size using multiboot header
	return boot_multiheader->mem_upper - boot_multiheader->mem_lower;
}

char * mem_btldr_name() {
	return (char*)boot_multiheader->boot_loader_name;
}

char * mem_kernel_filename() {
	return (char*)boot_multiheader->cmdline;
}

uint32_t mem_initrd_location() {
	return *((uint32_t*)(boot_multiheader->mods_addr));
}

uint32_t mem_initrd_location_end() {
	return *((uint32_t*)(boot_multiheader->mods_addr+4));
}

uint32_t mem_module_count() {
	return boot_multiheader->mods_count;
}