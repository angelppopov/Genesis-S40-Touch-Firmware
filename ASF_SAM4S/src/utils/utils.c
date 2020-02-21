/*
 * utils.c
 *
 * Created: 2/20/2020 11:12:03 AM
 *  Author: Angel Popov
 */ 

#include "utils.h"
#include "serial/serial_mcu.h"


void reset(void){
	
}


void std_write(char *data){
	mpu_write(data);
}