/*
 * eeprom.h
 *
 * Created: 2/11/2020 8:55:44 AM
 *  Author: Angel Popov
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>

typedef enum memory_types{
	CONF,
	CMD,
	DESC,
	CTR,
	RCP,
	BTN,
	ERR
}memory_types;

// typedef struct memory_descriptor{
// 		uint8_t start;
// 		uint8_t bytes;
// }data_region;

typedef struct conf{
	char *data;
	uint8_t type;
}conf;

typedef struct cmd{
	uint8_t code;
}cmd;

typedef struct ctr{
	char data[4];
	uint8_t rcp_linker;
}ctr;

typedef struct rcp{
	char data[100];
}rcp;

typedef struct t_btn{
	uint8_t touch_btn;
}t_btn;

typedef struct err{
	uint8_t code;
}err;

void memory_init();
static void read(void **variable, char *data, unsigned int size);
static void write(uint8_t addr, char *data);

#endif /* EEPROM_H_ */