/*
 * scheduler.c
 *
 * Created: 2/11/2020 10:32:48 AM
 *  Author: Angel Popov
 */ 
#include "scheduler.h"
#include "utils/utils.h"
#include <avr/io.h>
#define F_CPU 32000000
#include <avr/delay.h>
#include <stdbool.h>
#include <stddef.h>


/* Dynamically allocated buffer of events to be executed */
static q_event *event_queue;
static uint8_t q_size;

volatile event_scheduler scheduler;

/* Get first structure from the buffer */
#define queue_is_empty (q_size == 0)

void scheduler_init(){
	scheduler.add = append;
	scheduler.process = execute;
}

volatile void append(q_event *ev){
    q_size++;
    if(q_size <= 1){
	    /* Allocate memory if it has not been previously allocated */
	    event_queue = (q_event *)malloc(sizeof(q_event));
		if(event_queue == NULL) reset();
	    *event_queue = *ev;
	}else{
	    /* Resize the memory block if has been allocated */
	    event_queue = (q_event *)realloc(event_queue, sizeof(q_event) * q_size);
	    if(event_queue == NULL) reset();
	    /* Assign to data to last memory address */
	    *(event_queue + q_size - 1) = *ev;
    }
}

volatile void execute(){
	if(!queue_is_empty){
		cpu_relax();
		/* Process the first event in the event queue */
		bool success = event_trigger(event_queue->id, event_queue->data_size);
		if(success){
            if(q_size > 1){
	            /* Pop the first event when executed */
	            event_queue = remove_from_queue();
	        }else{
	            q_size = 0;
            }
		}
	}else{
		PORTQ_OUTTGL |= (1<<PIN3_bp);
		cpu_relax();
	}
}

static q_event* remove_from_queue(){
	/* Allocate with a size less than the current one */
	q_event *tmp = (q_event *)malloc((q_size - 1) * sizeof(q_event));
	/* Copy everything after the first index */
	if(tmp == NULL) reset();
	memcpy( tmp, (event_queue + 1), (q_size - 1) * sizeof(q_event));
	/* Free event queue when copied to tmp  */
	free(event_queue);
	/* Return tmp to be assigned to event_queue */
	q_size--;
	return tmp;
}

void cpu_relax(){
	_delay_ms(50);
}