#pragma once

#include "type\types.h"
#include "hal\regs.h"

extern void install_interrupt_handler(uint8_t n, isr_t handler);
extern void uninstall_interrupt_handler(uint8_t n);
extern bool isr_is_installed(uint8_t isr_code);
extern bool isr_run_handler(uint8_t handler_code, registers_t regs);
extern isr_t interrupt_handlers[256];

#define PIC_IRQ_TIMER 32
#define PIC_IRQ_KEYBOARD 33
#define PIC_IRQ_SPEAKER 34
#define PIC_IRQ_SERIAL1 35
#define PIC_IRQ_PARALLEL2 36
#define PIC_IRQ_DISKETTE 37
#define PIC_IRQ_PARALLEL1 38

#define PIC_IRQ_CMOSTIMER 39
#define PIC_IRQ_CGARETRACE 40
#define PIC_IRQ_AUXILIARY 41
#define PIC_IRQ_FPU 42
#define PIC_IRQ_HDC 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

