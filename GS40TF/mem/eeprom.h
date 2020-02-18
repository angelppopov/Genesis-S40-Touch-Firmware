/*
 * eeprom.h
 *
 * Created: 2/11/2020 8:55:44 AM
 *  Author: Angel Popov
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

void memory_init();
void from_memory_map(char *buffer, int addr);
void update_memory_block(char *data, int addr);

#endif /* EEPROM_H_ */