/*
 * sd_card.c
 *
 * Created: 2/20/2020 12:38:18 PM
 *  Author: Angel Popov
 */ 

#include "sd_card.h"
#include <asf.h>
#include <string.h>

void sd_card_init(void){

	sysclk_enable_peripheral_clock(ID_PIOA);							//Enable PIOA clock to detect Card Detect (CD) change
	pio_set_peripheral(PIOA, PIO_PERIPH_C,								//Configure SD card pins as per MCI peripheral (periph C)
	PIO_PA30C_MCDA0 |        //MCI Data 0
	PIO_PA31C_MCDA1 |        //MCI Data 1
	PIO_PA26C_MCDA2 |        //MCI Data 2
	PIO_PA27C_MCDA3 |        //MCI Data 3
	PIO_PA28C_MCCDA |        //MCI Command
	PIO_PA29C_MCCK);         //MCI Clock
	pio_set_input(PIOA, PIO_PA6, PIO_INPUT | PIO_PULLUP);				//SD Card Detect Pin (active low)
	//Configure LEDs for status update
	pio_set_output(PIOA, PIO_PA19, PIO_TYPE_PIO_OUTPUT_0, PIO_DEFAULT, PIO_DEFAULT);
	pio_set_output(PIOA, PIO_PA20, PIO_TYPE_PIO_OUTPUT_0, PIO_DEFAULT, PIO_DEFAULT);

	//Initialize SD MMC stack
//	sd_mmc_init();
}