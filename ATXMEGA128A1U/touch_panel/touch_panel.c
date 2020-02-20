/*
 * touch_panel.c
 *
 * Created: 2/10/2020 9:38:50 AM
 *  Author: Angel Popov
 */ 

#include "touch_panel.h"
#include "../handler.h"
#include "../scheduler.h"
#include "../timer/timer.h"
#include "../mem/eeprom.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#define led_output	PORTQ.DIR |= PIN3_bm
#define led_toggle	PORTQ_OUTTGL |= (1<<PIN3_bp)


static volatile char *touch_events;
static uint8_t current_event_read;
static uint8_t events;

extern int target_temp;
extern int current_temp;
extern bool processing_mpu_request;

static struct object touch = {
	.id = TOUCH,
	.input = touch_detected,
	.output = leds,
};

extern volatile event_scheduler scheduler;

void touch_panel_init(void){
	/* Register in event system */
	event_register(&touch);
	printf("Touch registered\n");
	/* Setup hardware */
	interruptPortB();
	init_hardware_interrupts();
	led_output;
}

/*
This function will be invoked from event system when an event is triggered from interrupt routine 
*/

static void touch_detected(char *pin, int size){
	
	/* 
		Check current temperature.
		If is high enough read the event
	*/
	
	if((current_temp  >= (target_temp - 20)) & !processing_mpu_request){
        *pin = *(touch_events + current_event_read);
        current_event_read++;
        if (current_event_read == events) {
            current_event_read = events = 0;
            free(touch_events);
		}
	}else{
		*pin = NULL;
		cpu_relax();
	}
}

/* 
This function will be called from event system when other event needs to send data to this object 
*/

static void leds(char *data){
	
	
}

ISR(PORTB_INT0_vect){
	led_toggle;
	events++;
	if(events > 1){
		touch_events = (char *)realloc(touch_events, sizeof(char) * events);
	}else{
		touch_events = (char *)malloc(sizeof(char));
	}
	/* 
		0x1 = touch 1, 0x2 = touch 2, 0x3 = touch 3, 0x4 = touch 4, 0x5 = touch 5
		0x6 = touch 6, 0x7 = touch 7, 0x8 = touch 8, 0x9 = touch 9, 0xA = touch 10
		0xB = touch 11, 0xC = touch 12
	 */
	*(touch_events + events - 1) = 0x6;
	
	scheduler.add(TOUCH_RECEIVE, 1);
}

static void interruptPortB(void){
	// External interrupt 0 on PB3, enable pull up, sense falling edge
	PORTB.PIN3CTRL = PORT_OPC_PULLUP_gc;	// pull up
	PORTB_OUT |= (1<<PIN3_bp);				// set pin high
	PORTB.PIN3CTRL = PORT_ISC_RISING_gc;	// configure interrupt
	PORTB.INT0MASK = PIN3_bm;				// configure interrupt vector
	PORTB.INTCTRL = PORT_INT0LVL_LO_gc;		// interrupt level pg.150
	
	// Input
	PORTB.DIRSET &= ~(1<<PIN3_bp);

	// Enable low level interrupts
	PMIC.CTRL = PMIC_LOLVLEN_bm;
}

/* Timer Counter */
ISR(TCC0_OVF_vect){
	if (sw)
	{
		led_toggle;
		events++;
		if(events > 1){
			touch_events = (char *)realloc(touch_events, sizeof(char) * events);
		}else{
			touch_events = (char *)malloc(sizeof(char));
		}
	/* 
		0x1 = touch 1, 0x2 = touch 2, 0x3 = touch 3, 0x4 = touch 4, 0x5 = touch 5
		0x6 = touch 6, 0x7 = touch 7, 0x8 = touch 8, 0x9 = touch 9, 0xA = touch 10
		0xB = touch 11, 0xC = touch 12
	 */
		*(touch_events + events - 1) = 0xA;
		//printf("%d\n",*(touch_events + events - 1));
		scheduler.add(TOUCH_RECEIVE, 1);
	}
	
}

/* One sec counter */
ISR(TCC1_OVF_vect){
	
}
