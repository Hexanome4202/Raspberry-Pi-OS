#include "gui.h"
#include "fb.h"
#include "syscall.h"

static pixel** buffer;
static uint32 width;
static uint32 height;

/**
 * 
 */ 
void guiInitialize() {
	//FramebufferInitialize();
	width = getWidth();
	height = getHeight();
	buffer = (pixel**) phyAlloc_alloc(sizeof(pixel*)*width);
	uint32 i, j;
	for(i=0; i < width; ++i) {
		buffer[i] = (pixel*) phyAlloc_alloc(sizeof(pixel)*height);
		for(j=0; j < height; ++j) {
			buffer[i][j].red = 255;
			buffer[i][j].green = 0;
			buffer[i][j].blue = 255;
		}
	}
}

void guiPainter() {
	uint32 i,j;
	for(i=0; i< width; ++i){
		for(j=0; j<height; ++j){
			pixel pix = buffer[i][j];
			put_pixel_RGB24(i, j, pix.red, pix.green, pix.blue);
		}	
	}
}
