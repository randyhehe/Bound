#ifndef STATES_H_
#define STATES_H_

typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[1];

const unsigned long tasksPeriodGCD = 10;

enum ETIMER_States {ETIMER_SMStart, ETIMER_Wait};
int ETIMERTick(int state);

void initTasks() {
	tasks[0].state = ETIMER_SMStart;
	tasks[0].period = 10;
	tasks[0].elapsedTime =  10;
	tasks[0].TickFct = &ETIMERTick;
}

#endif