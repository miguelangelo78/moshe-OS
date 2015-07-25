#pragma once

const static char error_msgs[][33] = {
	{ "Division By 0" },
	{ "Single step" },
	{ "Non maskable interrupt (NMI)" },
	{ "Breakpoint (debugger)" },
	{ "Overflow" },
	{ "Bounds check" },
	{ "OPCode undefined" },
	{ "No coprocessor" },
	{ "Double fault" },
	{ "Coprocessor Segment Overrun" },
	{ "Invalid task State Segment (TSS)" },
	{ "Segment not Present" },
	{ "Stack segment Overrun" },
	{ "General Protection Fault (GPF)" },
	{ "Page fault" },
	{ "Unassigned" },
	{ "Coprocessor error" },
	{ "Alignment check (486+ only)" },
	{ "Machine check (pen/586+ only)" },
	{ "Reserved exception" },
};