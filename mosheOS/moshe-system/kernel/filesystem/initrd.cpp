#include "initrd.h"
#include "memory\heap\kheap.h"
#include "string\string.h"
#include "debug\terminal.h"

DIRENT dir_found; // Used for readdir
initrd_header_t *initrd_header; // The header.
initrd_file_header_t *file_headers; // The list of file headers.
FILE *initrd_root; // Our root directory node.
FILE *initrd_dev; // We also add a directory node for /dev, so we can mount devfs later on.
FILE *root_nodes; // List of file nodes.
uint16_t root_nodes_count; // Number of file nodes.

uint32_t initrd_read(FILE *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	initrd_file_header_t header = file_headers[node->inode];
	if (offset > header.length) return 0;

	if (offset + size > header.length)
		size = header.length - offset;

	memcpy(buffer, (uint8_t*)(header.offset + offset), size);
	return size;
}

DIRENT *initrd_readdir(FILE *node, uint32_t index) {
	if (node == initrd_root && index == 0) {
		strcpy(dir_found.name, "dev");
		dir_found.ino = index;
		return &dir_found;
	}

	if (index > root_nodes_count) return 0;

	strcpy(dir_found.name, root_nodes[index - 1].name);
	dir_found.ino = root_nodes[index - 1].inode;
	return &dir_found;
}

FILE *initrd_finddir(FILE *node, char *name) {
	if (node == initrd_root && !strcmp(name, (char*)"dev"))
		return initrd_dev;

	for (int i = 0; i < root_nodes_count; i++)
		if (!strcmp(name, root_nodes[i].name))
		return &root_nodes[i];
	return 0;
}

FILE* initialize_initrd(uint32_t location) {
	// Initialise the main and file header pointers and populate the root directory.
	initrd_header = (initrd_header_t *)location;
	file_headers = (initrd_file_header_t *)(location + sizeof(initrd_header_t));

	/************** Initialise the root directory. **************/
	initrd_root = (FILE*)kmalloc(sizeof(FILE));

	strcpy(initrd_root->name, "initrd");
	initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->length = 0;
	initrd_root->flags = FS_DIRECTORY;
	// Initialize function pointers to 0 for root:
	initrd_root->read = 0;
	initrd_root->write = 0;
	initrd_root->open = 0;
	initrd_root->close = 0;
	initrd_root->ptr = 0;
	initrd_root->impl = 0;
	initrd_root->readdir = &initrd_readdir;
	initrd_root->finddir = &initrd_finddir;

	/************** Initialise the /dev directory (required!) **************/
	initrd_dev = (FILE*)kmalloc(sizeof(FILE));
	strcpy(initrd_dev->name, "dev");
	initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = initrd_dev->inode = initrd_dev->length = 0;
	initrd_dev->flags = FS_DIRECTORY;
	// Initialize function pointers to 0 for /dev:
	initrd_dev->read = 0;
	initrd_dev->write = 0;
	initrd_dev->open = 0;
	initrd_dev->close = 0;
	initrd_dev->ptr = 0;
	initrd_dev->impl = 0;
	initrd_dev->readdir = &initrd_readdir;
	initrd_dev->finddir = &initrd_finddir;

	// Allocate ramdisk:
	root_nodes = (FILE*)kmalloc(sizeof(FILE) * initrd_header->nfiles);
	root_nodes_count = initrd_header->nfiles;
	
	// Now actually set all function pointers for both / and /dev
	for (int i = 0; (uint32_t)i < initrd_header->nfiles; i++) {
		// Edit the file's header - currently it holds the file offset
		// relative to the start of the ramdisk. We want it relative to the start
		// of memory.
		file_headers[i].offset += location;

		// Create a new file node.
		strcpy(root_nodes[i].name, file_headers[i].name);
		root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
		root_nodes[i].length = file_headers[i].length;
		root_nodes[i].inode = i;
		root_nodes[i].flags = FS_FILE;
		root_nodes[i].read = &initrd_read;
		// No need to use these functions on initrd, just simply read, so don't initialize
		root_nodes[i].write = 0;
		root_nodes[i].readdir = 0;
		root_nodes[i].finddir = 0;
		root_nodes[i].open = 0;
		root_nodes[i].close = 0;
		root_nodes[i].impl = 0;
	}
	return initrd_root;
}

void dump_ramdisk() {
	d_printf("\n Dumping RAMDISK . . .\n\tcontents of /:\n");

	DIRENT * dir_file = 0;

	for (int i = 0; (dir_file = readdir_fs(fs_root, i)) != 0; i++) { // Read i-th directory

		d_printf("\t\t%s", dir_file->name);
		FILE * fsnode = finddir_fs(fs_root, dir_file->name);

		if ((fsnode->flags & 0x7) == FS_DIRECTORY)
			d_printf("\n\t\t\t(directory)\n");
		else {
			char buf[256];
			read_fs(fsnode, 0, 256, (uint8_t*)buf);
			d_printf("\n\t\t\t(file) contents: \"%s\"\n", buf);
		}
	}
}