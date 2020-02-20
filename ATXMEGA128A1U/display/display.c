/*
 * display.c
 *
 * Created: 2/19/2020 12:26:22 PM
 *  Author: Angel Popov
 */ 
#include "../spi/spi.h"
#include <avr/io.h>
#define DISPLAY 4

void display_init(void);

static void display_write(SPI_t *port, char *data);

static struct spi_device display = {
	.id = DISPLAY,
	.port = &SPIE,
	.write = display_write
	};
	
void display_init(void){
	spi_register_device(&display);
}
	
static void display_write(SPI_t *port, char *data){
	
}