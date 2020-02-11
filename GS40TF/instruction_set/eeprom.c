/*
 * eeprom.c
 *
 * Created: 2/11/2020 8:55:34 AM
 *  Author: Angel Popov
 */ 

#include "eeprom.h"

//static struct data_region memory_descriptor[6];
static enum memory_types types;

static struct conf			configs[6];
static struct cmd			commands[6];
static struct rcp			rcps[12];
static struct t_btn			touch_sensors[12];
static struct err			errs[6];


void memory_init(){
	
}

static void read(void **variable, char *data, unsigned int size){
	
}

static void write(uint8_t addr, char *data){
	
}

