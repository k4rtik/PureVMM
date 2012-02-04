#ifndef UTIL_H
#define UTIL_H

#include "types.h"

#define REACHED(msg) putStr("Reached ");putStr(msg);putStr(" - Team Virtua!!!\n")
#define TEST putStr("Test Message - Team Virtua!!!\n");

void panic();
void tester();

u8 *memCopy(u8 *dest, const u8 *src, u32 count);
u8 *memSet8(u8 *dest, u8 val, u32 count);
u16 *memSet16(u16 *dest, u16 val, u32 count);
u32 *memSet32(u32 *dest, u32 val, u32 count); 
u32 strlen(const u8 *str);

u8 inportb (u16 _port);
void outportb (u16 _port, u8 _data);

void enableInterrupts();
void disableInterrupts();

#endif

