#include "scrn.h"
#include "types.h"
#include "util.h"

u16 *scrn_start;
u8 attrib = 0x0F;
u8 cursor_x = 0, cursor_y = 0;

void scroll() {
	u32 no_rows_shift;
	u16 blank = 0x20 | (attrib << 8);
	if(cursor_y >= 25) {
		no_rows_shift = cursor_y - 25 + 1;
		memCopy((u8 *)scrn_start, (u8 *)scrn_start + no_rows_shift * 80 * 2, (25 - no_rows_shift) * 80 * 2);
		memSet16(scrn_start + 24 * 80, blank, 80);
		cursor_y = 24;
	}	
}

void moveCursor() {
	u16 point = cursor_y * 80 + cursor_x;

	outportb(0x3D4, 14);
    outportb(0x3D5, point >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, point);		
}

void cls() {
	u16 blank = 0x20 | (attrib << 8);
	memSet16(scrn_start, blank, 25*80);
	cursor_x = 0;
	cursor_y = 0;
	moveCursor();
}

void putChar(u8 c) {
	u16 *scrn_point;
	u16 att = attrib << 8;
	
	if(c == 0x08) {
		if(cursor_x != 0) {
			cursor_x--;
			scrn_point = scrn_start + (cursor_y * 80 + cursor_x);		
			*scrn_point = (0x20 | att);			
		}
	}
	else if(c == 0x09) {
		cursor_x += 8;
		if(cursor_x >= 80) cursor_y++;
		cursor_x = ((cursor_x / 8) * 8) % 80;
	}
	else if(c == '\r') cursor_x = 0;
	else if(c == '\n') {
		cursor_x = 0;
		cursor_y++;
	}
	else if(c >= ' ') {
		scrn_point = scrn_start + (cursor_y * 80 + cursor_x);
		*scrn_point = att | (u16)c;
		cursor_x++;
	}
	if(cursor_x >= 80) {
		cursor_x = 0;
		cursor_y++;
	}
	
	scroll();
	moveCursor();
}

void putNum(u32 count) {
	u8 num[15];
	int i = 0;
	if(count == 0) putChar('0');	
	while(count > 0) {
		num[i++] = count % 10 + '0';
		count /= 10;
	}
	while(--i >= 0) {
		putChar(num[i]);
	}
}

void putHex(u32 hex) {
	u8 num[8];
	int i = 0;
	putStr("0x");
	if(hex == 0) putChar('0');
	while(hex > 0) {
		u32 rem = hex % 16;
		if(rem < 10) num[i++] = '0' + rem;
		else num[i++] = 'A' + rem - 10;
		hex /= 16;
	}
	while(--i >= 0) {
		putChar(num[i]);
	}
}

void putStr(const u8 *text) {
	u32 i;
	for(i = 0; i < strlen(text); i++) putChar(text[i]);	
}

void initVideo() {
	cls();
	scrn_start = (u16 *)0xB8000;
}

