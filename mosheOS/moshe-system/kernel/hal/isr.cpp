#include "hal\regs.h"
#include "debug\terminal.h"
#include "type\types.h"

char error_msgs[][33] = {
	{"Division By 0"},
	{"Single step"},
	{"Non maskable interrupt (NMI)"},
	{"Breakpoint (debugger)"},
	{"Overflow"},
	{"Bounds check"},
	{"OPCode undefined"},
	{"No coprocessor"},
	{"Double fault"},
	{"Coprocessor Segment Overrun"},
	{"Invalid task State Segment (TSS)"},
	{"Segment not Present"},
	{"Stack segment Overrun"},
	{"General Protection Fault (GPF)"},
	{"Page fault"},
	{"Unassigned"},
	{"Coprocessor error"},
	{"Alignment check (486+ only)"},
	{"Machine check (pen/586+ only)"},
	{"Reserved exception"},
};

void show_bsod(uint32_t code) {
	uint8_t BSOD_COLOR = get_color(VIDBlue, VIDWhite);
	d_setcolor(BSOD_COLOR);
	d_clrscr();

	// DRAW SQUARE:
	int sq_width = strlen(error_msgs[code]);
	int sq_height = 4;
	
	d_setcolor(get_color(VIDRed, VIDWhite));
	
	for (int i = VID_CENTER_H-sq_height/2; i < VID_CENTER_H + sq_height/2+1;i++)
		for (int j = VID_CENTER_W-sq_width-5; j < VID_CENTER_W+sq_width; j++) {
			d_gotoxy(j, i);
			d_putc(' ');
		}

	// Show BSOD Message:
	d_gotoxy(VID_CENTER_W - sq_width, VID_CENTER_H-1);
	d_printf("OOPS! :(");
	d_gotoxy(VID_CENTER_W - sq_width, VID_CENTER_H+1);
	d_printf("ERROR %d: %s", code, error_msgs[code]);
}

void handle_Exception(registers_t regs, uint32_t code) {
	// SHOW BSOD:
	show_bsod(code);
	// AND HANDLE IT
	asm volatile("cli; hlt");
}

void isr_handler(registers_t regs) {
	uint32_t code = regs.int_no;
	// DECIDE WHETHER OR NOT IT'S A BAD EXCEPTION

	// IF IT IS BAD:
	handle_Exception(regs, code);
}
