/*
 * touch_panel.c
 *
 * Created: 2/25/2020 11:39:55 AM
 *  Author: Angel Popov
 */ 
#include <asf.h>
#include <stddef.h>
#include "touch_panel.h"
#include "../scheduler/handler.h"
#include "../scheduler/scheduler.h"
#include "../memory/eeprom.h"

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
static void pin_edge_handler(const uint32_t id, const uint32_t index);


void touch_panel_init(void){
	/* Register in event system */
	event_register(&touch);
	printf("Touch registered\n");
	/* Setup hardware */
	enable_interrupt();
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


static void enable_interrupt(void){
	
	pmc_enable_periph_clk(ID_PIOA);												// Enable the module clock to the PIOA peripheral
	pio_set_output(PIOA, PIO_PA3, LOW, DISABLE, ENABLE);						// Set pin 23 direction on PIOA as output, default low level
	pio_set_input(PIOA, PIO_PA2, PIO_PULLUP);									// Set pin 24 direction on PIOA as input, with pullup
	pio_handler_set(PIOA, ID_PIOA, PIO_PA2, PIO_IT_RISE_EDGE, pin_edge_handler);	// Configure the input pin 24 interrupt mode and handler
	pio_enable_interrupt(PIOA, PIO_PA2);										// Enable the interrupt for the configured input pin
	NVIC_EnableIRQ(PIOA_IRQn);													// Enable interrupt handling from the PIOA module

}

static void pin_edge_handler(const uint32_t id, const uint32_t index)
{
	
// 	if ((id == ID_PIOC) && (index == PIO_PC24)){
// 		if (pio_get(PIOA, PIO_TYPE_PIO_INPUT, PIO_PC24)){
// 			pio_clear(PIOA, PIO_PA3);
// 		}else{
// 			pio_set(PIOA, PIO_PA3);
// 		}
// 	}
	
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