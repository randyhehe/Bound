#ifndef LEDMATRIX_H
#define LEDMATRIX_H

typedef struct SingleMatrix {
	unsigned char m[8][8];
} SingleMatrix;

typedef struct SingleMatrixUser {
	unsigned char m[8][8];
	unsigned char row;
	unsigned char column;
	
} SingleMatrixUser;

SingleMatrix clearSingleMatrix(SingleMatrix singleMatrix) {
	for (unsigned char i = 0; i < 8; i++) {
		for (unsigned char j = 0; j < 8; j++) {
			singleMatrix.m[i][j] = 1;
		}
	}
	
	return singleMatrix;
}

SingleMatrixUser initSingleUserMatrix(SingleMatrixUser singleMatrix) {
	for (unsigned char i = 0; i < 8; i++) {
		for (unsigned char j = 0; j < 8; j++) {
			singleMatrix.m[i][j] = 1;
		}
	}
	
	unsigned char row = 0;
	unsigned char col = 3;
	
	singleMatrix.m[row][col] = 0;
	singleMatrix.row = row;
	singleMatrix.column = col;
	
	return singleMatrix;
}

#endif