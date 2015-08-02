#pragma once

extern unsigned short _ctype[];

/*DEPENDENCIES FOR THE MACROS*/
#define CT_UP 0x01
#define CT_DOWN 0x02
#define CT_DIGIT 0x04
#define CT_CTRL 0x08
#define CT_PUNC 0x10
#define CT_WHIT 0x20
#define CT_HEX 0x40
#define CT_SP 0x80

/*ESSENTIAL MACROS*/
#define isalnum(c)	((_ctype + 1)[(unsigned)(c)] & (CT_UP | CT_DOWN | CT_DIGIT))
#define isalpha(c)	((_ctype + 1)[(unsigned)(c)] & (CT_UP | CT_DOWN))
#define iscntrl(c)	((_ctype + 1)[(unsigned)(c)] & (CT_CTRL))
#define isdigit(c)	((_ctype + 1)[(unsigned)(c)] & (CT_DIGIT))
#define isgraph(c)	((_ctype + 1)[(unsigned)(c)] & (CT_PUNC | CT_UP | CT_DOWN | CT_DIGIT))
#define islower(c)	((_ctype + 1)[(unsigned)(c)] & (CT_DOWN))
#define isprint(c)	((_ctype + 1)[(unsigned)(c)] & (CT_PUNC | CT_UP | CT_DOWN | CT_DIGIT | CT_SP))
#define ispunct(c)	((_ctype + 1)[(unsigned)(c)] & (CT_PUNC))
#define isspace(c)	((_ctype + 1)[(unsigned)(c)] & (CT_WHIT))
#define isupper(c)	((_ctype + 1)[(unsigned)(c)] & (CT_UP))
#define isxdigit(c)	((_ctype + 1)[(unsigned)(c)] & (CT_DIGIT | CT_HEX))
#define isascii(c)	((unsigned)(c) <= 0x7F)
#define toascii(c)	((unsigned)(c) & 0x7F)
#define tolower(c)	(isupper(c) ? c + 'a' - 'A' : c)
#define toupper(c)	(islower(c) ? c + 'A' - 'a' : c)