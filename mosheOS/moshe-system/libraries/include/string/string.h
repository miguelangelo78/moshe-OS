#pragma once

#include <type/size_t.h>

extern size_t strlen(const char* str);
extern char *strcpy(char *s1, const char *s2);
extern int strcmp(char* str1, char* str2);
extern char * strcat(char *dst, const char * src);

extern void* memcpy(void *dst, const void *src, size_t count);
extern void* memset(void *dst, char val, size_t count);
extern unsigned short* memsetw(unsigned short *dst, unsigned short val, size_t count);