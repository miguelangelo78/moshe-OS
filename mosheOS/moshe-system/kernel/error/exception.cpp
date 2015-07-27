#include "error\exception.h"
#include "string\string.h"
#include "stdlib.h"

void handle_Exception(registers_t regs, uint32_t code) {
	// Run isr handler if it was installed, otherwise handle it right here in this function
	if (isr_run_handler(code, regs)) return;
	
	// DECIDE WHETHER OR NOT IT'S A BAD EXCEPTION

	// IF IT IS BAD:
	// SHOW BSOD:
	kernel_panic(code);
}

char pg_fault_info[256] = "Page fault! (";

void page_fault(registers_t regs) {
	// A page fault has occurred.
	// The faulting address is stored in the CR2 register.
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

	// The error code gives us details of what happened.
	bool present = !(regs.err_code & 0x1); // Page not present
	bool rw = regs.err_code & 0x2; // Write operation?
	bool us = regs.err_code & 0x4; // Processor was in user-mode?
	bool reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
	bool id = regs.err_code & 0x10;          // Caused by an instruction fetch?

	// Build error messase:
	if (present) strcat(pg_fault_info, "page not present, ");
	if (rw) strcat(pg_fault_info, "address read only, ");
	if (us) strcat(pg_fault_info, "user mode, ");
	if (reserved) strcat(pg_fault_info, "page reserved for CPU, ");
	if(id) strcat(pg_fault_info, "caused by instruction fetch, ");

	strcat(pg_fault_info, "...) at address 0x");
	
	char addr_as_str[10];
	itoa(faulting_address, 16, addr_as_str);
	strcat(pg_fault_info, addr_as_str);
	strcat(pg_fault_info, "!");
	
	kernel_panic(regs.int_no, pg_fault_info);
}