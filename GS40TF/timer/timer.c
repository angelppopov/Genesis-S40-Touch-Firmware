/*
 * timer.c
 *
 * Created: 2/12/2020 4:42:02 PM
 *  Author: Angel Popov
 */ 
#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void init_hardware_interrupts(void){
	TCC0.CNT = 0;                            // Zeroise count
	TCC0.PER = 0x7FFF;                       // Period
	
	TCC0.CTRLA = TC_CLKSEL_DIV1024_gc;       // Divider
	TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;      // Low level interrupt
	TCC0.INTFLAGS = 0x01;                    // Clear any initial interrupt flags
	TCC0.CTRLB = TC_WGMODE_NORMAL_gc;        // Normal operation
	// Enable low level interrupts
	
	PMIC.CTRL = PMIC_LOLVLEN_bm;
		sw_set;
		sw_pull_up;
		sw_high;
}
