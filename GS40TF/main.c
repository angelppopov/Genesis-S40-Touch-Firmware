/*
 * Created: 2/10/2020 9:33:39 AM
 * Author : Angel Popov
 */
 
#define F_CPU 32000000
#include <avr/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "serial/serial_ble.h"
#include "serial/serial_mcu.h"
#include "touch_panel/touch_panel.h"
#include "handler.h"
#include "scheduler.h"

extern FILE stdout_on_port_e;
extern event_scheduler scheduler;

void init_32_mhz_internal_osc(void);

void init(void)
{
	serial_mcu_init();
	serial_ble_init();
	touch_panel_init();
	scheduler_init();
	wdt_enable(1000);
}

int main(void)
{
	init_32_mhz_internal_osc();
	init();
	
	sei();							/*
										Enables interrupts by setting the global interrupt mask.
									*/
	stdout = &stdout_on_port_e;		/*
										Set default stdout to other MCU on PORTE0
									*/
	printf("Working\n");
    while (1)
	{
		scheduler.process();
		wdt_reset();
	}
}

void init_32_mhz_internal_osc(void){
		OSC_CTRL |= OSC_RC32MEN_bm;						//Setup 32Mhz crystal
		while(!(OSC_STATUS & OSC_RC32MRDY_bm));
		CCP = CCP_IOREG_gc;								//Trigger protection mechanism
		CLK_CTRL = CLK_SCLKSEL_RC32M_gc;				//Enable internal  32Mhz crystal
}