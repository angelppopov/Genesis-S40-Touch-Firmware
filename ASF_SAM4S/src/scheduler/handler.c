/*
 * handler.c
 *
 * Created: 2/20/2020 10:03:16 AM
 *  Author: Angel Popov
 */ 

#include "handler.h"
#include "scheduler.h"
#include "../utils/utils.h"
#include "../memory/eeprom.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define block_size 100

static struct object objects[6];
static int status = SUCCESS;
static bool locked = false;
int current_temp = 90;

/* 
	This function appends to the event buffer
*/
void event_register(struct object *obj){
	/* Assign an event read function */
	objects[obj->id].input = obj->input;
	/* Assign an event output function */
	objects[obj->id].output = obj->output;
	cpu_relax();
	std_write("Object registered\n");
}

volatile bool event_trigger(int id, int data_size){
	if(locked)
		return false;
	/* Lock event trigger */
	locked = true;
	/* Request buffer for the data */
	char *buffer = (char *)malloc(sizeof(char) * data_size);
	if(buffer == NULL) reset();
	/* Get the event result */
	objects[id].input(buffer, data_size);
	/* Process the event */
	event_processing(id, buffer, data_size);
	/* Free up memory */
	free(buffer);
	/* Release the flag */
	locked = false;
	/* Return true to other event triggers */
	return true;
}

static inline void event_processing(int id, char *data, int size){
	
	//printf("event processing.. %d\n", id);
	switch(id){
		case MPU_RECEIVE:
		mcu_receive_handler(data, size);
		break;
		case BLE_RECEIVE:
		ble_receive_handler(data, size);
		break;
		case TOUCH_RECEIVE:
		touch_receive_handler(data);
		break;
	}
}

static void mcu_receive_handler(char *data, int size){
	/* Echo to Serial BLE Module */
	//objects[BLE].output(data);
	
	if (strstr(data, "Error") != NULL) status = ERROR;
	else status = SUCCESS;
	std_write("mcu_receive_handler\n");
	//printf("mcu_receive_handler has had: %s\n", data);
	switch (status) {
		case SUCCESS:
		objects[BLE].output(data);
//		objects[TOUCH].output(data);
		break;
		case ERROR:
		objects[BLE].output(data);
		objects[TOUCH].output(data);
		break;
		default:
		objects[TOUCH].output(data);
		break;
	}
}

static void ble_receive_handler(char *data, int size){
	std_write("ble_receive_handler\n");
	objects[MPU].output(data);
}

static void touch_receive_handler(char *addr){
	/* Check if the object was able to give you the data */
	int data = (int) * addr;
	std_write("touch_receive_handler\n");
	
	if(addr != NULL){
		/* Handle touch event */
		char *command = (char*)malloc(sizeof(char) * block_size);
		if(command == NULL) reset();
		/* Get the command from memory address map */
		from_memory_map(command, (int ) *addr);		
		/* Send command over serial to MCU */
		objects[MPU].output(command);
		/* Free allocated memory */
		free(command);
	}else{
		cpu_relax();
	}
}