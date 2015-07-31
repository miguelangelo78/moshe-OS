#pragma once

#include "type\types.h"
#include "hal\hal.h"

#define FLOPPY_DMA_BUFFER_SIZE 0x4800
#define FLOPPY_CYLINDER_COUNT 80

#define FLOPPY_SECTORS_PER_CYLINDER 18
#define FLOPPY_SECTOR_SIZE 512
#define FLOPPY_CYLINDER_SIZE FLOPPY_SECTOR_SIZE * FLOPPY_SECTORS_PER_CYLINDER
#define FLOPPY_CYLINDER_SIZE_TWO_HEAD FLOPPY_CYLINDER_SIZE*2

#define FLOPPY_FULLSIZE FLOPPY_DMA_BUFFER_SIZE * FLOPPY_CYLINDER_COUNT

void dr_flpy_initialize(int dma_addr);

const char * dr_floppy_detect_drives();

//! install floppy driver
void flpydsk_install();

//! set current working drive
void flpydsk_set_working_drive(uint8_t drive);

//! get current working drive
uint8_t flpydsk_get_working_drive();

char* floppy_read_track(int base, unsigned cyl);

void floppy_deep_read(char* buffer);
