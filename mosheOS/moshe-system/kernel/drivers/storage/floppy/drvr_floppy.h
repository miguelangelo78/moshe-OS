#pragma once

#include "type\types.h"
#include "hal\hal.h"

void dr_floppy_initialize(uint32_t dma_buffer_loc);

void dr_floppy_install();

void dr_floppy_set_working_drive(uint8_t drive);

void dr_floppy_dma_set_buffer_loc(uint32_t addr);

uint8_t dr_floppy_get_working_drive();

uint8_t * dr_floppy_read_sector(int sectorLBA);

void dr_floppy_lba_to_chs(int lba, int * head, int *track, int * sector);