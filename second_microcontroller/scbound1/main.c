#include <avr/io.h>
#include "USART_1284.h"
#include "keypad.h"
#include "timer.h"
#include "io.c"

unsigned char isIdle;
unsigned char isAlive;
unsigned char level;
unsigned char lives;
unsigned char resetPressed;
unsigned char isEditing;
unsigned char rowSelected;

unsigned char currentLCDVal1;
unsigned char currentLCDVal2;

void updateBetween(unsigned char i);
void updateDuration(unsigned char i);
void updateDuration(unsigned char i);
void customWriteLCD(unsigned char data);

int main(void)
{
	DDRC = 0xF0; PORTC = 0x0F; 
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	
	initUSART(0);
	initTasks();
	
	TimerSet(10);
	TimerOn();

    while (1) {
		// Do something
    }
}

int KP_Tick(int state) {
	static unsigned char pressedButton;
	
	while(!USART_IsSendReady(0));
	
	switch (state) {
		case KP_SMStart:
		state = KP_Wait;
		break;
		
		case KP_Wait:
		pressedButton = GetKeypadKey();
		if (pressedButton == '\0') {
			state = KP_Wait;
		} 
		
		else if (pressedButton != '\0') {
			if (pressedButton == '4' && (isAlive || isEditing == 3)) { // Game Up
				USART_Send(0x00, 0);
			}
			
			else if (pressedButton == '4' && isEditing == 2) { //edit up/down the time 
					if (rowSelected == 0) {
						USART_Send(0xF0, 0); 
						updateDuration(1);
					} 
					
					else if (rowSelected == 1) {
						USART_Send(0xE0, 0); 
						
						updateBetween(1);
					}
				
			}
			
				
			 
			else if (pressedButton == '2') { // Right
				if (isAlive || isEditing == 3) {	// Game Right
					USART_Send(0x01, 0);
				}
				else if (isEditing) {			// Editing Right
					USART_Send(0x08, 0);
				}
			} 
				
			else if (pressedButton == '5' && (isAlive || isEditing == 3)) { // Game Down
				USART_Send(0x02, 0);
			}
				
			else if (pressedButton == '5' && isEditing == 2) { // edit up/down the time
				if (rowSelected == 0) {
					USART_Send(0xD0, 0);
					updateDuration(0);
				} else if (rowSelected == 1) {
					USART_Send(0xC0, 0); 
					updateBetween(0);
				}
			}
				
				
			else if (pressedButton == '8') { // Game Left
				if (isAlive || isEditing == 3) {
					USART_Send(0x03, 0);
				} else if (isEditing) {			// Editing Left
					USART_Send(0x09, 0);
				}
			} 
			
			else if (pressedButton == 'A' && isIdle) { // Start Playing
				isAlive = 1;
				isIdle = 0;
				USART_Send(0x04, 0);
			}
			
			else if (pressedButton == 'A' && isAlive == 2) {
				isAlive = 0;
			}
			
			else if (pressedButton == 'B') {
				if (isIdle) {		// Start Editing
					isEditing = 1;
					isIdle = 0;			
					USART_Send(0x06, 0);
				} 
				else if (isEditing == 1) {	// stop Editing 
					isEditing = 0;
					isIdle = 1;	
					USART_Send(0x07, 0);
				} 
				else if (isEditing == 2) {	// go back from pattern screen to regular editing screen
					isEditing = 1;
					isIdle = 0;
					USART_Send(0x0F, 0);
				}
				else if (isEditing == 3) {
					isEditing = 2;
					isIdle = 0;
					USART_Send(0x30, 0);
				}
			} 
			else if (pressedButton == 'D') {
				if (isAlive || isEditing) 
					resetPressed = 1;
			} 
			else if (pressedButton == '*') {	// go from regular editing screen to pattern screen
				if (isEditing == 1) {
					isEditing = 2;
					isIdle = 0;
					USART_Send(0x0E, 0);
				} 
				
				else if (isEditing == 2) {
					isEditing = 3;
					isIdle = 0;
					USART_Send(0x0E, 0);
				}
				
				else if (isEditing == 3) {
					USART_Send(0x40, 0);
				}
			}
			
			else if (pressedButton == '0') {
				if (isEditing == 2) {
					if (rowSelected == 1) {
						rowSelected = 0;
						LCD_Cursor(16);
					} else if (rowSelected == 0) {
						rowSelected = 1;
						LCD_Cursor(32);
					}
				}
			} 
			
			else if (pressedButton == '7') {
				USART_Send(0x50, 0);
			}
			
			else if (pressedButton == '#') {
				USART_Send(0x60, 0);
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

int GAME_Tick(int state) {
	unsigned char USARTReceiver;
	
	switch (state) {
		case GAME_SMStart:
			state = GAME_Off;
			break;
			
		case GAME_Off:
			if (!isAlive) {
				state = GAME_Off;
			} else if (isAlive) {
				state = GAME_On;
				lives = 3;
				level = 1;
			}
			break;
			
		case GAME_On:
			if (isAlive == 0) {
				state = GAME_Off;
			} else if (isAlive == 1) {
				state = GAME_On;
			} else if (isAlive == 2) {
				state = GAME_Done;
			}
			break;
			
		case GAME_Done:
			if (isAlive == 0) {
				state = GAME_Off;
				isIdle = 1;
				
			} else if (isAlive == 2) {
				state = GAME_Done;
			}
			break;
			
		default:
			break;
	}
	
	switch (state) {
		case GAME_SMStart:
			break;
			
		case GAME_Off:
			break;
		
		case GAME_On:
			if (USART_HasReceived(0)) {
				USARTReceiver = USART_Receive(0);
				if (USARTReceiver == 0 && lives > 0) {
					lives--;
					LCD_Cursor(16);
					LCD_WriteData(lives + '0');
					
					if (lives == 0) {
						isAlive = 2;
						while (!USART_IsSendReady(0));
						USART_Send(0x05, 0);
						LCD_DisplayString(1, "   Game Over!");
					} 
				} 
				
				if (USARTReceiver == 0x32) {
					level++;
					if (lives < 9) {
						lives++;
						LCD_Cursor(7);
						customWriteLCD(level);
						LCD_Cursor(16);
						LCD_WriteData(lives + '0');
					}
				}
				
				if (USARTReceiver == 0x69) {
					isAlive = 2;
					LCD_Cursor(1);
					LCD_DisplayString(1	, "    You Win!");
				}
			}
			
			if (resetPressed) {
				isAlive = 0;
				isIdle = 1;
				while (!USART_IsSendReady(0));
				USART_Send(0x05, 0);
				resetPressed = 0;
			}
			break;
			
		case GAME_Done:
			if (resetPressed) {
				isAlive = 0;
				isIdle = 1;
				resetPressed = 0;
			}
			break;
		
		default:
			break;
	}
	
	return state;
}

int EDIT_Tick(int state) {
	switch (state) {
		case EDIT_SMStart:
			state = EDIT_Off;
			break;
		case EDIT_Off:
			if (isEditing == 0) {
				state = EDIT_Off;
			} else if (isEditing == 1) {
				state = EDIT_On;
			} else if (isEditing == 2) {
				state = EDIT_Pattern;
			} else if (isEditing == 3) {
				state = EDIT_ChangePat;
			}
			break;
		case EDIT_On:
			if (isEditing == 0) {
				state = EDIT_Off;
			} else if (isEditing == 1) {
				state = EDIT_On;
			} else if (isEditing == 2) {
				state = EDIT_Pattern;
			} else if (isEditing == 3) {
				state = EDIT_ChangePat;
			}
			break;
		case EDIT_Pattern:
			if (isEditing == 0) {
				state = EDIT_Off;
			} else if (isEditing == 1) {
				state = EDIT_On;
			} else if (isEditing == 2) {
				state = EDIT_Pattern;
			} else if (isEditing == 3) {
				state = EDIT_ChangePat;
			}
			break;
			
		case EDIT_ChangePat:
			if (isEditing == 0) {
				state = EDIT_Off;
			} else if (isEditing == 1) {
				state = EDIT_On;
			} else if (isEditing == 2) {
				state = EDIT_Pattern;
			} else if (isEditing == 3) {
				state = EDIT_ChangePat;
			}
			break;
	
		default:
			break;
	}
	
	switch (state) {
		case EDIT_SMStart:
			break;
		case EDIT_Off:
			break;
		case EDIT_On:
			// Do stuff
			if (resetPressed) {
				isEditing = 0;
				isIdle = 1;
				while (!USART_IsSendReady(0)); 
				USART_Send(0x07, 0);
				resetPressed = 0;
			}  
			
			if (USART_HasReceived(0)) {
				unsigned char tempVal = USART_Receive(0);
				if ((tempVal & 0x80) == 0) {
					LCD_ClearScreen();
					LCD_DisplayString(1, "    Level: ");
					customWriteLCD(tempVal + 1);
				} 
				
				if (USART_HasReceived(0)) {
					tempVal = USART_Receive(0);
					if ((tempVal & 0x80) != 0) {
						LCD_Cursor(18);
						LCD_WriteData('P'); LCD_WriteData('a'); LCD_WriteData('t');
						LCD_WriteData('t'); LCD_WriteData('e'); LCD_WriteData('r');
						LCD_WriteData('n'); LCD_WriteData('s'); LCD_WriteData(':');
						LCD_WriteData(' ');
						customWriteLCD(tempVal & 0x7F);
					}
				}
			}
			break;
		case EDIT_Pattern:
			if (resetPressed) {
				isEditing = 0;
				isIdle = 1;
				while (!USART_IsSendReady(0)); 
				USART_Send(0x07, 0);
				resetPressed = 0;
			} 
			
			if (USART_HasReceived(0)) {
				unsigned char tempVal = USART_Receive(0);
				if ((tempVal & 0x80) == 0) {
					currentLCDVal1 = tempVal;
					
					LCD_ClearScreen();
					LCD_DisplayString(1, "Duration: ");
					customWriteLCD(tempVal);
					
					if ((tempVal & 0x7F) == 0) {
						LCD_WriteData(' ');
					}
					else {
						LCD_WriteData('0');
					}
					LCD_WriteData('m'); LCD_WriteData('s');
				} 
				
				if (USART_HasReceived(0)) {
					tempVal = USART_Receive(0);
				
					if ((tempVal & 0x80) != 0) {
						currentLCDVal2 = tempVal & 0x7F;
					
						LCD_Cursor(18);
						LCD_WriteData('B'); LCD_WriteData('e'); LCD_WriteData('t'); 
						LCD_WriteData('w'); LCD_WriteData('e'); LCD_WriteData('e'); 
						LCD_WriteData('n'); LCD_WriteData(':'); LCD_WriteData(' ');
						customWriteLCD(tempVal & 0x7F);
						if ((tempVal & 0x7F) == 0) {
							LCD_WriteData(' ');
						} 
						else {
							LCD_WriteData('0');
						}
						LCD_WriteData('m'); LCD_WriteData('s');
					}
				}
				if (!rowSelected) {
					LCD_Cursor(16);
				} else if (rowSelected) {
					LCD_Cursor(32);
				}
			}
			break;	
			
		case EDIT_ChangePat:
			if (resetPressed) {
				isEditing = 0;
				isIdle = 1;
				while (!USART_IsSendReady(0));
				USART_Send(0x07, 0);
				resetPressed = 0;
			}
			break;
		
		default:
			break;
	}
	
	return state;
}

void updateDuration(unsigned char i) {
	if (i && currentLCDVal1 < 99) {
		currentLCDVal1++;
		LCD_Cursor(11);
		customWriteLCD(currentLCDVal1);
		
		LCD_WriteData('0');
		LCD_WriteData('m');
		LCD_WriteData('s');
	}
	
	else if (!i && currentLCDVal1 > 0) {
		currentLCDVal1--;
		LCD_Cursor(11);
		customWriteLCD(currentLCDVal1);
		
		if (currentLCDVal1 < 1) {
			LCD_WriteData(' ');
			LCD_WriteData('m');
			LCD_WriteData('s');
			LCD_WriteData(' ');
		}
		
		else if (currentLCDVal1 <= 9) {
			LCD_WriteData('0');
			LCD_WriteData('m');
			LCD_WriteData('s');
			LCD_WriteData(' ');
		}
	}
	
	if (!rowSelected) {
		LCD_Cursor(16);
		} else if (rowSelected) {
		LCD_Cursor(32);
	}
}

void updateBetween(unsigned char i) {
	if (i && currentLCDVal2 < 99) {
		currentLCDVal2++;
		LCD_Cursor(27);
		customWriteLCD(currentLCDVal2);
		
		LCD_WriteData('0');
		LCD_WriteData('m');
		LCD_WriteData('s');
	}
	
	else if (!i && currentLCDVal2 > 0) {
		currentLCDVal2--;
		LCD_Cursor(27);
		customWriteLCD(currentLCDVal2);
		
		if (currentLCDVal2 < 1) {
			LCD_WriteData(' ');
			LCD_WriteData('m');
			LCD_WriteData('s');
			LCD_WriteData(' ');
		}
		
		else if (currentLCDVal2 <= 9) {
			LCD_WriteData('0');
			LCD_WriteData('m');
			LCD_WriteData('s');
			LCD_WriteData(' ');
		}
	}
	
	if (!rowSelected) {
		LCD_Cursor(16);
		} else if (rowSelected) {
		LCD_Cursor(32);
	}
}

void customWriteLCD(unsigned char data) {
	if (data < 10) {

	} else if (data < 20) {
		data -= 10;
		LCD_WriteData('1');
	} else if (data < 30) {
		data -= 20;
		LCD_WriteData('2');
	} else if (data < 40) {
		data -= 30;
		LCD_WriteData('3');
	} else if (data <  50) {
		data -= 40;
		LCD_WriteData('4');
	} else if (data < 60) {
		data -= 50;
		LCD_WriteData('5');
	} else if (data < 70) {
		data -= 60;
		LCD_WriteData('6');
	} else if (data < 80) {
		data -= 70;
		LCD_WriteData('7');
	} else if (data < 90) {
		data -= 80;
		LCD_WriteData('8');
	} else if (data < 100) {
		data -= 90;
		LCD_WriteData('9');
	}
	LCD_WriteData(data + '0');
} 

int LCD_Tick(int state) {
	switch (state) {
		case LCD_SMStart:
			isAlive = 0;
			isIdle = 1;
			isEditing = 0;
			state = LCD_Home;
			LCD_init();
			LCD_ClearScreen();
			LCD_DisplayString (1, "   Welcome to        Bound!");
			break;
			
		case LCD_Home:
			if (isAlive) {
				state = LCD_Play;
				LCD_ClearScreen();
				LCD_Cursor(1);
				LCD_WriteData('L'); LCD_WriteData('e');
				LCD_WriteData('v'); LCD_WriteData('e');
				LCD_WriteData('l'); LCD_WriteData(':');
				LCD_WriteData('1');
				LCD_WriteData(' '); LCD_WriteData(' ');
				LCD_WriteData('L'); LCD_WriteData('i');
				LCD_WriteData('v'); LCD_WriteData('e');
				LCD_WriteData('s'); LCD_WriteData(':');
				LCD_WriteData('3');
				break;
			} else if (isEditing) {
				state = LCD_Edit;
			} else {
				state = LCD_Home;
				isAlive = 0;
				isIdle = 1;
				isEditing = 0;
			}
			break;
		
		case LCD_Play:
			if (isAlive) {
				state = LCD_Play;
			} else if (!isAlive) {
				state = LCD_Home;
				isAlive = 0;
				isIdle = 1;
				isEditing = 0;
				LCD_ClearScreen();
				LCD_DisplayString (1, "  Welcome to        Bound!");
			}
			break;
		
		case LCD_End:
			break;
		
		case LCD_Edit:
		if (isEditing) {
			state = LCD_Edit;
			} 
		else if (!isEditing) {
			state = LCD_Home;
			LCD_ClearScreen();
			LCD_DisplayString (1, "  Welcome to        Bound!");
		}
		break;
		
		default:
			break;
		
	}
	
	switch (state) {
		case LCD_SMStart:
			break;
			
		case LCD_Home:
			break;
		
		case LCD_Play:
			break;
		
		case LCD_End:
			break;
		
		case LCD_Edit:
			break;
		
		default:
			break;
		
	}
	
	return state;
}