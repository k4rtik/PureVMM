#ifndef GIDT_H
#define GIDT_H

#include "types.h"

struct gdt_entry {
	u16 limit_low;
	u16 base_low;
	u8 base_mid;
	u8 access;
	u8 granularity;
	u8 base_high;
} __attribute__((packed));

struct gdt_ptr {
	u16 limit;
	u32 base;
} __attribute__((packed));

struct idt_entry {
	u16 base_low;
	u16 sel;
	u8 zero;
	u8 flags;
	u16 base_high;
} __attribute__((packed));

struct idt_ptr {
	u16 limit;
	u32 base;
} __attribute__((packed));

void installDescriptorTables();

#endif
