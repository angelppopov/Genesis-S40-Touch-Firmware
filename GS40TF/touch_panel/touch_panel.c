/*
 * touch_panel.c
 *
 * Created: 2/10/2020 9:38:50 AM
 *  Author: Angel Popov
 */ 

#include "touch_panel.h"
#include "../handler.h"
#include "../scheduler.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define led_output	PORTQ.DIR |= PIN3_bm
#define led_toggle	PORTQ_OUTTGL |= (1<<PIN3_bp)


static char *touch_events;
static uint8_t current_event_read;
static uint8_t events;

static struct event_linker touch_event = {
	.id = TOUCH_RECEIVE,
	.input = touch_detected,
	.output = leds,
};

static q_event this_event;

extern event_scheduler scheduler;

void touch_panel_init(void){
	/* Register in event system */
	event_register(&touch_event);
	this_event.id = TOUCH_RECEIVE;
	/* Setup hardware */
	interruptPortB();
	led_output;
	
}

static void touch_detected(char *pin, int size){
	/* This function will be invoked from event system when an event is triggered from interrupt routine */
    *pin = *(touch_events + current_event_read);
	printf("touch: %c\n", *pin);
    current_event_read++;
    if (current_event_read == events) {
	    current_event_read = events = 0;
	    free(touch_events);
    }
}

static void leds(char *data){
	/* This function will be called from event system when other event needs to send data to this object */
	
}

ISR(PORTB_INT0_vect){
	led_toggle;
	events++;
	if(events > 1){
		touch_events = (char *)realloc(touch_events, sizeof(char) * events);
	}else{
		touch_events = (char *)malloc(sizeof(char));
	}
	*(touch_events + events - 1) = '8';
	this_event.data_size = 1;
	
	scheduler.add(&this_event);
}

static void interruptPortB(void){
	// External interrupt 0 on PB3, enable pull up, sense falling edge
	PORTB.PIN3CTRL = PORT_OPC_PULLUP_gc;	// pull up
	PORTB_OUT |= (1<<PIN3_bp);				// set pin high
	PORTB.PIN7CTRL = PORT_ISC_RISING_gc;	// configure interrupt
	PORTB.INT0MASK = PIN3_bm;				// configure interrupt vector
	PORTB.INTCTRL = PORT_INT0LVL_LO_gc;		// interrupt level pg.150
	

	// Input
	PORTB.DIRSET &= ~(1<<PIN7_bp);

	// Enable low level interrupts
	PMIC.CTRL = PMIC_LOLVLEN_bm;
}
