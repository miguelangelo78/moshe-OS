#define VID_MEMORY	0xB8000

void DebugClrScr(const unsigned short c) {

	unsigned char* p = (unsigned char*)VID_MEMORY;

	int i;
	for (i = 0; i<160 * 30; i += 2) {

		p[i] = ' ';  /* Need to watch out for MSVC++ optomization memset() call */
		p[i + 1] = c;
	}

}

void kmain(void* MultibootStructure) {
	int i = 0x12;

	DebugClrScr(0x18);
}
