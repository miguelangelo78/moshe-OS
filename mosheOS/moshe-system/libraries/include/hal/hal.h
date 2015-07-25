#pragma once
#include "type\types.h"
#include "hal\cpu.h"
#include "hal\pic_pit\timer.h"
#include "hal\pic_pit\keyboard.h"
#include "hal\pic_pit\sound.h"

#define PIT_FREQUENCY 1193180

extern void hal_initialize();
extern void hal_shutdown();
extern void hal_hang();
extern void hal_halt();

extern void hal_set_ints(bool disable);

// IO:
extern void outb(uint16_t port, uint8_t val) __attribute__((cdecl));
extern uint8_t inb(uint16_t port) __attribute__((cdecl));
extern uint16_t inw(uint16_t port) __attribute__((cdecl));