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
void guiPainter(void);

#endif
