/*
 * nvm.h
 *
 * Created: 2/20/2020 11:18:05 AM
 *  Author: Angel Popov
 */ 


#ifndef NVM_H_
#define NVM_H_

void nvmem_init();
void from_memory_map(char *buffer, int addr);
void update_memory_block(char *data, int addr);

#endif /* NVM_H_ */