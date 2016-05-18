#ifndef LEDMATRIX_H
#define LEDMATRIX_H
	
typedef struct Matrix {
	unsigned char red[8][8];
	unsigned char green[8][8];
	unsigned char blue[8][8];
} Matrix;
	


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

#endif