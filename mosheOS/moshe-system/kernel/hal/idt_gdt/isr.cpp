#include "hal\hal.h"
#include "debug\terminal.h"
#include "type\types.h"
#include "error\exception.h"

isr_t interrupt_handlers[256];
void interrupt_done(uint32_t irq_code);

// EXCEPTIONS:
void isr_handler(registers_t regs) {
	uint32_t code = regs.int_no;
	
	//Handle exception if necessary:
	handle_Exception(regs, code);

	// Call IRQ in case it was registered
	isr_run_handler(code, regs); // This will only run if the 'code' IRQ was installed
}

// INTERRUPT REQUESTS:
// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs) {
	uint32_t code = regs.int_no;
	
	// This will only run if the 'code' IRQ was installed
	isr_run_handler(code, regs); 
	
	// INTERRUPT DONE
	interrupt_done(code);
}

void interrupt_done(uint32_t irq_code) {
	// Send an EOI (end of interrupt) signal to the PICs.
	// If this interrupt involved the slave.
	if (irq_code >= 40) // Send reset signal to slave.
		outb(0xA0, 0x20);

	// Send reset signal to master. (As well as slave, if necessary).
	outb(0x20, 0x20);
}

void install_interrupt_handler(uint8_t n, isr_t handler) {
	if(n>=0 && n<256)
		interrupt_handlers[n] = handler;
}

void uninstall_interrupt_handler(uint8_t n) {
	if (n >= 0 && n < 256)
		interrupt_handlers[n] = 0;
}

bool isr_is_installed(uint8_t isr_code) {
	return interrupt_handlers[isr_code] != 0;
}

bool isr_run_handler(uint8_t handler_code, registers_t regs) {
	if (isr_is_installed(handler_code)) {
		interrupt_handlers[handler_code](regs); // Grab callback and run it
		return true;
	}
	else return false;
}