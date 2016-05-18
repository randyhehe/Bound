#ifndef STATES_H_
#define STATES_H_

typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[2];

const unsigned long tasksPeriodGCD = 1;

enum KP_States {KP_SMStart, KP_Wait, KP_Pressed};
int KPTick(int state);

enum ETIMER_States {ETIMER_SMStart, ETIMER_Wait};
int ETIMERTick(int state);

void initTasks() {
	tasks[0].state = KP_SMStart;
	tasks[0].period = 20;
	tasks[0].elapsedTime = 20;
	tasks[0].TickFct = &KPTick;

	tasks[1].state = ETIMER_SMStart;
	tasks[1].period = 1;
	tasks[1].elapsedTime =  1;
	tasks[1].TickFct = &ETIMERTick;
}

#endif