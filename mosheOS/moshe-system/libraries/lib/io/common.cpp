#include "io\common.h"

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