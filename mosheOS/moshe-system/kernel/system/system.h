#pragma once

#include "hal\hal.h"
#include "memory\heap\kheap.h"
#include "memory\heap\paging.h"

#include "debug\terminal.h"

extern void system_initialize();
extern void system_shutdown();
extern void system_halt();
