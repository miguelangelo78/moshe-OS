#include "error\exception.h"

void handle_Exception(registers_t regs, uint32_t code) {
	// DECIDE WHETHER OR NOT IT'S A BAD EXCEPTION

	// IF IT IS BAD:
	// SHOW BSOD:
	kernel_panic(code);
}