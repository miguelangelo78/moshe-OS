#pragma once
#include "type\types.h"

#define VID_MEMORY 0xB8000
#define VID_WIDTH 80
#define VID_HEIGHT 25

#define VID_CENTER_W VID_WIDTH/2
#define VID_CENTER_H VID_HEIGHT/2
				
// DECLARE COLORS:
enum VIDColor {
	VIDBlack = 0x0,
	VIDBlue = 0x1,
	VIDGreen = 0x2,
	VIDCyan = 0x3,
	VIDRed = 0x4,
	VIDMagenta = 0x5,
	VIDBrown = 0x6,
	VIDLightGray = 0x7,
	VIDDarkGray = 0x8,
	VIDLightBlue = 0x9,
	VIDLightGreen = 0xA,
	VIDLightCyan = 0xB,
	VIDLightRed = 0xC,
	VIDLightMagenta = 0xD,
	VIDYellow = 0xE,
	VIDWhite = 0xF
};

extern uint8_t * vid_ptr;
const uint8_t VIDCOLOR_DEFAULT = (VIDBlack << 4) | VIDLightGray;