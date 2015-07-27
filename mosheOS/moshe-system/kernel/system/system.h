#pragma once

#include "boot\multiboot.h"
#include "hal\hal.h"
#include "memory\heap\kheap.h"
#include "memory\heap\paging.h"
#include "../filesystem/initrd.h"

/* TODO: Move this stuff somewhere else */
#include "debug\terminal.h"
extern void welcome_terminal();
extern const char OS_NAME[];
extern const char OS_VER[];
/* This stuff up here */

extern void system_initialize(multiboot_header_t* multiboot_ptr);
extern void system_shutdown();
extern void system_halt();
