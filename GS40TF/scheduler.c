/*
 * scheduler.c
 *
 * Created: 2/11/2020 10:32:48 AM
 *  Author: Angel Popov
 */ 
#include "scheduler.h"
#include <avr/io.h>


/* Dynamically allocated buffer of events to be executed */
static q_event *event_queue;
static uint8_t q_size;

event_scheduler scheduler;

/* Get first structure from the buffer */
#define  first event_queue[q_size - (q_size + 1)]

void scheduler_init(){
	scheduler.add = append;
	scheduler.process = execute;
}

volatile void append(q_event *ev){
	q_size++;
	if(q_size <= 1){
		/* Allocate memory if it has not been previously allocated */
		event_queue = (q_event *)malloc(sizeof(q_event) * q_size);	
	}else{
		/* Resize the memory block if has been allocated */
		event_queue = (q_event *)realloc(event_queue, sizeof(q_event) * q_size);
	}
}

volatile void execute(){
	event_trigger(first.id, first.data_size);
	
	if(q_size == 0) free(event_queue);
}