#pragma once

enum EXCEPTION_LIST {
	EXC_DIVBY0,
	EXC_SINGLESTEP,
	EXC_NONMASKABLE,
	EXC_BREAKPOINT,
	EXC_OVERFLOW,
	EXC_BOUNDS,
	EXC_OPUNDEF,
	EXC_NOCOP,
	EXC_DBLFAULT,
	EXC_SEGOVERRUN,
	EXC_INVTASK,
	EXC_NOSEG,
	EXC_STACKOVERR,
	EXC_PROTFAULT,
	EXC_PAGEFAULT,
	EXC_UNASS,
	EXC_COERR,
	EXC_ALIGNCHECK,
	EXC_MACHCHECK,
	EXC_RESEXC,
	EXC_DMA_BADBUFF
};

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
	{ "DMA Error - Bad buffer" }
};