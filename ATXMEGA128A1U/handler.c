#include "handler.h"
#include "scheduler.h"
#include "mem/eeprom.h"
#include <stdlib.h>
#include <string.h>

#define block_size 100

static struct object objects[6];
static int status = SUCCESS;
static bool locked = false;
int current_temp = 0;
bool processing_mpu_request = false;

/* 
	This function appends to the event buffer
*/

void event_register(struct object *obj){
	/* Assign an event read function */
	objects[obj->id].input = obj->input;
	/* Assign an event output function */
	objects[obj->id].output = obj->output;
	cpu_relax();
}

volatile bool event_trigger(int id, int data_size){
	if(locked)
		return false;
	/* Lock event trigger */
	locked = true;
	/* Request buffer for the data */
	char *buffer = (char *)malloc(sizeof(char) * data_size);
	if(buffer == NULL)
		reset();
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
	
	printf("event processing.. %d\n", id);
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
	printf("mcu_receive_handler has had: %s\n", data);
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

static void touch_receive_handler(char *addr_offset){
	/* Check if the object was able to give you the data */
	int data = (int) * addr_offset;
	printf("touch_receive_handler has had: %d\n", data);
	if(*addr_offset != NULL){
		/* Handle touch event */
		char *command = (char*)malloc(sizeof(char) * block_size);
		if(command == NULL) reset();
		/* Get the command from memory address map */
		from_memory_map(command, (int ) *addr_offset);		
		/* Send command over serial to MCU */
		objects[MPU].output(command);
		/* Free allocated memory */
		free(command);
	}else{
		cpu_relax();
	}
}
