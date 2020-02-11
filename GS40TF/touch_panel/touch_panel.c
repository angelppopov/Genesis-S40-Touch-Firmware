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

static volatile char get_touch;

static struct event_linker touch_event = {
	.end_point = touch_detected,
	.id = TOUCH_RECEIVE,
	.out_put = leds,
};

static q_event *emit_event;

extern event_scheduler scheduler;

void touch_panel_init(void){
	/* Register in event system */
	event_register(&touch_event);
	
	emit_event->id = TOUCH_RECEIVE;
	emit_event->data_size = 1;
	/* Setup hardware */
	interruptPortQ();
	led_output;
	PORTQ_OUT |= (1<<PIN3_bm);
	led_toggle;
	led_toggle;
	
	
}

static void touch_detected(char *pin, int size){
	/* This function will be invoked from event system when an event is triggered from interrupt routine */
	*pin = get_touch;
	led_toggle;
}

static void leds(char *data){
	/* This function will be called from event system when other event needs to send data to this object */
	
}


ISR(PORTQ_INT0_vect){
	led_toggle;
	scheduler.add(emit_event);
}

static void interruptPortQ(void){
	// External interrupt 0 on PQ2, enable pull up, sense falling edge
	PORTQ.PIN2CTRL = PORT_OPC_PULLUP_gc;	// pull up
	PORTQ_OUT |= (1<<PIN2_bp);				// set pin high
	PORTQ.PIN2CTRL = PORT_ISC_FALLING_gc;	// configure interrupt
	PORTQ.INT0MASK = PIN2_bm;				// configure interrupt vector
	PORTQ.INTCTRL = PORT_INT0LVL_HI_gc;		// interrupt level pg.150
	

	// Input
	PORTQ.DIRSET &= ~(1<<PIN2_bp);

	// Enable low level interrupts
	PMIC.CTRL = PMIC_LOLVLEN_bm;
}
