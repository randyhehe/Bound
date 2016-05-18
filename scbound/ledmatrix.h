#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include "bit.h"

typedef struct SingleMatrix {
	unsigned char m[8];
} SingleMatrix;

typedef struct SingleMatrixUser {
	unsigned char m[8];
	unsigned char row;
	unsigned char column;
	
} SingleMatrixUser;

SingleMatrix clearSingleMatrix(SingleMatrix singleMatrix) {
	for (unsigned char i = 0; i < 8; i++)
		singleMatrix.m[i] = 0xFF;
	
	return singleMatrix;
}

SingleMatrixUser initSingleUserMatrix(SingleMatrixUser singleMatrix) {
	for (unsigned char i = 0; i < 8; i++)
		singleMatrix.m[i] = 0xFF;
	
	unsigned char row = 0;
	unsigned char col = 3;
	
	singleMatrix.m[row] = SetBit(singleMatrix.m[row], col, 0);
	singleMatrix.row = row;
	singleMatrix.column = col;
	
	return singleMatrix;
}

#endif