/*
 * i2c.c
 *
 * Created: 28.2.2020 г. 12:56:24
 *  Author: angel
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c.h"

static void write(unsigned const char addr ,unsigned const char data);
static void read(void);

void i2c_init(void){
	
	//SDA hold time OFF and normal TWI operation with two-pin interface
	TWIF_CTRL = 0x00;
	//enable TWI read and write interrupt with highest interrupt and enable master
	TWIF_MASTER_CTRLA = TWI_MASTER_INTLVL_gm | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm; 
	//disable Inactive Bus Timeout, disable QCEN and SMEN
	TWIF_MASTER_CTRLB = 0x00;
	//send ACK and NOACT initially 
	TWIF_MASTER_CTRLC = 0x00;
	//initially clear all flags and select bus state as IDLE
	TWIF_MASTER_STATUS = TWI_MASTER_RIF_bm | TWI_MASTER_WIF_bm | TWI_MASTER_ARBLOST_bm | TWI_MASTER_BUSERR_bm | TWI_MASTER_BUSSTATE0_bm; 
	/*
	The BAUD rate register defines relation between system clock and TWI SCL frequency. 
	This relation is given by following equation.
	BAUD=(fsys/(2*ftwi))-5
	We will use ftwi as 100KHz. As fsys=16MHz
	BAUD=(16*10^6)/(2*100*10^3)-5=75
	*/
	
	//BAUD rate for ftwi=100KHz
	TWIF_MASTER_BAUD = 0x4b; 
	
	TWIF_MASTER_ADDR = 0xd1; //for TWI read
	TWIF_MASTER_ADDR = 0xd0; //for TWI write
	write(1 , '1');
}

/* TWI Master Interrupt */
ISR(TWIF_TWIM_vect){
	printf("TWI Master Interrupt\n");
}

/* TWI Slave Interrupt */
ISR(TWIF_TWIS_vect){
	
	printf("TWI Slave Interrupt\n");
}

static void write(unsigned const char addr, unsigned const char data){
	TWIF_MASTER_DATA = data;
	printf("write\n");
}

static void read(void){
	
}