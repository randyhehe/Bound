#ifndef STATES_H_
#define STATES_H_

typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[4];
const unsigned long tasksPeriodGCD = 1;

enum KP_States {KP_SMStart, KP_Wait, KP_Pressed};
int KP_Tick(int state);

enum LCD_States {LCD_SMStart, LCD_Home, LCD_Play, LCD_End, LCD_Edit};
int LCD_Tick(int state);

enum GAME_States {GAME_SMStart, GAME_Off, GAME_On, GAME_Done};
int GAME_Tick(int state);

enum LED_States {LED_SMStart, LED_Wait, LED_Display};
int LED_Tick(int state);

enum EDIT_States {EDIT_SMStart, EDIT_Off, EDIT_On, EDIT_Pattern, EDIT_ChangePat};
int EDIT_Tick(int state);

void initTasks() {
	tasks[0].state = KP_SMStart;
	tasks[0].period = 1;
	tasks[0].elapsedTime = 1;
	tasks[0].TickFct = &KP_Tick;
	
	tasks[1].state = LCD_SMStart;
	tasks[1].period = 20;
	tasks[1].elapsedTime = 0;
	tasks[1].TickFct = &LCD_Tick;
	
	tasks[2].state = GAME_SMStart;
	tasks[2].period = 20;
	tasks[2].elapsedTime = 20;
	tasks[2].TickFct = &GAME_Tick;
	
	tasks[3].state = EDIT_SMStart;
	tasks[3].period = 20;
	tasks[3].elapsedTime = 20;
	tasks[3].TickFct = &EDIT_Tick;
}

#endif /* STATES_H_ */