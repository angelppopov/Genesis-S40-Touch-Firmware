/*
 * utils.c
 *
 * Created: 2/12/2020 5:26:31 PM
 *  Author: Angel Popov
 */ 
#include <avr/io.h>

void reset(void)
{
	// no real need to store the interrupt context as the reset will pre-empt its restoration
	uint8_t oldInterruptState = SREG;
	// Configuration change protection: allow protected IO regiser write
	CCP = 0xD8;
	// Request software reset by writing to protected IO register
	RST.CTRL = RST_SWRST_bm;
	// Restore interrupts enabled/disabled state (out of common decency - this line will never be reached because the reset will pre-empt it)
	SREG = oldInterruptState;            
}

