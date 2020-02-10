/*
 * GS40TF.c
 *
 * Created: 2/10/2020 9:33:39 AM
 * Author : Angel Popov
 */ 
#define F_CPU 32000000
#include <avr/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "serial/serial_ble.h"
#include "serial/serial_mcu.h"
#include "handler.h"

extern FILE stdout_on_port_e;

#define sw_input_set				PORTQ.DIR &= ~(1<<PIN2_bp)
#define sw_input_pull_up			PORTQ.PIN2CTRL |= PORT_OPC_PULLUP_gc
#define sw_high				PORTQ_OUT |= (1<<PIN2_bp)
#define sw					!(PORTQ.IN & PIN2_bm)
void setup32MhzInternalOsc(void);
void init(void)
{
	// Initialize serial_mcu
	serial_mcu_init();
	// Initialize serial_ble
	serial_ble_init();
	touch_panel_init();
	sw_input_set;
	sw_input_pull_up;
	sw_high;
	
}

int main(void)
{
	setup32MhzInternalOsc();
	init();
	
	sei();							/*
										Enables interrupts by setting the global interrupt mask.
									*/
	stdout = &stdout_on_port_e;		/*
										Set default stdout to MCU on PORTC0
									*/
	printf("Working\n");
    while (1)
	{
		if(sw){
			event_trigger(TOUCH_RECEIVE, 1);
			_delay_ms(500);
		}
	}
}

void setup32MhzInternalOsc(void){
		OSC_CTRL |= OSC_RC32MEN_bm;						//Setup 32Mhz crystal
		while(!(OSC_STATUS & OSC_RC32MRDY_bm));
		CCP = CCP_IOREG_gc;								//Trigger protection mechanism
		CLK_CTRL = CLK_SCLKSEL_RC32M_gc;				//Enable internal  32Mhz crystal
}