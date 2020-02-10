/*
 * GS40TF.c
 *
 * Created: 2/10/2020 9:33:39 AM
 * Author : Angel Popov
 */ 

#include <avr/io.h>
#include "serial/serial_ble.h"
#include "serial/serial_mcu.h"
#include "handler.h"

extern struct ev;

void init(void)
{
	// Initialize serial_mcu
	serial_mcu_init();
	// Initialize serial_ble
	serial_ble_init();
}

int main(void)
{
	init();
	
    while (1)
	{
		
	}
}

