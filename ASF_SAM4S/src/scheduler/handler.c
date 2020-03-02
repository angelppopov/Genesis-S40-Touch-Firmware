/*
 * handler.c
 *
 * Created: 2/20/2020 10:03:16 AM
 *  Author: Angel Popov
 */

#include <asf.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "handler.h"
#include "scheduler.h"
#include "../utils/utils.h"
#include "../memory/eeprom.h"

#define block_size 100
#define locked	!ioport_get_pin_level(LED_0_PIN)
#define unlock	ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE)
#define lock	ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE)

static struct object objects[6];
static int status = SUCCESS;
static char* request;
int current_temp = 90;
bool processing_mpu_request;

/* 
	This function appends to the event buffer
*/

void event_register(struct object *obj){
	/* Assign an event read function */
	objects[obj->id].input = obj->input;
	/* Assign an event output function */
	objects[obj->id].output = obj->output;
	cpu_relax();
	printf("Object registered id: %d\n", obj->id);
}

volatile bool event_trigger(int id, int data_size){
	if(locked)
		return false;
	/* Lock event trigger */
	lock;
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
	unlock;
	/* Return true to other event triggers */
	return true;
}

static inline void event_processing(int id, char *data, int size){
	
	printf("event processing id %d\n", id);
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
	
	printf("mcu_receive_handler has had: %s\n", data);
	
	/* Handle errors */
	if (strstr(data, "Error") != NULL) status = ERROR;
	else status = SUCCESS;
	/* Read the current temperature */
	if (strstr(data, "Boiler") != NULL) current_temp = get_current_temp(data);
	/* Confirm request */
	if((sizeof(request) > 1) & strstr(data, request) != NULL) {
		/* Free up allocated memory */
		free(request);
		/* Send conformation */
		objects[MPU].output("Confirmed");
	}else{
		/* Resend the command over serial to MCU */
		objects[MPU].output(request);
	}
	/* Increment the counter on success */
	if(strstr(data, "Ready") != NULL){
		
	}
	
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
	printf("ble_receive_handler has had: %s\n", data);
	objects[MPU].output(data);
}

static void touch_receive_handler(char *addr){
	/* Check if the object was able to give you the data */
	int data = (int) * addr;
	printf("touch_receive_handler has had: %d\n", data);
	
	if(*addr != NULL){
		/* Handle touch event */
		char *command = (char*)malloc(sizeof(char) * block_size);
		if(command == NULL) reset();
		/* Get the command from memory address map */
		from_memory_map(command, (int ) *addr);		
		/* Send command over serial to MCU */
		objects[MPU].output(command);
		/* Save instruction in new buffer */
		memcpy(request, command, sizeof(char) * block_size);
		/* Free allocated memory */
		free(command);
	}else{
		cpu_relax();
	}
}