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
	TCC0.CNT = 0;
	TCC0.PER = 0x7FFF;
	TCC0.CTRLA = TC_CLKSEL_DIV1024_gc;       
	TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
	TCC0.INTFLAGS = 0x01;
	TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
	
	TCC1.CNT = 0;
	TCC1.PER = 0x7FFF;
	TCC1.CTRLA = TC_CLKSEL_DIV1024_gc;
	TCC1.INTCTRLA = TC_OVFINTLVL_LO_gc;
	TCC1.INTFLAGS = 0x01;
	TCC1.CTRLB = TC_WGMODE_NORMAL_gc;
	
	// Enable low level interrupts
	PMIC.CTRL = PMIC_LOLVLEN_bm;
	
	sw_set;
	sw_pull_up;
	sw_high;
}
