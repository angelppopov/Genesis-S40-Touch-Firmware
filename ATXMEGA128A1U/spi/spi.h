/*
 * spi.h
 *
 * Created: 2/18/2020 9:55:54 PM
 *  Author: Angel Popov
 */ 


#ifndef SPI_H_
#define SPI_H_

#define SPI_PORT_AVALIABLE true
#define SPI_PORT_UNAVALIABLE false

#include <avr/io.h>
#include <stdbool.h>

typedef volatile int (*f_read)(SPI_t *port);
typedef volatile void (*f_write)(SPI_t *port, char *data);

struct spi_device{
	int id;
	SPI_t *port;
	f_read read;
	f_write write;
	};
	
void spi_init(void);
bool spi_register_device(struct spi_device *device);
struct spi_device *get_spi_device_table();
static int spi_read(SPI_t *port);
static void spi_write(SPI_t *port, char *data);
static inline void spi_initialize_port(SPI_t *port);

#endif /* SPI_H_ */