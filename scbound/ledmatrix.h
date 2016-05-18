#ifndef LEDMATRIX_H
#define LEDMATRIX_H
	
typedef struct Matrix {
	unsigned char red[8][8];
	unsigned char green[8][8];
	unsigned char blue[8][8];
} Matrix;

typedef struct SingleMatrix {
	unsigned char m[8][8];
} SingleMatrix;

typedef struct SingleMatrixUser {
	unsigned char m[8][8];
	unsigned char row;
	unsigned char column;
	
} SingleMatrixUser;


Matrix clearMatrix(Matrix matrix) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			matrix.red[i][j] = 1;
			matrix.green[i][j] = 1;
			matrix.blue[i][j] = 1;
		}
	}
	return matrix;
} 

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