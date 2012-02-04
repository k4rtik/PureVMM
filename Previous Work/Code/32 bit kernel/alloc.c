#include "alloc.h"

extern u32 end;
u32 start_addr = (u32)&end;

u32 kalloc(u32 size, int align, u32 *phys) {    
    if (align == 1 && (start_addr & 0xFFFFF000) ) {
        start_addr &= 0xFFFFF000;
        start_addr += 0x1000;
    }
    if (phys) *phys = start_addr;
    u32 tmp = start_addr;
    start_addr += size;
    return tmp;
}

