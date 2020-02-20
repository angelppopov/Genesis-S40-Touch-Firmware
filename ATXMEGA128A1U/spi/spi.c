/*
 * spi.c
 *
 * Created: 2/18/2020 9:52:32 PM
 * Author: Angel Popov
 
 */ 
#include "spi.h"
#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>

static struct spi_device *devices;
static uint8_t device_number = 1;


void spi_init(void){
	/* Allocate memory for the devices */
	devices = (struct spi_device *)malloc(sizeof(struct spi_device));
}

bool spi_register_device(struct spi_device *device){
	/* Check if device ID already exists */
	for (int i = 0; i < device_number; i++)
		if(device->id == ((struct spi_device *) (device_number + i))->id) return SPI_PORT_UNAVALIABLE;
	/* Keep track of the allocated devices */
	device_number++;
	/* Reallocate space for the new device */
	devices = (struct spi_device *)realloc(devices, sizeof(struct spi_device) * device_number);
	/*
		If realloc return NULL reset the MCU
	*/
	if(devices == NULL) reset();
	else
		cpu_relax();
	/* Initialize SPI port for the driver */	
	spi_initialize_port(devices->port);
	/* Return success to the calling driver */
	return SPI_PORT_AVALIABLE;
}

struct spi_device *get_spi_device_table(){
	
}
static int spi_read(SPI_t *port){
	
}

static void spi_write(SPI_t *port, char *data){
	
}

static inline void spi_initialize_port(SPI_t *port){
	port->CTRL = 0xD0;    // SPI master, clock idle low, data setup on trailing edge, data sampled on leading edge, double speed mode enabled
	port->INTCTRL  = 0x03; // assign high priority to SPI interrupt
}