#ifndef EXPLOSIONS_H_
#define EXPLOSIONS_H_

#define NUMEXP 15

typedef struct Explosions {
	Matrix matricies[NUMEXP];
	unsigned short timeBetween[NUMEXP];
	unsigned short timeDuration[NUMEXP];
	unsigned char index;
	unsigned char displayIndex;
} Explosions;

Explosions initExplosions(Explosions explosions) {
	for (int i = 0; i < NUMEXP; i++) {
		clearMatrix(explosions.matricies[i]);
	}
	explosions.index = 0;
	explosions.displayIndex = 0;
	
	return explosions;
}

Explosions pushExplosion(Explosions explosions, Matrix matrix, unsigned short a, unsigned short b) {
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
	d3_setMatrix(explosions.matricies[explosions.displayIndex]);
	explosions.displayIndex++;
}


#endif 