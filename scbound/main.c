#define TASKSNUM 2

#include <avr/io.h>
#include <avr/eeprom.h>
#include "timer.h"
#include "shiftreg.h"
#include "states.h"
#include "Explosions.h"
//#include <avr/delay.h>
#include "USART_1284.h"

unsigned long contClock = 0;
unsigned char displayBlank = 0;

Explosions ExpTick(Explosions explosions);
unsigned char DeathTick();

Explosions explosions;
SingleMatrixUser userMatrix;
SingleMatrix matrix;
SingleMatrix matrix2;
SingleMatrix blankMatrix;
SingleMatrix wallMatrix;

unsigned char USARTReceiver;
unsigned char displayLCD;

enum LCD_States {LCD_SMStart, LCD_Wait, LCD_Display} LCD_State;
void LCD_Tick();


int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	initUSART(0);
	
	initTasks();
	
	TimerSet(1);
	TimerOn();
	
	LCD_State = LCD_SMStart;
	
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
	wallMatrix = clearSingleMatrix(wallMatrix);
	wallMatrix.m[4] = 0xCC;
	
	while (1) {
		LCD_Tick();
		
		if (USART_HasReceived(0)) {
			USARTReceiver = USART_Receive(0);
			
			if (USARTReceiver == 0x00) { // up
				if (userMatrix.row < 7 && GetBit(wallMatrix.m[userMatrix.row + 1], userMatrix.column)) {
					userMatrix.row++;
					userMatrix.m[userMatrix.row - 1] = SetBit(userMatrix.m[userMatrix.row - 1], userMatrix.column, 1);
					userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
				}
			} else if (USARTReceiver == 0x01) { // right
				if (userMatrix.column  < 7 && GetBit(wallMatrix.m[userMatrix.row], userMatrix.column + 1)) {
					userMatrix.column++;
					userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column - 1, 1);
					userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
				}
			} else if (USARTReceiver == 0x02) { // down
				if (userMatrix.row > 0  && GetBit(wallMatrix.m[userMatrix.row - 1], userMatrix.column)) {
					userMatrix.row--;
					userMatrix.m[userMatrix.row + 1] = SetBit(userMatrix.m[userMatrix.row + 1], userMatrix.column, 1);
					userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
				}
			} else if (USARTReceiver == 0x03) { // left
				if (userMatrix.column > 0 &&  GetBit(wallMatrix.m[userMatrix.row], userMatrix.column - 1)) {
					userMatrix.column--;
					userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column + 1, 1);
					userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
				}
			} else if (USARTReceiver == 0x04) { // Game start
				displayLCD = 1;
			} else if (USARTReceiver == 0x05) {
				displayLCD = 0;
			}
			
		}
	}
	
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
		if (USART_IsSendReady(0)) {
			USART_Send(0x00, 0);
		}
		return 1;
	} else {
		// nothing
		return 0;
	}
}

void LCD_Tick() {
	switch (LCD_State) {
		case LCD_SMStart:
			LCD_State = LCD_Wait;
			displayLCD = 0;
			break;
		
		case LCD_Wait:
			if (displayLCD) {
				LCD_State = LCD_Display;
				userMatrix = initSingleUserMatrix(userMatrix);
			} else if (!displayLCD) {
				LCD_State = LCD_Wait;
			}
			break;
		
		case LCD_Display:
			if (displayLCD) {
				LCD_State = LCD_Display;
				} else if (!displayLCD) {
				LCD_State = LCD_Wait; 
				d3_setMatrixColor(blankMatrix.m, RED);
				d3_setMatrixColor(blankMatrix.m, GREEN);
				d3_setMatrixColor(blankMatrix.m, BLUE);
			}
			break;
		
		default:
			break;
	}
	
	switch (LCD_State) {
		case LCD_SMStart:
			break;
		
		case LCD_Wait:
			break;
		
		case LCD_Display:
			explosions = ExpTick(explosions);
			DeathTick();
			d3_setMatrixColor(userMatrix.m, GREEN);
			d3_setMatrixColor(wallMatrix.m, BLUE);
			break;
		
		default:
			break;
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