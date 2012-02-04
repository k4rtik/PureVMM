#include "page.h"
#include "isr.h"
#include "util.h"
#include "types.h"
#include "scrn.h"
#include "frame.h"
#include "alloc.h"

extern u32 start_addr;
page_directory *kernel_pdp = 0;
page_directory *current_pdp = 0;

void setupKernelPageDirectory();
void switchPageDirectory(page_directory *pdp);
void allocPageFrame(page *p, int is_kernel, int is_writable);
void freePageFrame(page *p);
page *getPage(u32 addr, int create, page_directory *pdp);
void pageFaultHandler(struct regs *r);

void initializePaging() {
    initializeFrameBitmap(0x1000000);
	setupKernelPageDirectory();
    installInterruptHandler(14, pageFault	Handler);
    switchPageDirectory(kernel_pdp);
}

void setupKernelPageDirectory() {
    kernel_pdp = (page_directory*)kalloc(sizeof(page_directory), 1, 0);
    int i = 0;
    while (i < start_addr)     {        
        allocPageFrame(getPage(i, 1, kernel_pdp), 1, 0);
        i += 0x1000;
    }
}	

void allocPageFrame(page *p, int is_kernel, int is_writable) {
	if(p->frame == 0) {
		u32 frame_no = findFreeFrame();
		if(frame_no == NO_FRAMES) {
			putStr("No free frames!");
			panic();
		}
		setFrame(frame_no);
        p->present = 1;
        p->rw = (is_writable)?1:0;
        p->user = (is_kernel)?0:1;
        p->frame = frame_no;		
	}
}

void freePageFrame(page *p) {
    u32 frame_no = p->frame;
    if (frame_no) {
        clearFrame(frame_no);
        p->frame = 0x0;
    }
}

void switchPageDirectory(page_directory *pdp) {
    current_pdp = pdp;
    asm volatile("mov %0, %%cr3":: "r"(pdp->table_phys_addrs));
    u32 cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; 
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page *getPage(u32 addr, int create, page_directory *pdp) {
    u32 page_no = addr / 0x1000;
    u32 table_no = page_no / 1024;
    if (pdp->tables[table_no]) return &(pdp->tables[table_no]->pages[page_no%1024]);
    else if(create) {
        u32 phys;
        pdp->tables[table_no] = (page_table*)kalloc(sizeof(page_table), 1, &phys);
        pdp->table_phys_addrs[table_no] = phys | 0x7;
        return &(pdp->tables[table_no]->pages[page_no%1024]);
    }
    else return 0;
}

void pageFaultHandler(struct regs *r) {
    u32 fault_addr;
    asm volatile("mov %%cr2, %0" : "=r" (fault_addr));
        
    int present   = !(r->err_code & 0x1); 
    int rw = r->err_code & 0x2;           
    int us = r->err_code & 0x4;           
    int reserved = r->err_code & 0x8;     
    int id = r->err_code & 0x10;          

    putStr("Page fault! ( ");
    if (present) putStr("present ");
    if (rw) putStr("read-only ");
    if (us) putStr("user-mode ");
    if (reserved) putStr("reserved ");
	if(id) putStr("Instruction Fetch ");
    putStr(") at location : ");
    putHex(fault_addr);
    putStr("\n");
	panic();
}


