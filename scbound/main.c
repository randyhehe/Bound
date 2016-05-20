#define TASKSNUM 2

#include <avr/io.h>
#include "timer.h"
#include "shiftreg.h"
#include "keypad.h"
#include "states.h"
#include "Explosions.h"
//#include <avr/delay.h>
//#include "USART_1284.h"

unsigned long contClock = 0;
unsigned char displayBlank = 0;

Explosions ExpTick(Explosions explosions);
unsigned char DeathTick();

Explosions explosions;
SingleMatrixUser userMatrix;
SingleMatrix matrix;
SingleMatrix matrix2;
SingleMatrix blankMatrix;


int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	
	initTasks();
	
	TimerSet(1);
	TimerOn();
	
	matrix = clearSingleMatrix(matrix);
	
	/*
	unsigned char alternate = 0;
	for (unsigned char i = 1; i < 7; i++) {
		for (unsigned char j = 0; j < 8; j++) {
			matrix.m[i] = SetBit(matrix.m[i], j, alternate);
			if (alternate == 1) alternate = 0;
			else if (alternate == 0) alternate = 1;
		}
		if (alternate == 1) alternate = 0;
		else if (alternate == 0) alternate = 1;
	}
	
	matrix2 = clearSingleMatrix(matrix2);
	alternate = 1;
	for (unsigned char i = 1; i < 7; i++) {
		for (unsigned char j = 0; j < 8; j++) {
			matrix2.m[i] = SetBit(matrix2.m[i], j, alternate);
			if (alternate == 1) alternate = 0;
			else if (alternate == 0) alternate = 1;
		}
		if (alternate == 1) alternate = 0;
		else if (alternate == 0) alternate = 1;
	}
	
	blankMatrix = clearSingleMatrix(blankMatrix);
	
	explosions = initExplosions(explosions);
	explosions = pushExplosion(explosions, matrix, 350, 200);
	explosions = pushExplosion(explosions, matrix2, 350, 200);
	*/
	
	blankMatrix = clearSingleMatrix(blankMatrix);
	
	explosions = initExplosions(explosions);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 0, 0);
	matrix.m[6] = SetBit(matrix.m[6], 0, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 1, 0);
	matrix.m[6] = SetBit(matrix.m[6], 1, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 2, 0);
	matrix.m[6] = SetBit(matrix.m[6], 2, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 3, 0);
	matrix.m[6] = SetBit(matrix.m[6], 3, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 4, 0);
	matrix.m[6] = SetBit(matrix.m[6], 4, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 5, 0);
	matrix.m[6] = SetBit(matrix.m[6], 5, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 6, 0);
	matrix.m[6] = SetBit(matrix.m[6], 6, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 7, 0);
	matrix.m[6] = SetBit(matrix.m[6], 7, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[2] = SetBit(matrix.m[2], 7, 0);
	matrix.m[5] = SetBit(matrix.m[5], 7, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[2] = SetBit(matrix.m[2], 6, 0);
	matrix.m[5] = SetBit(matrix.m[5], 6, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[2] = SetBit(matrix.m[2], 5, 0);
	matrix.m[5] = SetBit(matrix.m[5], 5, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[2] = SetBit(matrix.m[2], 4, 0);
	matrix.m[5] = SetBit(matrix.m[5], 4, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[2] = SetBit(matrix.m[2], 3, 0);
	matrix.m[5] = SetBit(matrix.m[5], 3, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[2] = SetBit(matrix.m[2], 2, 0);
	matrix.m[5] = SetBit(matrix.m[5], 2, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[2] = SetBit(matrix.m[2], 1, 0);
	matrix.m[5] = SetBit(matrix.m[5], 1, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[2] = SetBit(matrix.m[2], 0, 0);
	matrix.m[5] = SetBit(matrix.m[5], 0, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[3] = 0x00;
	matrix.m[4] = 0x00;
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	userMatrix = initSingleUserMatrix(userMatrix);
	
	while (1) {
		explosions = ExpTick(explosions);
		DeathTick();
		d3_setMatrixColor(userMatrix.m, GREEN);
	}
}

int KPTick(int state) {
	static unsigned char pressedButton;
	
	switch (state) {
		case KP_SMStart:
		state = KP_Wait;
		break;
		
		case KP_Wait:
		pressedButton = GetKeypadKey();
		if (pressedButton == '\0') {
			state = KP_Wait;
			} else if (pressedButton != '\0') {
				if (pressedButton == '4') {
					if (userMatrix.row < 7) {
						userMatrix.row++;
						userMatrix.m[userMatrix.row - 1] = SetBit(userMatrix.m[userMatrix.row - 1], userMatrix.column, 1);
						userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
					}
				} else if (pressedButton == '2') {
					if (userMatrix.column  < 7) {
						userMatrix.column++;
						userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column - 1, 1);
						userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
					}
				} else if (pressedButton == '5') {
					if (userMatrix.row > 0) {
						userMatrix.row--;
						userMatrix.m[userMatrix.row + 1] = SetBit(userMatrix.m[userMatrix.row + 1], userMatrix.column, 1);
						userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
					}
				} else if (pressedButton == '8') {
					if (userMatrix.column > 0) {
						userMatrix.column--;
						userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column + 1, 1);
						userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
					}
				}
			state = KP_Pressed;
		}
		break;
		
		case KP_Pressed:
		if (GetKeypadKey() == '\0') {
			state = KP_Wait;
			} else if (GetKeypadKey() != '\0') {
			state = KP_Pressed;
		}
		break;
		
		default:
		break;
	}
	
	switch (state) {
		case KP_SMStart:
		break;
		
		case KP_Wait:
		break;
		
		default:
		break;
	}
	
	return state;
}

int ETIMERTick(int state) {
	static unsigned short cnt;
	
	switch (state) {
		case ETIMER_SMStart:
			state = ETIMER_Wait;
			cnt = 0;
			break;
		
		case ETIMER_Wait:
			state = ETIMER_Wait;
			break;
		
		default:
			break;
	}
	
	switch (state) {
		case ETIMER_SMStart:
		break;
		
		case ETIMER_Wait:
			cnt++;
			contClock++;
	}
	return state;
}

unsigned char DeathTick() {	
	if ((displayBlank == 0) && GetBit(explosions.matricies[explosions.displayIndex].m[userMatrix.row], userMatrix.column) == 0) {
		userMatrix = initSingleUserMatrix(userMatrix);
		return 1;
	} else {
		// nothing
		return 0;
	}
}

Explosions ExpTick(Explosions explosions) {	
	// Return if empty (Protection code)
	if (explosions.index == 0) {
		return explosions;
	} 
	
	if (explosions.displayIndex >= explosions.index) {
		explosions.displayIndex = 0;
	}
	
	if (displayBlank == 0)
		d3_setMatrixColor(explosions.matricies[explosions.displayIndex].m, RED);
	else if (displayBlank == 1)
		d3_setMatrixColor(blankMatrix.m, RED);
	
	if (displayBlank == 0 && contClock >= (explosions.timeDuration[explosions.displayIndex] * 10)) {
		displayBlank = 1;
		explosions.displayIndex++;
		contClock = 0;
	} else if (displayBlank == 1 && contClock >= (explosions.timeBetween[explosions.displayIndex] * 10)) {
		displayBlank = 0;
		contClock = 0;
	}
	
	return explosions;
}