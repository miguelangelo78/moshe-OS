#include "system\system.h"

void kmain(void* MultibootStructure) {
	system_initialize((multiboot_header_t*)MultibootStructure);
	
	welcome_terminal();

	d_printf("contents of / :\n");
	int i = 0;
	struct dirent * node = 0;
	while ((node = readdir_fs(fs_root, i)) != 0) {
		d_printf("Found file: %s", node->name);
		fs_node_t * fsnode = finddir_fs(fs_root, node->name);

		if ((fsnode->flags & 0x7) == FS_DIRECTORY)
			d_printf("\n\t(directory)\n");
		else {
			char buf[256];
			read_fs(fsnode, 0, 256, (uint8_t*)buf);
			d_printf("\n\t contents: \"%s\"\n", buf);
		}
		i++;
	}

	system_shutdown();
}
