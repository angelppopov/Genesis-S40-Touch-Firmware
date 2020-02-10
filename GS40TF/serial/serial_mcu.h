/*
 * serial_mcu.h
 *
 * Created: 2/10/2020 9:37:47 AM
 *  Author: Angel Popov
 */ 


#ifndef SERIAL_MCU_H_
#define SERIAL_MCU_H_
void serial_mcu_init(void);
static void put_string(char* sp);
static void transmit(unsigned char data);
static void receive(char* buffer, unsigned int buf_size);
#endif /* SERIAL_MCU_H_ */