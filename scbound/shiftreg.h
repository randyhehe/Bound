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
#include "bit.h"

void d3_clearSR() {
	clearSR();
	clearSR();
	clearSR();
}

void d3_setColumn(unsigned char column) {
	COLUMNPORT = 1 << column;
}

void d3_setRow(unsigned char m[8]) {
	for (unsigned char i = 0; i < 8; i++) {
		shiftSR(m[i]);
	} latchSR();
}

void d3_setRowMatrixColor(unsigned char m[8][8], unsigned char column, unsigned char color) {
	d3_clearSR();
	
	unsigned char arr = 0;
	for (unsigned char i = 0; i < 8; i++) {
		arr = SetBit(arr, i, m[i][column]);
	}
	
	switch (color) {
		case RED:
			clearSR();
			shiftWhole(arr);
			clearSR();
			break;
			
		case GREEN:
			shiftWhole(arr);
			clearSR();
			clearSR();
			break;
			
		case BLUE:
			clearSR();
			clearSR();
			shiftWhole(arr);
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
