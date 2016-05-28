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

unsigned short durationIndex = 0;
unsigned char durationChanged = 0;
unsigned short betweenIndex = 0;
unsigned char betweenChanged = 0;


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

enum Edit_States {Edit_SMStart, Edit_Wait, Edit_Display, Edit_Pat, Edit_ChangePat} Edit_State;
void Edit_Tick();

void KB_Tick();

void setLevel();
void initNumLevel();

void displayClear();
void displayLevel();
void displayGame();

void kpReceiver();

void sendLevelDetails();
void sendPatDetails();

void WinTick();
unsigned char DeathTick();


int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	initUSART(0);
	
	//eeprom_update_byte((uint8_t*)(1), 1);
	
	initNumLevel();
	initTasks();
	
	TimerSet(1);
	TimerOn();
	
	LED_State = LED_SMStart;
	
	blankMatrix = clearSingleMatrix(blankMatrix);
	explosions = initExplosions(explosions);
	wallMatrix = clearSingleMatrix(wallMatrix);
	
	setLevel();
	
	userMatrix = initSingleUserMatrix(userMatrix);
	explosions = initExplosions(explosions);
	matrix = clearSingleMatrix(matrix);
	
	// eeprom_update_byte((uint8_t*)(752), 0);
	
	//eeprom_update_byte((uint8_t*)(519), 0);
	//eeprom_update_byte((uint8_t*)(520), 0);
	
	// eeprom_update_byte((uint8_t*)(251), 1);
	
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

void WinTick() {
	if (userMatrix.row == 7) {
		if (eeprom_read_byte((uint8_t*)(curLevel * 250 + 251)) != 0) {
			curLevel++;
			setLevel();
			userMatrix = initSingleUserMatrix(userMatrix);
			if (USART_IsSendReady(0)) {
				// Send something
			}
		}
		
		//userMatrix = initSingleUserMatrix(userMatrix);
	}
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
				curLevel = 0;
				setLevel();
			} else if (displayEDIT == 2) {
				Edit_State = Edit_Pat;
			} else if (displayEDIT == 3) {
				Edit_State = Edit_ChangePat;
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
			} else if (displayEDIT == 3) {
				Edit_State = Edit_ChangePat;
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
			} else if (displayEDIT == 3) {
				Edit_State = Edit_ChangePat;
			}
			break;
			
		case Edit_ChangePat:
			if (displayEDIT == 0) {
				Edit_State = Edit_Wait;
				displayClear();
			} else if (displayEDIT == 1) {
				Edit_State = Edit_Display;
			} else if (displayEDIT == 2) {
				Edit_State = Edit_Pat;
			} else if (displayEDIT == 3) {
				Edit_State = Edit_ChangePat;
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
			d3_setMatrixColor(wallMatrix.m, BLUE);
			break;
			
		case Edit_ChangePat:
			d3_setMatrixColor(userMatrix.m, GREEN);
			d3_setMatrixColor(explosions.matricies[curPattern].m, RED);
			d3_setMatrixColor(wallMatrix.m, BLUE);
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
				curLevel = 0;
				setLevel();
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
	unsigned short startingIndex = curLevel * 250;
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

void addLevel() {
	if (numLevels >= 10) { // Only 10 levels allowed in the game
		return;
	}
	
	unsigned short startingIndex = 1;
	while (eeprom_read_byte((uint8_t*)startingIndex) != 0) {
		startingIndex += 250;
	}
	
	eeprom_update_byte((uint8_t*)startingIndex, 0x01);	// Initialize to one pattern
	for (unsigned char i = 0; i < 8; i++) {				
		eeprom_update_byte((uint8_t*)(startingIndex + 1 + i), 0xFF);		// Initialize empty wall matrix
		eeprom_update_byte((uint8_t*)(startingIndex + 10 + i), 0xFF);	// Initialize empty pattern
	}
	eeprom_update_byte((uint8_t*)(startingIndex + 18), 0);
	eeprom_update_byte((uint8_t*)(startingIndex + 19), 0);
	
	 numLevels++;	// Increment numLevels
 }

void addPattern() {
	unsigned short startingIndex = curLevel * 250;
	
	unsigned char numPatterns = eeprom_read_byte((uint8_t*)(startingIndex + 1));
	if (numPatterns >= 18) {
		return;
	}
	
	unsigned short tempCnt = startingIndex + 11;
	for (unsigned char i = 0; i < numPatterns; i++) {
		tempCnt += 10;
	}
	
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(i + tempCnt), 0xFF);
	}
	
	// Set duration/between for this new pattern 0;
	eeprom_update_byte((uint8_t*)(8 + tempCnt), 0); 
	eeprom_update_byte((uint8_t*)(9 + tempCnt), 0);
	
	numPatterns++;
	eeprom_update_byte((uint8_t*)(startingIndex + 1), numPatterns);
	
	setLevel();
}

void deletePattern() {
	unsigned short startingIndex = curLevel * 250;
	
	unsigned char numPatterns = eeprom_read_byte((uint8_t*)(startingIndex + 1));
	if (numPatterns <= 1) {
		return;
	}
	
	unsigned short tempCnt = startingIndex + 11;
	for (unsigned char i = 0; i < curPattern; i++) {
		tempCnt += 10;
	}
	
	unsigned char iterations = 0;
	unsigned char tempPatternIndex = curPattern;
	while (tempPatternIndex < numPatterns) {
		for (unsigned char i = 0; i < 8; i++) {
			unsigned char next = eeprom_read_byte((uint8_t*)tempCnt + i + 10);
			eeprom_update_byte((uint8_t*)tempCnt + i, next);
		}
		
		tempPatternIndex++;
		tempCnt += 10;
		iterations++;
	}
	
	numPatterns--;
	eeprom_update_byte((uint8_t*)(startingIndex + 1), numPatterns);
	
	if (iterations == 1) {
		curPattern--;
	}
	
	setLevel();
}

void updatePattern() {
	unsigned short startingIndex = curLevel * 250;
	
	unsigned short tempCnt = startingIndex + 11;
	for (unsigned char i = 0; i < curPattern; i++) {
		tempCnt += 10;
	}
	
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)tempCnt + i, explosions.matricies[curPattern].m[i]);
	}
}

void updateWall() {
	unsigned short startingIndex = curLevel * 250;
	
	for (unsigned char i = 0; i < 8; i++) {
		eeprom_update_byte((uint8_t*)(startingIndex + 2 + i), wallMatrix.m[i]);
	}
}

void editBetween(unsigned char i) {
	// update storage
	unsigned short startingIndex = curLevel * 250;
	
	unsigned short tempCnt = startingIndex + 19;
	for (unsigned char i = 0; i < curPattern; i++) {
		tempCnt += 10;
	}
	
	if (i) {
		explosions.timeBetween[curPattern]++;
		} else {
		explosions.timeBetween[curPattern]--;
	}

	//eeprom_update_byte((uint8_t*)tempCnt, timeBetween);
	betweenIndex = tempCnt;
	betweenChanged = 1;
	
	sendPatDetails();
} 

void editDuration(unsigned char i) {
	// update storage
	unsigned short startingIndex = curLevel * 250;
	
	unsigned short tempCnt = startingIndex + 20;
	for (unsigned char i = 0; i < curPattern; i++) {
		tempCnt += 10;
	}
	
	if (i) {
		explosions.timeDuration[curPattern]++;
	} else {
		explosions.timeDuration[curPattern]--;
	}

	//eeprom_update_byte((uint8_t*)tempCnt, timeDuration);
	durationIndex = tempCnt;
	durationChanged = 1;
	
	sendPatDetails();
	
}

void displayLevel() {
	explosions = ExpTick(explosions);
	d3_setMatrixColor(wallMatrix.m, BLUE);
}

void displayGame() {
	displayLevel();
	DeathTick();
	WinTick();
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

void storePattern() {
	if (betweenChanged) {
		eeprom_update_byte((uint8_t*)betweenIndex, explosions.timeBetween[curPattern]);
		betweenChanged = 0;
	}
	
	if (durationChanged) {
		eeprom_update_byte((uint8_t*)durationIndex, explosions.timeDuration[curPattern]);
		durationChanged = 0;
	}
}

void kpReceiver() {
	if (USART_HasReceived(0)) {
		USARTReceiver = USART_Receive(0);
		
		if (USARTReceiver == 0x00) { // Game Up Button	
			if (userMatrix.row < 7 && (displayEDIT == 3 || GetBit(wallMatrix.m[userMatrix.row + 1], userMatrix.column))) { // Move if allowed
					userMatrix.row++;
					userMatrix.m[userMatrix.row - 1] = SetBit(userMatrix.m[userMatrix.row - 1], userMatrix.column, 1);
					userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
			}
		}
		
		else if (USARTReceiver == 0x01) { // Game Right Button
			if (userMatrix.column < 7 && (displayEDIT == 3 || GetBit(wallMatrix.m[userMatrix.row], userMatrix.column + 1))) {
					userMatrix.column++;
					userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column - 1, 1);
					userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
			}
		}
		
		else if (USARTReceiver == 0x02) { // Game Down Button
			if (userMatrix.row > 0  && (displayEDIT == 3 || GetBit(wallMatrix.m[userMatrix.row - 1], userMatrix.column))) { // Move if allowed
				userMatrix.row--;
				userMatrix.m[userMatrix.row + 1] = SetBit(userMatrix.m[userMatrix.row + 1], userMatrix.column, 1);
				userMatrix.m[userMatrix.row] = SetBit(userMatrix.m[userMatrix.row], userMatrix.column, 0);
			}
		}
		
		else if (USARTReceiver == 0x03) { // Game Left Button
			if (userMatrix.column > 0 &&  (displayEDIT == 3 || GetBit(wallMatrix.m[userMatrix.row], userMatrix.column - 1))) { // Move if allowed
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
			storePattern();
			
			displayEDIT = 0;
			curLevel = 0;
			curPattern = 0;
			setLevel();
			explosions.displayIndex = 0;
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
				storePattern();
				
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
				storePattern();
				
				curPattern--;
				sendPatDetails();
			} 
		}
		
		else if (USARTReceiver == 0x0E && displayEDIT == 1) { // go from regular editing screen to pattern screen
			displayEDIT = 2;
			durationChanged = 0;
			betweenChanged = 0;
			sendPatDetails();
		}
		
		else if (USARTReceiver == 0x0E && displayEDIT == 2) {
			storePattern();
			displayEDIT = 3;
		}
		
		else if (USARTReceiver == 0x50 && displayEDIT == 2) { // '7' pressed when at pattern screen
			addPattern();
		}
		
		else if (USARTReceiver == 0x50 && displayEDIT == 1) { // '7' pressed when at level screen
			addLevel();
			//initNumLevel();
		}
		
		else if (USARTReceiver == 0x60 && displayEDIT == 2) {
			deletePattern();
		}
		
		else if (USARTReceiver == 0x30) {
			displayEDIT = 2;
			sendPatDetails();
			// update the current explosion
			updatePattern();
			updateWall();
		}
 		
		else if (USARTReceiver == 0x0F) { // go from pattern screen to regular editing screen
			storePattern();
			displayEDIT = 1;
			sendLevelDetails();
			explosions.displayIndex = 0;
			curPattern = 0;
		}
		
		else if (USARTReceiver == 0xF0) {
			// Up the current thing if less than 99 (cant get to 100)
			// Make sure to update the eeprom bs
			// duration up
			
			if (explosions.timeDuration[curPattern] < 99) {
				editDuration(1);
			}
		}
		
		else if (USARTReceiver == 0xE0) {
			// Lower the current thing if greater than 0 (cant be less than 0)
			// Make sure to update the eeprom bs
			// between up
			if (explosions.timeBetween[curPattern] < 99) {
				editBetween(1);
			}
		}
		
		else if (USARTReceiver == 0xD0) {
			// Lower the current thing if greater than 0 (cant be less than 0)
			// Make sure to update the eeprom bs
			// duration down
			
			if (explosions.timeDuration[curPattern] > 0) {
				editDuration(0);
			}
		}
		
		else if (USARTReceiver == 0xC0) {
			// Lower the current thing if greater than 0 (cant be less than 0)
			// Make sure to update the eeprom bs
			// between down
			if (explosions.timeBetween[curPattern] > 0) {
				editBetween(0);
			}
		}
		
		else if (USARTReceiver == 0x40 && displayEDIT == 3) {
			if (!GetBit(explosions.matricies[curPattern].m[userMatrix.row], userMatrix.column)) {
				// MAKE SURE TO EDIT EEPROM SHIT
				explosions.matricies[curPattern].m[userMatrix.row] = SetBit(explosions.matricies[curPattern].m[userMatrix.row], userMatrix.column, 1);
				wallMatrix.m[userMatrix.row] = SetBit(wallMatrix.m[userMatrix.row], userMatrix.column, 0);
			} 
			else if (!GetBit(wallMatrix.m[userMatrix.row], userMatrix.column)) {
				wallMatrix.m[userMatrix.row] = SetBit(wallMatrix.m[userMatrix.row], userMatrix.column, 1);
			}
			
			else {
				// MAKE SURE TO EDIT EEPROM SHIT
				explosions.matricies[curPattern].m[userMatrix.row] = SetBit(explosions.matricies[curPattern].m[userMatrix.row], userMatrix.column, 0);
			}
		}
	}
}