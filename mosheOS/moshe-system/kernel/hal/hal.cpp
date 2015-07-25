#include "hal\hal.h"

void hal_initialize() {
	// Initialize GDT, IDT and initialize PIC and PIT
	cpu_initialize(); 
	
	// Install PICs and PIT:
	init_keyboard();
	init_timer(TIMER_DEFAULT_HZ); 
}

void hal_shutdown() {
	cpu_shutdown();
}

void hal_hang() {
	cpu_hang();
}

void hal_halt() {
	cpu_halt();
}

void hal_set_ints(bool disable) {
	if (disable) disable_ints();
	else enable_ints();
}

void outb(uint16_t port, uint8_t val) {
	asm volatile("outb %1, %0" : : "dN" (port), "a" (val));
}

uint8_t inb(uint16_t port) {
	uint8_t res;
	asm volatile("inb %1, %0" : "=a" (res) : "dN" (port));
	return res;
}

uint16_t inw(uint16_t port) {
	uint16_t res;
	asm volatile("inw %1, %0" : "=a" (res) : "dN" (port));
	return res;
}

