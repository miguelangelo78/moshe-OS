#pragma once

#include "boot\multiboot.h"
#include "hal\hal.h"
#include "memory\heap\kheap.h"
#include "memory\heap\paging.h"
#include "../filesystem/initrd.h"
#include "../drivers/driver_man.h"
#include "task\thread.h"

/* TODO: Move this stuff somewhere else */
#include "debug\terminal.h"
extern void welcome_terminal();
extern const char OS_NAME[];
extern const char OS_VER[];
/* This stuff up here */

extern void system_initialize(multiboot_header_t* multiboot_ptr, uint32_t stack_ptr);
extern void system_shutdown();
extern void system_halt();
