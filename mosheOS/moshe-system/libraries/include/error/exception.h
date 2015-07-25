#pragma once

#include "error\exception_data.h"
#include "error\panic.h"
#include "hal\hal.h"

extern void handle_Exception(registers_t regs, uint32_t code);
