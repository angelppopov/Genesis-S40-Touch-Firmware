/*
 * eeprom.h
 *
 * Created: 2/20/2020 11:18:05 AM
 *  Author: angelpopov
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

void memory_init();
void from_memory_map(char *buffer, int addr);
void update_memory_block(char *data, int addr);

#endif /* EEPROM_H_ */