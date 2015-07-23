
#define VID_MEMORY	0xB8000

class Test {
public:
	Test() {}
	void DebugClrScr(const unsigned short c) {
		unsigned char* p = (unsigned char*)VID_MEMORY;

		for (int i = 0; i<160 * 30; i += 2) {
			p[i] = 'b';
			p[i + 1] = c;
		}
	}
};

void bar() {
	Test t;


	t.DebugClrScr(0b11000);
}