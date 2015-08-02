#include "type\ctype.h"

unsigned short _ctype[257] = {
	0,                      // -1 EOF
	CT_CTRL,               // 00 (NUL)
	CT_CTRL,               // 01 (SOH)
	CT_CTRL,               // 02 (STX)
	CT_CTRL,               // 03 (ETX)
	CT_CTRL,               // 04 (EOT)
	CT_CTRL,               // 05 (ENQ)
	CT_CTRL,               // 06 (ACK)
	CT_CTRL,               // 07 (BEL)
	CT_CTRL,               // 08 (BS)
	CT_SP + CT_CTRL,        // 09 (HT)
	CT_SP + CT_CTRL,        // 0A (LF)
	CT_SP + CT_CTRL,        // 0B (VT)
	CT_SP + CT_CTRL,        // 0C (FF)
	CT_SP + CT_CTRL,        // 0D (CR)
	CT_CTRL,               // 0E (SI)
	CT_CTRL,               // 0F (SO)
	CT_CTRL,               // 10 (DLE)
	CT_CTRL,               // 11 (DC1)
	CT_CTRL,               // 12 (DC2)
	CT_CTRL,               // 13 (DC3)
	CT_CTRL,               // 14 (DC4)
	CT_CTRL,               // 15 (NAK)
	CT_CTRL,               // 16 (SYN)
	CT_CTRL,               // 17 (ETB)
	CT_CTRL,               // 18 (CAN)
	CT_CTRL,               // 19 (EM)
	CT_CTRL,               // 1A (SUB)
	CT_CTRL,               // 1B (ESC)
	CT_CTRL,               // 1C (FS)
	CT_CTRL,               // 1D (GS)
	CT_CTRL,               // 1E (RS)
	CT_CTRL,               // 1F (US)
	CT_SP + CT_WHIT,       // 20 SPACE
	CT_PUNC,                 // 21 !
	CT_PUNC,                 // 22 "
	CT_PUNC,                 // 23 #
	CT_PUNC,                 // 24 $
	CT_PUNC,                 // 25 %
	CT_PUNC,                 // 26 &
	CT_PUNC,                 // 27 '
	CT_PUNC,                 // 28 (
	CT_PUNC,                 // 29 )
	CT_PUNC,                 // 2A *
	CT_PUNC,                 // 2B +
	CT_PUNC,                 // 2C ,
	CT_PUNC,                 // 2D -
	CT_PUNC,                 // 2E .
	CT_PUNC,                 // 2F /
	CT_DIGIT + CT_HEX,            // 30 0
	CT_DIGIT + CT_HEX,            // 31 1
	CT_DIGIT + CT_HEX,            // 32 2
	CT_DIGIT + CT_HEX,            // 33 3
	CT_DIGIT + CT_HEX,            // 34 4
	CT_DIGIT + CT_HEX,            // 35 5
	CT_DIGIT + CT_HEX,            // 36 6
	CT_DIGIT + CT_HEX,            // 37 7
	CT_DIGIT + CT_HEX,            // 38 8
	CT_DIGIT + CT_HEX,            // 39 9
	CT_PUNC,                 // 3A :
	CT_PUNC,                 // 3B ;
	CT_PUNC,                 // 3C <
	CT_PUNC,                 // 3D =
	CT_PUNC,                 // 3E >
	CT_PUNC,                 // 3F ?
	CT_PUNC,                 // 40 @
	CT_UP + CT_HEX,            // 41 A
	CT_UP + CT_HEX,            // 42 B
	CT_UP + CT_HEX,            // 43 C
	CT_UP + CT_HEX,            // 44 D
	CT_UP + CT_HEX,            // 45 E
	CT_UP + CT_HEX,            // 46 F
	CT_UP,                 // 47 G
	CT_UP,                 // 48 H
	CT_UP,                 // 49 I
	CT_UP,                 // 4A J
	CT_UP,                 // 4B K
	CT_UP,                 // 4C L
	CT_UP,                 // 4D M
	CT_UP,                 // 4E N
	CT_UP,                 // 4F O
	CT_UP,                 // 50 P
	CT_UP,                 // 51 Q
	CT_UP,                 // 52 R
	CT_UP,                 // 53 S
	CT_UP,                 // 54 T
	CT_UP,                 // 55 U
	CT_UP,                 // 56 V
	CT_UP,                 // 57 W
	CT_UP,                 // 58 X
	CT_UP,                 // 59 Y
	CT_UP,                 // 5A Z
	CT_PUNC,                 // 5B [
	CT_PUNC,                 // 5C (\) 
	CT_PUNC,                 // 5D ]
	CT_PUNC,                 // 5E ^
	CT_PUNC,                 // 5F _
	CT_PUNC,                 // 60 `
	CT_DOWN + CT_HEX,            // 61 a
	CT_DOWN + CT_HEX,            // 62 b
	CT_DOWN + CT_HEX,            // 63 c
	CT_DOWN + CT_HEX,            // 64 d
	CT_DOWN + CT_HEX,            // 65 e
	CT_DOWN + CT_HEX,            // 66 f
	CT_DOWN,                 // 67 g
	CT_DOWN,                 // 68 h
	CT_DOWN,                 // 69 i
	CT_DOWN,                 // 6A j
	CT_DOWN,                 // 6B k
	CT_DOWN,                 // 6C l
	CT_DOWN,                 // 6D m
	CT_DOWN,                 // 6E n
	CT_DOWN,                 // 6F o
	CT_DOWN,                 // 70 p
	CT_DOWN,                 // 71 q
	CT_DOWN,                 // 72 r
	CT_DOWN,                 // 73 s
	CT_DOWN,                 // 74 t
	CT_DOWN,                 // 75 u
	CT_DOWN,                 // 76 v
	CT_DOWN,                 // 77 w
	CT_DOWN,                 // 78 x
	CT_DOWN,                 // 79 y
	CT_DOWN,                 // 7A z
	CT_PUNC,                 // 7B {
	CT_PUNC,                 // 7C |
	CT_PUNC,                 // 7D }
	CT_PUNC,                 // 7E ~
	CT_CTRL,               // 7F (DEL)
						  // and the rest are 0...
};