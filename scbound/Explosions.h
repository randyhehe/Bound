#ifndef EXPLOSIONS_H_
#define EXPLOSIONS_H_

#define NUMEXP 15

#include "ledmatrix.h"

typedef struct Explosions {
	SingleMatrix matricies[NUMEXP];
	unsigned short timeBetween[NUMEXP];
	unsigned short timeDuration[NUMEXP];
	unsigned char index;
	unsigned char displayIndex;
} Explosions;

Explosions initExplosions(Explosions explosions) {
	for (unsigned char i = 0; i < NUMEXP; i++) {
		clearSingleMatrix(explosions.matricies[i]);
	}
	explosions.index = 0;
	explosions.displayIndex = 0;
	
	return explosions;
}

Explosions pushExplosion(Explosions explosions, SingleMatrix matrix, unsigned short a, unsigned short b) {
	explosions.matricies[explosions.index] = matrix;
	explosions.timeBetween[explosions.index] = a;
	explosions.timeDuration[explosions.index] = b;
	explosions.index++;
	
	return explosions;
}

void displayExplosion(Explosions explosions) {
	// Return if no explosions
	if (explosions.index == 0) {
		return;
	}
	
	// Loop back to first explosion
	else if (explosions.displayIndex >= explosions.index) {
		explosions.displayIndex = 0;
	}
	
	// Display explosion
	d3_setMatrixColor(explosions.matricies[explosions.displayIndex].m, RED);
	explosions.displayIndex++;
}


#endif 