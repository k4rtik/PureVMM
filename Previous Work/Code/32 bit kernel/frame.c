#include "frame.h"
#include "util.h"
#include "alloc.h" 

u32 *frame_bitmap;
u32 frame_count;

void initializeFrameBitmap(u32 mem_size) {
    frame_count = mem_size / 0x1000;
    frame_bitmap = (u32*)kalloc(frame_count / 32, 0, 0);
    memSet32(frame_bitmap, 0, frame_count / 32);
}

u32 findFreeFrame() {
	u32 i, j;
	for(i = 0; i < frame_count; i++) {
		u32 word = frame_bitmap[i];
		if(word != 0xFFFFFFFF) {
			for(j = 0; j < 32; j++) {
				u32 test = 0x1 << j;
				if(!(word & test)) {
					u32 frame_no = i * 32 + j;
					return frame_no;
				}
			}
		}
	}
	return NO_FRAMES;
}

void setFrame(u32 frame_no) {	
	u32 word_no = frame_no / 32;
	u32 bit_no = frame_no % 32;
	frame_bitmap[word_no] |= 0x1 << bit_no;	
}

void clearFrame(u32 frame_no) {	
	u32 word_no = frame_no / 32;
	u32 bit_no = frame_no % 32;
	frame_bitmap[word_no] &= ~(0x1 << bit_no);	
}

u32 isFramed(u32 frame_no) {
	u32 word_no = frame_no / 32;
	u32 bit_no = frame_no % 32;
	return (frame_bitmap[word_no] & (0x1 << bit_no));	
}

