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
	if (interrupt_handlers[code] != 0) interrupt_handlers[code](regs);
}

// INTERRUPT REQUESTS:
// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs) {
	uint32_t code = regs.int_no;
	
	if (interrupt_handlers[code] != 0)
		interrupt_handlers[code](regs); // Grab callback and run it
	
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