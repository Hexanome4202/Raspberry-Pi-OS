#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "types.h"

typedef struct pixel pixel;
struct pixel {
	uint8 red;
	uint8 green;
	uint8 blue;
};

void guiInitilize(void);
void guiPainter(uint32 minX, uint32 maxX, uint32 minY, uint32 maxY);
void addMatrix(uint32 x, uint32 y, pixel** matrix, uint32 sizex, uint32 sizey);
void addWhiteSquare(uint32 x, uint32 y, uint32 sizex, uint32 sizey);
void addBlackSquare(uint32 x, uint32 y, uint32 sizex, uint32 sizey);

#endif
