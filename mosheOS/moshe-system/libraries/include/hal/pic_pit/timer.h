#pragma once

#include "type\types.h"
#define TIMER_DEFAULT_HZ 1

extern int get_tick_count() __attribute__((cdecl));
extern void init_timer(uint32_t frequency)  __attribute__((cdecl));