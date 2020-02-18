/*
 * eeprom.c
 *
 * Created: 2/11/2020 8:55:34 AM
 *  Author: Angel Popov
 */ 

#include "eeprom.h"

#define CONFIG 0
#define COMMANDS 100
#define DATA 150
#define TOUCH 800
#define ERROR 900

static char ESPR[] = "E:10-20-30-5-5";
static char MLK[] = "C1:1-70-5";
static char CHOC[] = "C2:1-70-5";
static char WAT[] = "W1:1-120";
static char MIX[] = "M1:1-130";

//static struct data_region memory_descriptor[6];
static enum memory_types types;

static struct conf			configs[6];
static struct cmd			commands[6];
static struct rcp			rcps[12];
static struct t_btn			touch_sensors[12];
static struct err			errs[6];


void memory_init(){
	/* Read the data from eeprom memory */
	
}

static void read(void **variable, char *data, unsigned int size){
	
}

static void write(uint8_t addr, char *data){
	
}

