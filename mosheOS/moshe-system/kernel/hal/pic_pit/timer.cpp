#include "debug\terminal.h"
#include "hal\hal.h"

uint32_t tick = 0;

static void timer_callback(registers_t regs) {
	UPoint oldPoint = d_gotoxy(0, 0);
	d_printf("Tick: %d\n", tick);
	d_gotoxy(oldPoint.X, oldPoint.Y);
	tick++;
}

void init_timer(uint32_t frequency) {
	install_interrupt_handler(PIC_IRQ_TIMER, &timer_callback);
	uint32_t divisor = PIT_FREQUENCY / frequency;
	
	outb(0x43, 0x36);

	uint8_t lower = (uint8_t)(divisor & 0xFF);
	uint8_t upper= (uint8_t)((divisor >> 8) & 0xFF);

	outb(0x40, lower);
	outb(0x40, upper);
}

int get_tick_count() {
	return tick;
}
