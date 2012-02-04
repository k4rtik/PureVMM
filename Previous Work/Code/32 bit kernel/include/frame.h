#ifndef FRAME_H
#define FRAME_H

#include "types.h"

#define NO_FRAMES 0xFFFFFFFF

void initializeFrameBitmap(u32 mem_size);
u32 findFreeFrame();
void setFrame(u32 frame_no);
void clearFrame(u32 frame_no);
u32 isFramed(u32 frame_no);

#endif




