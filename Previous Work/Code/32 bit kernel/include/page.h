#ifndef PAGING_H
#define PAGING_H

#include "types.h"

typedef struct page {
    u32 present    : 1;   
    u32 rw         : 1;   
    u32 user       : 1;   
    u32 accessed   : 1;   
    u32 dirty      : 1;   
    u32 unused     : 7;   
    u32 frame      : 20;  
} page;

typedef struct page_table {
    page pages[1024];
} page_table;

typedef struct page_directory {
	page_table *tables[1024];
    u32 table_phys_addrs[1024];
} page_directory;

void initializePaging();

#endif


