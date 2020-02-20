/*
 * sd_card.c
 *
 * Created: 2/19/2020 11:54:30 AM
 *  Author: Angel Popov
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "sd_card.h"
#include "../spi/spi.h"

#define SD 3
volatile uint8_t tx_byte = 0;
volatile uint8_t rx_byte = 0;

static struct spi_device sd_card = {
	.id = SD,
	.port = &SPIC,
	.read = spi_read,
	.write = spi_write,
	};

// http://www.rjhcoding.com/avrc-sd-interface-1.php

void sd_card_init(void){
	/* Configure GPIO */
	PORTC.DIR = 0x40;          // MISO output; MOSI, SCK, SS inputs 0b01000000
	PORTE.DIR = 0xB0;          // MOSI, SCK, SS outputs; MISO input 0b10110000
	PORTE.OUTSET = 0x10;       // De-assert SS pin (active low)     0b00010000
	
	/* Configure SPI on PORTC and PORTE */
	SPIC.CTRL = 0x40;          // spi slave, spi mode 0
	SPIC.INTCTRL = 0x3;        // assign high priority to SPIC interrupts
	
	SPIE.CTRL = 0x50;          // spi master, spi mode 0
	SPIE.INTCTRL = 0x3;        // assign high priority to SPIF interrupts
	
	/* Flush slave receive buffer */
	while(SPIC.STATUS & 0x80) {
		rx_byte = SPIC.DATA;   // flush spi receive buffer
	}
	
	/* Flush master receive buffer */
	while(SPIE.STATUS & 0x80) {
		rx_byte = SPIE.DATA;   // flush spi receive buffer
	}
	

	SPIC.DATA = rx_byte;
	
	PORTE.OUTCLR = 0x10; // assert SS pin (active low)
	SPIE.DATA = tx_byte; // start transmission
	
	// spi_register_device(&sd_card);
	// spi_write(sd_card.port, "data");
}

static volatile int spi_read(SPI_t *port){
	
}

static volatile void spi_write(SPI_t *port, char *data){
	
}

ISR(SPIE_INT_vect) {
	tx_byte = SPIE.DATA;     // store character received from slave (int flag auto cleared)
	printf("master received %d\n", tx_byte);
	cpu_relax();
	SPIE.DATA = tx_byte + 1; // increment received data and send
}

ISR(SPIC_INT_vect) {
	rx_byte = SPIC.DATA;     // grab received byte
	printf("slave received %d\n", rx_byte);
	cpu_relax();
	SPIC.DATA = rx_byte;     // send back to the master
}

