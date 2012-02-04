#include "table.h"
#include "types.h"
#include "util.h"
#include "isr.h"

struct gdt_entry gdt[3];
struct gdt_ptr gptr;

struct idt_entry idt[256];
struct idt_ptr iptr;

extern void gdtFlush();
extern void idtFlush();

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

extern void hwtimer();

void remapIRQs();
void setInterruptGates();
void idtSetGate(u8 num, u32 base, u16 sel, u8 flags);
void gdtSetGate(int num, u32 base, u32 limit, u8 access, u8 gran);
void installGDT();
void installIDT();

void installDescriptorTables() {
	installGDT();
	installIDT();
}

void installGDT() {
	gptr.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gptr.base = (u32)&gdt;	
	
	gdtSetGate(0, 0, 0, 0, 0);	
	gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);	
	gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	
	gdtFlush();
}

void gdtSetGate(int num, u32 base, u32 limit, u8 access, u8 gran) {
	gdt[num].base_low = base & 0xFFFF;
	gdt[num].base_mid = (base >> 16) && 0xFF;
	gdt[num].base_high = (base >> 24) && 0xFF;
	
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = (limit >> 16) & 0x0F;
	
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}

void idtSetGate(u8 num, u32 base, u16 sel, u8 flags) {
	idt[num].base_low = base & 0xFFFF;
	idt[num].base_high = (base >> 16) & 0xFFFF;
	
	idt[num].sel = sel;
	idt[num].zero = 0;
	idt[num].flags = flags;	
}

void installIDT() {
	remapIRQs();
	clearInterruptHandlers();

	iptr.limit = sizeof(struct idt_entry) * 256 - 1;
	iptr.base = (u32)&idt;
	
	memSet8((u8*)&idt, 0, sizeof(struct idt_entry) * 256);	
	
	setInterruptGates();
	
	idtFlush();
}

void remapIRQs() {
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

void setInterruptGates() {
    idtSetGate( 0, (u32)isr0 , 0x08, 0x8E);
    idtSetGate( 1, (u32)isr1 , 0x08, 0x8E);
    idtSetGate( 2, (u32)isr2 , 0x08, 0x8E);
    idtSetGate( 3, (u32)isr3 , 0x08, 0x8E);
    idtSetGate( 4, (u32)isr4 , 0x08, 0x8E);
    idtSetGate( 5, (u32)isr5 , 0x08, 0x8E);
    idtSetGate( 6, (u32)isr6 , 0x08, 0x8E);
    idtSetGate( 7, (u32)isr7 , 0x08, 0x8E);
    idtSetGate( 8, (u32)isr8 , 0x08, 0x8E);
    idtSetGate( 9, (u32)isr9 , 0x08, 0x8E);
    idtSetGate(10, (u32)isr10, 0x08, 0x8E);
    idtSetGate(11, (u32)isr11, 0x08, 0x8E);
    idtSetGate(12, (u32)isr12, 0x08, 0x8E);
    idtSetGate(13, (u32)isr13, 0x08, 0x8E);
    idtSetGate(14, (u32)isr14, 0x08, 0x8E);
    idtSetGate(15, (u32)isr15, 0x08, 0x8E);
    idtSetGate(16, (u32)isr16, 0x08, 0x8E);
    idtSetGate(17, (u32)isr17, 0x08, 0x8E);
    idtSetGate(18, (u32)isr18, 0x08, 0x8E);
    idtSetGate(19, (u32)isr19, 0x08, 0x8E);
    idtSetGate(20, (u32)isr20, 0x08, 0x8E);
    idtSetGate(21, (u32)isr21, 0x08, 0x8E);
    idtSetGate(22, (u32)isr22, 0x08, 0x8E);
    idtSetGate(23, (u32)isr23, 0x08, 0x8E);
    idtSetGate(24, (u32)isr24, 0x08, 0x8E);
    idtSetGate(25, (u32)isr25, 0x08, 0x8E);
    idtSetGate(26, (u32)isr26, 0x08, 0x8E);
    idtSetGate(27, (u32)isr27, 0x08, 0x8E);
    idtSetGate(28, (u32)isr28, 0x08, 0x8E);
    idtSetGate(29, (u32)isr29, 0x08, 0x8E);
    idtSetGate(30, (u32)isr30, 0x08, 0x8E);
    idtSetGate(31, (u32)isr31, 0x08, 0x8E);
    idtSetGate(32, (u32)hwtimer, 0x08, 0x8E);
    idtSetGate(33, (u32)irq1, 0x08, 0x8E);
    idtSetGate(34, (u32)irq2, 0x08, 0x8E);
    idtSetGate(35, (u32)irq3, 0x08, 0x8E);
    idtSetGate(36, (u32)irq4, 0x08, 0x8E);
    idtSetGate(37, (u32)irq5, 0x08, 0x8E);
    idtSetGate(38, (u32)irq6, 0x08, 0x8E);
    idtSetGate(39, (u32)irq7, 0x08, 0x8E);
    idtSetGate(40, (u32)irq8, 0x08, 0x8E);
    idtSetGate(41, (u32)irq9, 0x08, 0x8E);
    idtSetGate(42, (u32)irq10, 0x08, 0x8E);
    idtSetGate(43, (u32)irq11, 0x08, 0x8E);
    idtSetGate(44, (u32)irq12, 0x08, 0x8E);
    idtSetGate(45, (u32)irq13, 0x08, 0x8E);
    idtSetGate(46, (u32)irq14, 0x08, 0x8E);
    idtSetGate(47, (u32)irq15, 0x08, 0x8E);		
}


