#pragma once

#include "type\types.h"
#define TIMER_DEFAULT_HZ 100

extern int get_tick_count();
extern void init_timer(uint32_t frequency) __attribute__((cdecl));