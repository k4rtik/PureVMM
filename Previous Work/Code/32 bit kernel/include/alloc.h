#ifndef ALLOC_H
#define ALLOC_H

#include "types.h"

u32 kalloc(u32 size, int align, u32 *phys);

#endif

