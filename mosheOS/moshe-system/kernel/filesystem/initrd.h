#pragma once

#include "fs.h"

typedef struct {
	uint32_t nfiles; // number of files in the ram disk
} initrd_header_t;

typedef struct {
	uint8_t magic;	// magic numbers, for errors 'n stuff
	char name[64]; // filename
	uint32_t offset; // offset of the file
	uint32_t length; // length of the file, duh
} initrd_file_header_t;

// This function is self explanatory:
FILE* initialize_initrd(uint32_t location);

extern void dump_ramdisk();