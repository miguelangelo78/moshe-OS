#include <string/string.h>

size_t strlen(const char * str) {
	size_t len = 0;
	while (str[len++]);
	return len;
}

int strcmp(char* str1, char* str2) {
	int i = 0;
	int failed = 0;
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			failed = 1;
			break;
		}
		i++;
	}

	if ((str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0'))
		failed = 1;

	return failed;
}

char *strcpy(char *s1, const char *s2) {
	char *s1_old = s1;
	while ((*(s1++) = *(s2++)));
	return s1_old;
}

char * strcat(char *dst, const char * src) {
	while (*dst) dst++;
	
	do *dst++ = *src++; while (*src!=0);
	return dst;
}

void* memcpy(void *dst, const void *src, size_t count) {
	const char *sp = (const char*)src;
	char *dp = (char *)dst;
	for (; count != 0; count--) *dp++ = *sp++;
	return dst;
}

void* memset(void *dst, char val, size_t count) {
	unsigned char * tmp = (unsigned char *)dst;
	for (; count != 0; count--, tmp[count] = val);
	return dst;
}

unsigned short* memsetw(unsigned short *dst, unsigned short val, size_t count) {
	unsigned short *tmp = (unsigned short *)dst;
	for (; count != 0; count--)
		*tmp++ = val;
	return dst;
}