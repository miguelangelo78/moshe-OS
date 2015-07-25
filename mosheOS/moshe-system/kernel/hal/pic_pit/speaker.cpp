#include "hal\hal.h"
#include "debug\terminal.h"

void speaker(uint32_t frequency) {
	uint32_t divisor = PIT_FREQUENCY / frequency;

	// Configure PIT:
	outb(0x43, 0xb6);
	
	outb(0x42, (uint8_t) divisor);
	outb(0x42, (uint8_t)(divisor >> 8));

	// And play sound:
	uint8_t tmp = inb(0x61);
	if (tmp != (tmp | 3)) outb(0x61, tmp | 3);
}

void stop_speaker() {
	outb(0x61, inb(0x61) & 0xFC);
}