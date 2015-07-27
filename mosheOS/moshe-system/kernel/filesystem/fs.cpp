#include "fs.h"

fs_node * fs_root = 0; // define root!

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	if (node->read != 0)
		return node->read(node, offset, size, buffer); // call callback!
	else return 0; // no callback was assigned to the node, wut? how?
}

uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	if (node->write != 0)
		return node->write(node, offset, size, buffer);
	else return 0; // seriously, who isn't initializing their variables?
}

void open_fs(fs_node_t *node, uint8_t read, uint8_t write) {
	if (node->open != 0) return node->open(node);
	// this time we're not checking, ah!
}

void close_fs(fs_node_t *node) {
	if (node->close != 0) return node->close(node);
	// it's closed now, yes it is, believe me
}

struct dirent *readdir_fs(fs_node_t *node, uint32_t index) {
	// check if node is a directory AND if it has a callback
	if ((node->flags & 0x7) == FS_DIRECTORY && node->readdir != 0)
		return node->readdir(node, index);
	else return 0; // not a directory and/or not initialized, seriously dude?
}

fs_node_t *finddir_fs(fs_node_t *node, char *name) {
	// check if node is a directory AND if it has a callback
	if ((node->flags & 0x7) == FS_DIRECTORY && node->finddir != 0)
		return node->finddir(node, name);
	else return 0; // oops
}