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

void isr_handler(registers_t regs) {
	uint32_t exc = regs.int_no;
	uint8_t BSOD_COLOR = get_color(VIDBlue, VIDWhite);
	d_setcolor(BSOD_COLOR);
	d_clrscr();

	d_gotoxy(VID_CENTER_W - strlen(error_msgs[exc]), VID_CENTER_H);
	d_printf("ERROR %d: %s", exc, error_msgs[exc]);
	asm volatile("cli; hlt");
}
