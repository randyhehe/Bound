#ifndef SHIFTREG_H
#define SHIFTREG_H

#define F_CPU 8000000

#define RED 0
#define BLUE 1
#define GREEN 2

#define DELAYTIME 0.6

#include <avr/io.h>
#include <util/delay.h>
#include "srhelpers.h"

void d3_clearSR() {
	clearSR();
	clearSR();
	clearSR();
}

void d3_setColumn(unsigned char column) {
	COLUMNPORT = 1 << column;
}

void d3_setRow(unsigned char m[8]) {
	for (int i = 0; i < 8; i++) {
		shiftSR(m[i]);
	} latchSR();
}

void d3_setRowMatrix(Matrix matrix, unsigned char column) {
	d3_clearSR();
	
	unsigned char r[8];
	unsigned char g[8];
	unsigned char b[8];
	
	for (int i = 0; i < 8; i++) {
		r[i] = matrix.red[i][column];
		g[i] = matrix.green[i][column];
		b[i] = matrix.blue[i][column];
	}
	
	d3_setRow(g);
	d3_setRow(r);
	d3_setRow(b);
}

void d3_setMatrix(Matrix matrix) {
	for (int i = 0; i < 8; i++) {
		d3_setColumn(i);
		d3_setRowMatrix(matrix, i);
		_delay_ms(DELAYTIME); //set to fix bad lighting. adjust depending on how the program runs
		d3_clearSR();
	}	
}

void d3_setRowMatrixColor(unsigned char m[8][8], unsigned char column, unsigned char color) {
	d3_clearSR();
	
	unsigned char arr[8];
	
	for (unsigned char i = 0; i < 8; i++) {
		arr[i] = m[i][column];
	}
	
	switch (color) {
		case RED:
			clearSR();
			d3_setRow(arr);
			clearSR();
			break;
		case GREEN:
			d3_setRow(arr);
			clearSR();
			clearSR();
			break;
		case BLUE:
			clearSR();
			clearSR();
			d3_setRow(arr);
			break;
		default:
			break;
	}
}

void d3_setMatrixColor(unsigned char m[8][8], unsigned char color) {
	for (unsigned char i = 0; i < 8; i++) {
		d3_setColumn(i);
		d3_setRowMatrixColor(m, i, color);
		_delay_ms(DELAYTIME);
		d3_clearSR();
	}
}
	


#endif
