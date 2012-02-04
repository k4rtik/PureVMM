#include "util.h"
#include "scrn.h"

void panic()
{
	asm volatile("sti");    
    putStr("Kernel Panic at ");
    putStr(__FILE__);
    putStr(":");
    putNum(__LINE__);
    putStr(" - Team Virtua !!!\n");
    for(;;);
}

void enableInterrupts() {
	asm volatile("sti");
}

void disableInterrupts() {
	asm volatile("cli");
}

u8 *memCopy(u8 *dest, const u8 *src, u32 count) {	
	u8 *temp = dest;
	while(count-- > 0) *temp++ = *src++;
	return dest;
}

u8 *memSet8(u8 *dest, u8 val, u32 count) {
    u8 *temp = dest;
	while(count-- > 0) *temp++ = val;
	return dest;
}

u16 *memSet16(u16 *dest, u16 val, u32 count) {
    u16 *temp = dest;
	while(count-- > 0) *temp++ = val;
	return dest;
}

u32 *memSet32(u32 *dest, u32 val, u32 count) {
    u32 *temp = dest;
	while(count-- > 0) *temp++ = val;
	return dest;
}

u32 strlen(const u8 *str) {
	u32 count = 0;
	while(*str++) count++;
	return count;
}

u8 inportb (u16 _port) {
    u8 rv;
    asm volatile("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (u16 _port, u8 _data) {
    asm volatile("outb %1, %0" : : "dN" (_port), "a" (_data));
}


