#include "util.h"
#include "scrn.h"
#include "table.h"
#include "timer.h"
#include "kb.h"
#include "types.h"
#include "page.h"

extern u32 end;

int kickoff() {
	initVideo();	
	putStr("Hello World! - Team Virtua!!!\n");
	
 	installDescriptorTables();
	installTimer();
	installKeyboard();	
    initializePaging();    
	putStr("Hello, paging world! - Team Virtua !!!\n");	

	enableInterrupts();	

	
	// u32 end_addr = (u32)&end;
	// u32 *ptr = (u32 *)end_addr;		
	
	// while(1) {
		// putStr("End Address : ");
		// putHex(end_addr);
		// putStr(" ");
		// putHex((u32)ptr);
		// putStr(" : ");
		// putNum(*ptr);
		// putStr("\n");
		// ptr++;	
	// }
			
    putStr("Gotcha!!!\n");

    for (;;);
	return 0;	
}
