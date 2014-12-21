#include "gui.h"
#include "types.h"
#include "fb.h"
#include "syscall.h"

static uint8** buffer;
static uint32 width;
static uint32 height;

/**
 * 
 */ 
void guiInitialize() {
	FramebufferInitialize();
	width = getWidth();
	height = getHeight();
	buffer = (uint8**) phyAlloc_alloc(sizeof(uint8*)*width);
	int i = 0;
	for(i; i < width; ++i) {
		buffer[i] = (uint8*) phyAlloc_alloc(sizeof(uint8)*height);
	}
}

void guiPainter() {
	
}
