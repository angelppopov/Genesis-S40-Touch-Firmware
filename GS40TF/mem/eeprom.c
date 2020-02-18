/*
 * eeprom.c
 *
 * Created: 2/11/2020 8:55:34 AM
 *  Author: Angel Popov
 */ 

#include "eeprom.h"
#include <avr/eeprom.h>

#define TMP 0
#define HF 2

#define COMMANDS 50
#define DATA 150
#define block_size 100

int target_temp;

void memory_init(){
	/* Read the data from eeprom memory */
	target_temp = (int) eeprom_read_word (( uint16_t *) TMP);
}

void from_memory_map(char *buffer, int addr){
	eeprom_read_block(( void *) buffer, ( const void *) DATA + (addr * block_size), block_size);
}

void update_memory_block(char *data, int addr){
	eeprom_update_block(( void *) data ,
						( const void *) DATA + (addr * block_size),
						block_size);
}
