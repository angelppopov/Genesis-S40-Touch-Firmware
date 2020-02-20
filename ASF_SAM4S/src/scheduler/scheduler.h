/*
 * scheduler.h
 *
 * Created: 2/20/2020 10:02:50 AM
 *  Author: Angel Popov
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

typedef struct {
	int id;
	int data_size;
}q_event;

typedef volatile void (*f_add)(int id, int data_size);
typedef volatile void (*f_process)();

typedef struct {
	f_add add;
	f_process process;
}event_scheduler;

void scheduler_init();
static volatile void append(int id, int data_size);
static volatile void execute();
static q_event* remove_from_queue();
void cpu_relax();


#endif /* SCHEDULER_H_ */