/*
 * sd_card.c
 *
 * Created: 2/20/2020 12:38:18 PM
 *  Author: Angel Popov
 */ 

#include "sd_card.h"
#include "../utils/utils.h"
#include <asf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void sd_card_init(void){
	
	std_write("SD Card initializing\n");
	//Enable PIOA clock to detect Card Detect (CD) change
	sysclk_enable_peripheral_clock(ID_PIOA);

	//Configure SD card pins as per MCI peripheral (periph C)
	pio_set_peripheral(PIOA, PIO_PERIPH_C, PIO_PA30C_MCDA0 |        //MCI Data 0
	PIO_PA31C_MCDA1 |        //MCI Data 1
	PIO_PA26C_MCDA2 |        //MCI Data 2
	PIO_PA27C_MCDA3 |        //MCI Data 3
	PIO_PA28C_MCCDA |        //MCI Command
	PIO_PA29C_MCCK);         //MCI Clock
	//SD Card Detect Pin (active low)
	pio_set_input(PIOA, PIO_PA6, PIO_INPUT | PIO_PULLUP);

	//Configure LEDs for status update
	pio_set_output(PIOA, PIO_PA19, PIO_TYPE_PIO_OUTPUT_0, PIO_DEFAULT, PIO_DEFAULT);
	pio_set_output(PIOA, PIO_PA20, PIO_TYPE_PIO_OUTPUT_0, PIO_DEFAULT, PIO_DEFAULT);

	//Initialize SD MMC stack
	sd_mmc_init();
	
	write("0:test.txt", "test data\n");
	write("0:test2.txt", "test2 data\n");

}

static void write(const char *file_name, const char *data){
	FATFS fs;       //File system variable
	FIL fhandle;    //File handle variable
	char buf[25];
		
	std_write("Writing to sd card\n");
	
	// Mount the file system
	memset(&fs, 0, sizeof(FATFS));					//initially clear it
	if(f_mount(LUN_ID_SD_MMC_0_MEM, &fs) != FR_OK){
		std_write("Fail to mount\n");
		return 0; //failed
	}

	Ctrl_status status = sd_mmc_test_unit_ready(0);
	while (status != CTRL_GOOD){
		status = sd_mmc_test_unit_ready(0);
		if(status == CTRL_FAIL){
			//Wait for a card to be inserted
			while (sd_mmc_check(0) != CTRL_NO_PRESENT)
				wdt_restart(WDT);
		}
	}
		
	//Open a file
	if(f_open(&fhandle, file_name, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
	{
		uint32_t cap = sd_mmc_get_capacity(0);								//File opened successfully, read card capacity
		sprintf(buf,"Capacity: %lu\n",cap);
		//Write capacity to file
		if(f_puts(data, &fhandle) != 0){
			std_write("Data is successfully written to the SD Card\n");
		}else{
			std_write("Data fail to be written to the SD Card\n");
		}
		f_close(&fhandle);													//Close the file (important!!)
	}else{
		std_write("Fail to open the file\n");
	}
}