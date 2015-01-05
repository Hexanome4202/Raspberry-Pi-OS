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
			buffer[i][j].red = 0;
			buffer[i][j].green = 0;
			buffer[i][j].blue = 0;
		}
	}
}

void guiPainter(uint32 minX, uint32 maxX, uint32 minY, uint32 maxY) {
	uint32 i,j;
	for(i=minX; i< maxX+1; ++i){
		for(j=minY; j<maxY+1; ++j){
			pixel pix = buffer[i][j];
			put_pixel_RGB24(i, j, pix.red, pix.green, pix.blue);
		}	
	}
}

void addMatrix(uint32 x, uint32 y, pixel** matrix, uint32 sizex, uint32 sizey) {
	if(x < width && y < height && x >= 0 && y >= 0) {
		uint32 i, j, z, w;
		z = x;
		for(i = 0; i < sizex; ++i) {
			w = y;			
			for(j = 0; j < sizey; ++j) {
				buffer[z][w++] = matrix[i][j];
			}
			++z;
		}
	}
}

void addWhiteSquare(uint32 x, uint32 y, uint32 sizex, uint32 sizey){
	pixel** matrix = (pixel**) phyAlloc_alloc(sizeof(pixel*)*sizex);
	uint32 i,j;
	for(i=0; i< sizex; ++i){
		matrix[i] = (pixel*) phyAlloc_alloc(sizeof(pixel)*sizey);
		for(j=0; j<sizey; ++j){
			matrix[i][j].red = 255;
			matrix[i][j].green = 255;
			matrix[i][j].blue = 255;
		}
	}
	addMatrix(x,y,matrix,sizex,sizey);
}

void addBlackSquare(uint32 x, uint32 y, uint32 sizex, uint32 sizey){
	pixel** matrix = (pixel**) phyAlloc_alloc(sizeof(pixel*)*sizex);
	uint32 i,j;
	for(i=0; i< sizex; ++i){
		matrix[i] = (pixel*) phyAlloc_alloc(sizeof(pixel)*sizey);
		for(j=0; j<sizey; ++j){
			matrix[i][j].red = 0;
			matrix[i][j].green = 0;
			matrix[i][j].blue = 0;
		}
	}
	addMatrix(x,y,matrix,sizex,sizey);
}
