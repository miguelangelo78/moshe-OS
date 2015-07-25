#pragma once
#include "hal\descriptor_tables.h"
#include "hal\isr.h"
#include "hal\regs.h"

extern void cpu_initialize() __attribute__((cdecl));
extern void cpu_shutdown() __attribute__((cdecl));
extern void cpu_hang() __attribute__((cdecl));
extern void cpu_halt() __attribute__((cdecl));
extern void enable_ints() __attribute__((cdecl));
extern void disable_ints() __attribute__((cdecl));

extern char* get_cpu_vender() __attribute__((cdecl));