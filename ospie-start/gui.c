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
			matrix[i][j].red = 0;
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

void drawLine(int x0, int y0, int x1, int y1, int r, int g, int b) {
	int dx = x1 - x0;
	dx = (dx >= 0) ? dx : -dx; // abs()
	int dy = y1 - y0;
	dy = (dy >= 0) ? dy : -dy; // abs()
	int sx;
	int sy;
	if (x0 < x1)
		sx = 1;
	else
		sx = -1;
	if (y0 < y1)
		sy = 1;
	else
		sy = -1;
	int err = dx - dy;
	int e2;
	int done = 0;
	while (!done) {
		put_pixel_RGB24(x0, y0, r, g, b);
		if ((x0 == x1) && (y0 == y1))
			done = 1;
		else {
			e2 = 2 * err;
			if (e2 > -dy) {
				err = err - dy;
				x0 = x0 + sx;
			}
			if (e2 < dx) {
				err = err + dx;
				y0 = y0 + sy;
			}
		}
	}
}

void drawCircle(int x0, int y0, int rayon, int r, int g, int b)
{
	int x = rayon;
	int y = 0;
	int radiusError = 1 - x;

	while(x >= y)
	{
		// top left
		put_pixel_RGB24(-y + x0, -x + y0, r, g, b);
		// top right
		put_pixel_RGB24(y + x0, -x + y0, r, g, b);
		// upper middle left
		put_pixel_RGB24(-x + x0, -y + y0, r, g, b);
		// upper middle right
		put_pixel_RGB24(x + x0, -y + y0, r, g, b);
		// lower middle left
		put_pixel_RGB24(-x + x0, y + y0, r, g, b);
		// lower middle right
		put_pixel_RGB24(x + x0, y + y0, r, g, b);
		// bottom left
		put_pixel_RGB24(-y + x0, x + y0, r, g, b);
		// bottom right
		put_pixel_RGB24(y + x0, x + y0, r, g, b);

		y++;
		if (radiusError < 0) {
			radiusError += 2 * y + 1;
		} else {
			x--;
			radiusError+= 2 * (y - x + 1);
		}
	}
}

void fill_circle(int x0, int y0, int rayon, int r, int g, int b) {
	int x = rayon;
	int y = 0;
	int radiusError = 1 - x;

	while(x >= y)
	{
		// top
		drawLine(-y + x0, -x + y0, y + x0, -x + y0, r, g, b);
		// upper middle
		drawLine(-x + x0, -y + y0, x + x0, -y + y0, r, g, b);
		// lower middle
		drawLine(-x + x0, y + y0, x + x0, y + y0, r, g, b);
		// bottom 
		drawLine(-y + x0, x + y0, y + x0, x + y0, r, g, b);

		y++;
		if (radiusError < 0) {
			radiusError += 2 * y + 1;
		} else {
			x--;
			radiusError+= 2 * (y - x + 1);
		}
	}
}
