#define TASKSNUM 2

#include <avr/io.h>
#include <avr/eeprom.h>
#include "timer.h"
#include "shiftreg.h"
#include "states.h"
#include "Explosions.h"
#include "USART_1284.h"

unsigned long contClock = 0;
unsigned char displayBlank = 0;

unsigned char numLevels = 0;
unsigned char curLevel = 0;
unsigned char curPattern = 0;

Explosions ExpTick(Explosions explosions);
unsigned char DeathTick();

Explosions explosions;
SingleMatrixUser userMatrix;
SingleMatrix matrix;
SingleMatrix blankMatrix;
SingleMatrix wallMatrix;

unsigned char USARTReceiver;
unsigned char displayLED;
unsigned char displayEDIT;

enum LED_States {LED_SMStart, LED_Wait, LED_Display} LED_State;
void LED_Tick();

enum Edit_States {Edit_SMStart, Edit_Wait, Edit_Display, Edit_Pat} Edit_State;
void Edit_Tick();

void KB_Tick();

void setLevel();
void initNumLevel();

void displayClear();
void displayLevel();
void displayGame();

void kpReceiver();

void sendLevelDetails();


int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	initUSART(0);
	
	initNumLevel();
	initTasks();
	
	TimerSet(1);
	TimerOn();
	
	LED_State = LED_SMStart;
	
	blankMatrix = clearSingleMatrix(blankMatrix);
	explosions = initExplosions(explosions);
	wallMatrix = clearSingleMatrix(wallMatrix);
	
	setLevel();
	
	/*
	unsigned short tempCnter = 501;
	for (unsigned char i = 2; i < 16; i++) {
		eeprom_update_byte((uint8_t*)tempCnter, 0);
		tempCnter += 250;
	}
	*/
	
	/*
	wallMatrix = clearSingleMatrix(wallMatrix);
	
	eeprom_update_byte((uint8_t*)251, 2);
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+252), wallMatrix.m[i]);
	}
	
	matrix = clearSingleMatrix(matrix);
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
	
	// Assign 261-270
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+261), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(269), 35);
	eeprom_update_byte((uint8_t*)(270), 20);
	
	matrix = clearSingleMatrix(matrix);
	alternate = 1;
	for (unsigned char i = 1; i < 7; i++) {
		for (unsigned char j = 0; j < 8; j++) {
			matrix.m[i] = SetBit(matrix.m[i], j, alternate);
			if (alternate == 1) alternate = 0;
			else if (alternate == 0) alternate = 1;
		}
		if (alternate == 1) alternate = 0;
		else if (alternate == 0) alternate = 1;
	}
	
	// Assign 271-280
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+271), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(279), 35);
	eeprom_update_byte((uint8_t*)(280), 20);
	*/
	
	
	
	
	
	
	
	//wallMatrix.m[4] = 0xCC;
	/*
	/ Assign 1 as number of patterns
	eeprom_update_byte((uint8_t*)1, 10);
	
	// Assign 2-9 as wall
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+2), wallMatrix.m[i]);
	}
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 0, 0);
	matrix.m[6] = SetBit(matrix.m[6], 0, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	// Assign 11-20
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+11), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(19), 0);
	  eeprom_update_byte((uint8_t*)(20), 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 1, 0);
	matrix.m[6] = SetBit(matrix.m[6], 1, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	// Assign 21-30
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+21), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(29), 0);
	eeprom_update_byte((uint8_t*)(30), 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 2, 0);
	matrix.m[6] = SetBit(matrix.m[6], 2, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	// Assign 31-40
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+31), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(39), 0);
	eeprom_update_byte((uint8_t*)(40), 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 3, 0);
	matrix.m[6] = SetBit(matrix.m[6], 3, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	// Assign 41-50
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+41), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(49), 0);
	eeprom_update_byte((uint8_t*)(50), 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 4, 0);
	matrix.m[6] = SetBit(matrix.m[6], 4, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	// Assign 51-60
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+51), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(59), 0);
	eeprom_update_byte((uint8_t*)(60), 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 5, 0);
	matrix.m[6] = SetBit(matrix.m[6], 5, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	// Assign 61-70
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+61), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(69), 0);
	eeprom_update_byte((uint8_t*)(70), 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 6, 0);
	matrix.m[6] = SetBit(matrix.m[6], 6, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	// Assign 71-80
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+71), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(79), 0);
	eeprom_update_byte((uint8_t*)(80), 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[1] = SetBit(matrix.m[1], 7, 0);
	matrix.m[6] = SetBit(matrix.m[6], 7, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	// Assign 81-90
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+81), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(89), 0);
	eeprom_update_byte((uint8_t*)(90), 2);
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[2] = SetBit(matrix.m[2], 7, 0);
	matrix.m[5] = SetBit(matrix.m[5], 7, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	// Assign 91-100
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+91), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(99), 0);
	eeprom_update_byte((uint8_t*)(100), 2);
	
	
	matrix = clearSingleMatrix(matrix);
	matrix.m[2] = SetBit(matrix.m[2], 6, 0);
	matrix.m[5] = SetBit(matrix.m[5], 6, 0);
	explosions = pushExplosion(explosions, matrix, 0, 2);
	
	// Assign 101-110
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i+101), matrix.m[i]);
	} eeprom_update_byte((uint8_t*)(109), 0);
	eeprom_update_byte((uint8_t*)(110), 2);
	*/
	/*
	
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
	*/
	
	userMatrix = initSingleUserMatrix(userMatrix);
	
	while (1) {
		LED_Tick();
		Edit_Tick();
		KB_Tick();
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

void Edit_Tick() {
	switch (Edit_State) {
		case Edit_SMStart:
			Edit_State = Edit_Wait;
			break;
			
		case Edit_Wait:
			if (displayEDIT == 0) {
				Edit_State = Edit_Wait;
			} else if (displayEDIT == 1) {
				Edit_State = Edit_Display;
			} else if (displayEDIT == 2) {
				Edit_State = Edit_Pat;
			} 
			break;
			
		case Edit_Display:
			if (displayEDIT == 0) {
				Edit_State = Edit_Wait;
				displayClear();
			} else if (displayEDIT == 1) {
				Edit_State = Edit_Display;
			} else if (displayEDIT == 2) {
				Edit_State = Edit_Pat;
			}
			break;
			
		case Edit_Pat:
			if (displayEDIT == 0) {
				Edit_State = Edit_Wait;
				displayClear();
			} else if (displayEDIT == 1) {
				Edit_State = Edit_Display;
			} else if (displayEDIT == 2) {
				Edit_State = Edit_Pat;
			}
			break;
			
		default:
			break;
	}
	
	switch (Edit_State) {
		case Edit_SMStart:
			break;
		
		case Edit_Wait:
			break;
		
		case Edit_Display:
			displayLevel();
			break;
			
		case Edit_Pat:
			d3_setMatrixColor(explosions.matricies[curPattern].m, RED);
			break;
		
		default:
			break;
	}
}

void LED_Tick() {
	switch (LED_State) {
		case LED_SMStart:
			LED_State = LED_Wait;
			displayLED = 0;
			break;
		
		case LED_Wait:
			if (displayLED) {
				LED_State = LED_Display;
				userMatrix = initSingleUserMatrix(userMatrix);
				explosions.displayIndex = 0;
			} else if (!displayLED) {
				LED_State = LED_Wait;
			}
			break;
		
		case LED_Display:
			 if (displayLED) {
				LED_State = LED_Display;
			} else if (!displayLED) {
				LED_State = LED_Wait; 
				displayClear();
			}
			break;
		
		default:
			break;
	}
	
	switch (LED_State) {
		case LED_SMStart:
			break;
		
		case LED_Wait:
			break;
		
		case LED_Display:
			displayGame();
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

void initNumLevel() {
	unsigned short startingIndex = 1;
	
	while(eeprom_read_byte((uint8_t*)startingIndex) != 0) {
		numLevels++;
		startingIndex += 250;
	}
}

void setLevel() {
	unsigned short startingIndex = curLevel*250;
	unsigned char numPatterns = eeprom_read_byte((uint8_t*)(startingIndex + 1));
	
	for (unsigned char i = 0; i < 8; i++) {
		wallMatrix.m[i] = eeprom_read_byte((uint8_t*)(startingIndex + 2 + i));
	}
	
	explosions = initExplosions(explosions);
	
	unsigned short tempCnt = startingIndex + 11;
	unsigned char timeBetween;
	unsigned char timeDuration;
	for (unsigned char i = 0; i < numPatterns; i++) {
		for (unsigned char j = 0; j < 8; j++) {
			matrix.m[j] = eeprom_read_byte((uint8_t*)(j+ tempCnt));
		}
		timeBetween = eeprom_read_byte((uint8_t*)(tempCnt + 8));
		timeDuration = eeprom_read_byte((uint8_t*)(tempCnt + 9));
		explosions = pushExplosion(explosions, matrix, timeBetween, timeDuration);
		
		tempCnt +=10;
	}
}

void displayLevel() {
	explosions = ExpTick(explosions);
	d3_setMatrixColor(wallMatrix.m, BLUE);
}

void displayGame() {
	displayLevel();
	DeathTick();
	d3_setMatrixColor(userMatrix.m, GREEN);
}

void displayClear() {
	d3_setMatrixColor(blankMatrix.m, GREEN);
	d3_setMatrixColor(blankMatrix.m, BLUE);
	d3_setMatrixColor(blankMatrix.m, RED);
}

void KB_Tick() {
	kpReceiver();
}

void sendLevelDetails() {
	if (USART_IsSendReady(0)) {
		USART_Send(curLevel, 0);	// Send Current Level
	}
	if (USART_IsSendReady(0)) {
		unsigned char numPatterns = eeprom_read_byte((uint8_t*)(curLevel*250 + 1)) | 0x80; // Send Current Level numPatterns
		USART_Send(numPatterns, 0);
	}
}

void sendPatDetails() {
	if (USART_IsSendReady(0)) {
		USART_Send(explosions.timeDuration[curPattern], 0);
	}
	
	if (USART_IsSendReady(0)) {
		USART_Send(explosions.timeBetween[curPattern] | 0x80, 0);
	}
}

void kpReceiver() {
	if (USART_HasReceived(0)) {
		USARTReceiver = USART_Receive(0);
		
		if (USARTReceiver == 0x00) { // Game Up Button
			if (userMatrix.row < 7 && GetBit(wallMatrix.m[userMatrix.row + 1], userMatrix.column)) { // Move if allowed
				userMatrix.row++;
				userMatrix.m[userMatrix.row - 1] = SetBit(userMatrix.m[userMatrix.row - 1], userMatrix.column, 1);
				userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
			}
		}
		
		else if (USARTReceiver == 0x01) { // Game Right Button
			if (userMatrix.column  < 7 && GetBit(wallMatrix.m[userMatrix.row], userMatrix.column + 1)) { // Move if allowed
				userMatrix.column++;
				userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column - 1, 1);
				userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
			}
		}
		
		else if (USARTReceiver == 0x02) { // Game Down Button
			if (userMatrix.row > 0  && GetBit(wallMatrix.m[userMatrix.row - 1], userMatrix.column)) { // Move if allowed
				userMatrix.row--;
				userMatrix.m[userMatrix.row + 1] = SetBit(userMatrix.m[userMatrix.row + 1], userMatrix.column, 1);
				userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
			}
		}
		
		else if (USARTReceiver == 0x03) { // Game Left Button
			if (userMatrix.column > 0 &&  GetBit(wallMatrix.m[userMatrix.row], userMatrix.column - 1)) { // Move if allowed
				userMatrix.column--;
				userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column + 1, 1);
				userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
			}
		}
		
		else if (USARTReceiver == 0x04) { // Game Start Button
			displayLED = 1;
		} 
		
		else if (USARTReceiver == 0x05) { // Game End Button
			displayLED = 0;
		}
		
		else if (USARTReceiver == 0x06) { // Edit Start Button
			displayEDIT = 1;
			sendLevelDetails();
		}
		
		else if (USARTReceiver == 0x07) { // Edit end
			displayEDIT = 0;
			curLevel = 0;
			curPattern = 0;
			setLevel();
		}
		
		else if (USARTReceiver == 0x08 && displayEDIT == 1) { // displayEDIT == 1 && Right pressed
			if (eeprom_read_byte((uint8_t*)(curLevel * 250 + 251)) != 0) { // if there exists a next level
				curLevel++;
				setLevel();
				
				sendLevelDetails();
			}
		}
		else if (USARTReceiver == 0x08 && displayEDIT == 2) { // displayEDIT == 2 && Right pressed
			if (curPattern < explosions.index - 1) {	// if there exists a next pattern
				curPattern++;
				sendPatDetails();
			}
		} 
		
		else if (USARTReceiver == 0x09 && displayEDIT == 1) {
			if (curLevel > 0) {
				curLevel--;
				setLevel();
				
				sendLevelDetails();
			}
		}
		
		else if (USARTReceiver == 0x09 && displayEDIT == 2) {
			if (curPattern > 0) {
				curPattern--;
				sendPatDetails();
			} 
		}
		
		else if (USARTReceiver == 0x0E) { // go from regular editing screen to pattern screen
			displayEDIT = 2;
			sendPatDetails();
		}
		
		else if (USARTReceiver == 0x0F) { // go from pattern screen to regular editing screen
			displayEDIT = 1;
			sendLevelDetails();
		}
	}
}