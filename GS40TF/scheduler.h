/*
 * scheduler.h
 *
 * Created: 2/11/2020 10:33:06 AM
 *  Author: Angel Popov
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

typedef struct q_event{
	int id;
	int data_size;
}q_event;

typedef volatile void (*f_add)(q_event *ev);
typedef volatile void (*f_process)();

typedef struct event_scheduler{
	f_add add;
	f_process process;
}event_scheduler;

void scheduler_init();
volatile void append(q_event *ev);
volatile void execute();

#endif /* SCHEDULER_H_ */