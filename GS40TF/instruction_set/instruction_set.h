/*
 * instruction_set.h
 *
 * Created: 2/10/2020 9:38:13 AM
 *  Author: angelpopov
 */ 


#ifndef INSTRUCTION_SET_H_
#define INSTRUCTION_SET_H_

struct commands{
		uint8_t command;
		unsigned int mem_addr;
	};
	
struct touch{
	uint8_t pin;
	unsigned int mem_addr;
};

#endif /* INSTRUCTION_SET_H_ */