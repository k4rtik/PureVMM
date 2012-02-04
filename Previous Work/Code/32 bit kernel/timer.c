#include "types.h"
#include "timer.h"
#include "isr.h"
#include "scrn.h"

void timerHandler(struct regs *r);

u32 timer_ticks = 0, count = 0;

void installTimer() {
    installInterruptHandler(IRQ0, timerHandler);
}

void timerHandler(struct regs *r) {
    timer_ticks++;
    if (timer_ticks % 500 == 0) {	
		count++;
		putNum(count);
        putStr(" ticks has passed - Team Virtua!!!\n\n\n");
		schedule();
    }
}

void timer_handler()
{
    timer_ticks++;
    if (timer_ticks % 500 == 0) {	
		count++;
		putNum(count);
        putStr(" ticks has passed - Team Virtua!!!\n\n\n");
		//schedule(); NOT DONE
    }
}



