#pragma once

#include "type\types.h"

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08 // Is the file an active mountpoint?

struct FILE;
struct DIRENT;

// These typedefs define the type of callbacks - called when read/write/open/close
// are called.
typedef uint32_t(*read_type_t)(FILE*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t(*write_type_t)(FILE*, uint32_t, uint32_t, uint8_t*);
typedef void(*open_type_t)(FILE*);
typedef void(*close_type_t)(FILE*);
typedef DIRENT* (*readdir_type_t)(FILE*, uint32_t);
typedef struct FILE* (*finddir_type_t)(FILE*, char*name);

struct FILE {
	char name[128]; // filename
	uint32_t mask;	// permissions mask
	uint32_t uid; // who owns the node
	uint32_t gid; // what group owns the node
	uint32_t flags; // indicates the node type (file, directory, mountpoint, pipe or something else)
	uint32_t inode; // a way for a filesystem to ID files
	uint32_t length; // size of the file (in bytes)
	uint32_t impl;

	// callback functions: (they're function pointers btw)
	read_type_t read;
	write_type_t write;
	open_type_t open;
	close_type_t close;
	readdir_type_t readdir;
	finddir_type_t finddir;
	FILE * ptr; // for mounting purposes
};

struct DIRENT {
	char name[128]; // filename
	uint32_t ino; // node number
};

extern FILE * fs_root; // declare root!

// actually declare the open, read, write, close, .... , functions:

// Standard read/write/open/close functions. Note that these are all suffixed with
// _fs to distinguish them from the read/write/open/close which deal with file descriptors
// , not file nodes.
uint32_t read_fs(FILE *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_fs(FILE *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_fs(FILE *node, uint8_t read, uint8_t write);
void close_fs(FILE *node);
DIRENT *readdir_fs(FILE *node, uint32_t index);
FILE *finddir_fs(FILE *node, char *name);