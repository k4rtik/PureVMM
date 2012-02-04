#ifndef SCRN_H
#define SCRN_H

#include "types.h"

void initVideo();
void cls();
void putChar(u8 c);
void putNum(u32 count);
void putHex(u32 hex);
void putStr(const u8 *str);

#endif


