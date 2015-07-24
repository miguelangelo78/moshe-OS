#include "stdlib.h"

char tbuf[32];
char bchars[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

void itoa(unsigned i, unsigned radix, char* buff) {
	int pos = 0, opos = 0, top = 0;

	if (i == 0 || radix > 16) {
		buff[0] = '0';
		buff[1] = '\0';
		return;
	}	

	while (i != 0) {
		tbuf[pos] = bchars[i % radix];
		pos++;
		i /= radix;
	}

	top = pos--;

	for (opos = 0; opos < top; pos--, opos++)
		buff[opos] = tbuf[pos];

	buff[opos] = 0;
}

void itoa_s(int i, unsigned radix, char* buff) {
	if (radix > 16) return;
	if (i < 0) {
		*buff++ = '-';
		i *= -1;
	}
	itoa(i, radix, buff);
}