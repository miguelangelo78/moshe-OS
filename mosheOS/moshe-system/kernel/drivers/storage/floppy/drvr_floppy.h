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

void floppy_write_track(int base, unsigned cyl);
void floppy_deep_write(char* buffer);
void floppy_write_s(char* buffer, unsigned cyl);
void floppy_write_st(char* buffer, unsigned cyl_start);
void floppy_write_en(char* buffer, unsigned cyl_end);
void floppy_write(char* buffer, unsigned cyl_start, unsigned cyl_end);

void floppy_deep_read(char* buffer);
void floppy_read_s(char* buffer, unsigned cyl);
void floppy_read_st(char* buffer, unsigned cyl_start);
void floppy_read_en(char* buffer, unsigned cyl_end);
void floppy_read(char* buffer, unsigned cyl_start, unsigned cyl_end);
