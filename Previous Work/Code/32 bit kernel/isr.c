#include "isr.h"
#include "scrn.h"
#include "util.h"

u8 *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void *interrupt_handlers[256];

void installInterruptHandler(u8 n, fregptr handler) {	
    interrupt_handlers[n] = handler;
}

void clearInterruptHandlers() {
	memSet8((u8 *)&interrupt_handlers, 0, sizeof(fregptr)*256);
}

void IRQHandler(struct regs *r) {
	//REACHED("IRQHandler");
	fregptr handler = interrupt_handlers[r->int_no];
    if (handler) 
		handler(r);
    if (r->int_no >= 40)
        outportb(0xA0, 0x20);
    outportb(0x20, 0x20);
}

void ISRHandler(struct regs *r) {
	//REACHED("ISRHandler");
	fregptr handler = interrupt_handlers[r->int_no];
	if (handler)        
        handler(r);		
    else {		
		putStr("Interrupt No : ");
		putChar(r->int_no);
		putStr(" - ");
		if(r->int_no < 32)
			putStr(exception_messages[r->int_no]);			
		else 
			putStr("Unknown");
		putStr(" Exception. System Halted -- Team Virtua!!!\n");	
		while(1);
	}
}


