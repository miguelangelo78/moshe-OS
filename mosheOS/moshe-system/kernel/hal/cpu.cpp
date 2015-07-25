#include "hal\cpu.h"

void cpu_initialize() {
	init_descriptor_tables();
	enable_ints();
}

void cpu_shutdown() {
	cpu_hang(); // TODO: TEMPORARY
	
	// TODO: Completely shutdown CPU, don't just halt
	disable_ints();
	asm volatile("hlt");
}

void cpu_halt() {
	disable_ints();
	asm volatile("hlt");
}

void cpu_hang() {
	for (;;);
}

void enable_ints() {
	asm volatile("sti");
}

void disable_ints() {
	asm volatile("cli");
}

char* get_cpu_vender() {
	static char vender[32] = { 0 };
	asm volatile("xor %%eax, %%eax; cpuid; movl %%ebx, %0; movl %%edx, 4+%0; movl %%ecx, 8+%0":"=m"(vender));
	return vender;
}