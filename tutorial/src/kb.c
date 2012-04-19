#include "common.h"
#include "isr.h"
#include "monitor.h"

///////////////////////////////////
u8int key = 0;
/* key is used as a bit checker
 * bit 0 : CAPSLOCK
 * bit 1 : ALT KEY
 * bit 2 : SHIFT KEY
 * bit 3 : CONTROL KEY
 */
/////////////////////////////////

/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish! */
unsigned char kbdus[128] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
	'9', '0', '-', '=', '\b',	/* Backspace */
	'\t',			/* Tab */
	'q', 'w', 'e', 'r',	/* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
	0,			/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
	'\'', '`', 0,		/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',	/* 49 */
	'm', ',', '.', '/', 0,	/* Right shift */
	'*',
	0,			/* Alt */
	' ',			/* Space bar */
	0,			/* Caps lock */
	0,			/* 59 - F1 key ... > */
	0, 0, 0, 0, 0, 0, 0, 0,
	0,			/* < ... F10 */
	0,			/* 69 - Num lock */
	0,			/* Scroll Lock */
	0,			/* Home key */
	0,			/* Up Arrow */
	0,			/* Page Up */
	'-',
	0,			/* Left Arrow */
	0,
	0,			/* Right Arrow */
	'+',
	0,			/* 79 - End key */
	0,			/* Down Arrow */
	0,			/* Page Down */
	0,			/* Insert Key */
	0,			/* Delete Key */
	0, 0, 0,
	0,			/* F11 Key */
	0,			/* F12 Key */
	0,			/* All other keys are undefined */
};

unsigned char kbdus_shifted[128] = {
	0, 27, '!', '@', '#', '$', '%', '^', '&', '*' /* 9 */ ,
	'(', ')', '_', '+', '\b' /* Backspace */ ,
	'\t' /* Tab */ ,
	'Q', 'W', 'E', 'R' /* 19 */ ,
	'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n' /* Enter key */ ,
	24 /* 29   - Control */ ,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':' /* 39 */ ,
	'\"', '~', 16 /*42  Left shift */ ,
	'|', 'Z', 'X', 'C', 'V', 'B', 'N',	/* 49 */
	'M', '<', '>', '?', 22 /* Right shift */ ,
	'*',
	15,			/* 56 Alt */
	' ',			/* 57 Space bar */
	14,			/* 58 Caps lock */
	0,			/* 59 - F1 key ... > */
	0, 0, 0, 0, 0, 0, 0, 0,
	0,			/* < ... F10 */
	0,			/* 69 - Num lock */
	0,			/* Scroll Lock */
	0,			/* Home key */
	0,			/* Up Arrow */
	0,			/* Page Up */
	'-',
	0,			/* Left Arrow */
	0,
	0,			/* Right Arrow */
	'+',
	0,			/* 79 - End key */
	0,			/* Down Arrow */
	0,			/* Page Down */
	0,			/* Insert Key */
	0,			/* Delete Key */
	0, 0, 0,
	0,			/* F11 Key */
	0,			/* F12 Key */
	0,			/* All other keys are undefined */
};

/* Handles the keyboard interrupt */
void keyboard_handler(registers_t regs)
{
	unsigned char scancode;

	/* Read from the keyboard's data buffer */
	scancode = inb(0x60);

	/* If the top bit of the byte we read from the keyboard is
	 *  set, that means that a key has just been released */
	if (scancode & 0x80) {
		/* You can use this one to see if the user released the
		 *  shift, alt, or control keys... */
		if (scancode == 170 || scancode == 182)	// Left shift or right shift released
			key = key & (0xFB);
	} else {
		/* Here, a key was just pressed. Please note that if you
		 *  hold a key down, you will get repeated key press
		 *  interrupts. */

		/* Just to show you how this works, we simply translate
		 *  the keyboard scancode into an ASCII value, and then
		 *  display it to the screen. You can get creative and
		 *  use some flags to see if a shift is pressed and use a
		 *  different layout, or you can add another 128 entries
		 *  to the above layout to correspond to 'shift' being
		 *  held. If shift is held using the larger lookup table,
		 *  you would add 128 to the scancode when you look for it */

		if (scancode == 58)	// caps lock
			key = key ^ 0x1;	// toggle capslock bit (bit 0)

		else if (scancode == 42 || scancode == 54)	// shift key pressed
			key = key | 0x4;	// set shift key bit (bit 2) as 1

		else {

			if (key & 1 << 2) {	// shift key pressed

				if ((key & 1 << 0)) {	// caps key was already pressd
					if (kbdus[scancode] > 96 && kbdus[scancode] < 123)	// it is an alphabet
						monitor_put(kbdus[scancode]);	// insert small letters !
					else
						monitor_put(kbdus_shifted[scancode]);	// inserted shifted characters ( 'A' - 'Z', symbols etc)
				} else
					monitor_put(kbdus_shifted[scancode]);	// insert shifted characters
			}

			else {
				if (key & 0x1) {	//      CAPS KEY bit is ON
					if (kbdus[scancode] > 96 && kbdus[scancode] < 123)	// it is an alphabet
						monitor_put(kbdus[scancode] - 32);	// insert CAPITAL letters
					else	// it is not an alphabet
						monitor_put(kbdus[scancode]);	// insert normal symbols
				} else
					monitor_put(kbdus[scancode]);	// insert normal symbols
			}
		}
	}
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install()
{
	register_interrupt_handler(IRQ1, &keyboard_handler);
}
