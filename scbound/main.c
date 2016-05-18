#define TASKSNUM 2

#include <avr/io.h>
#include "timer.h"
#include "shiftreg.h"
#include "keypad.h"
#include "states.h"
#include "Explosions.h"
//#include <avr/delay.h>
//#include "USART_1284.h"

Matrix matrix;
Matrix matrix2;
Matrix blankMatrix;
Matrix userMatrix;

unsigned long contClock = 0;
unsigned short timeBetween = 500;
unsigned char shouldSwitch = 0;
unsigned char displayBlank = 0;

Explosions ExpTick(Explosions explosions);
Matrix UserTick(Matrix matrix);


int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	initTasks();
	
	TimerSet(1);
	TimerOn();
	
	matrix = clearMatrix(matrix);

	unsigned char alternate = 0;
	for (int i = 1; i < 7; i++) {
		for (int j = 0; j < 8; j++) {
			matrix.red[i][j] = alternate;
			if (alternate == 1) alternate = 0;
			else if (alternate == 0) alternate = 1;
		}
		if (alternate == 1) alternate = 0;
		else if (alternate == 0) alternate = 1;
	}
	
	matrix2 = clearMatrix(matrix2);
	alternate = 1;
	for (int i = 1; i < 7; i++) {
		for (int j = 0; j < 8; j++) {
			matrix2.red[i][j] = alternate;
			if (alternate == 1) alternate = 0;
			else if (alternate == 0) alternate = 1;
		}
		if (alternate == 1) alternate = 0;
		else if (alternate == 0) alternate = 1;
	}
	
	blankMatrix = clearMatrix(blankMatrix);
	
	Explosions explosions;
	explosions = initExplosions(explosions);
	explosions = pushExplosion(explosions, matrix, 350, 200);
	explosions = pushExplosion(explosions, matrix2, 350, 200);
	
	userMatrix = clearMatrix(userMatrix);
	userMatrix.green[0][3] = 0;
	
	while (1) {
		//d3_setMatrix(explosions.matricies[0]);
		explosions = ExpTick(explosions);
		d3_setMatrix(userMatrix);
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
			if (cnt <= timeBetween) {
				// Do nothing
				} else if (cnt > timeBetween) {
				if (shouldSwitch == 0) shouldSwitch = 1;
				else if (shouldSwitch == 1) shouldSwitch = 0;
				cnt = 0;
			}
		}
	return state;
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
		d3_setMatrix(explosions.matricies[explosions.displayIndex]);
	else if (displayBlank == 1)
		d3_setMatrix(blankMatrix);
	
	if (displayBlank == 0 && contClock >= explosions.timeDuration[explosions.displayIndex]) {
		displayBlank = 1;
		explosions.displayIndex++;
		contClock = 0;
	} else if (displayBlank == 1 && contClock >= explosions.timeBetween[explosions.displayIndex]) {
		displayBlank = 0;
		contClock = 0;
	}
	
	return explosions;
}