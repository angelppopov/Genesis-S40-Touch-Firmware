#include "handler.h"
#include "scheduler.h"
#include <stdlib.h>
#include <string.h>

static event_linker events[16];

//static enum event_types type;

static bool locked = false;

void event_register(struct event_linker *ev){
	events[ev->id].end_point = ev->end_point;
	events[ev->id].out_put = ev->out_put;
}

volatile bool event_trigger(int id, int data_size){
	if(locked)
	return false;
	/* Lock event trigger */
	locked = true;
	/* Request buffer for the data */
	char *buffer = (char *)malloc(sizeof(char) * data_size);
	/* Get the event result */
	events[id].end_point(buffer, data_size);
	/* Process the event */
	event_processing(id, buffer, data_size);
	/* Free up memory */
	free(buffer);
	/* Release the flag */
	locked = false;
	/* Return true to other event triggers */
	return true;
}

static void event_processing(int id, char *data, int size){

	switch(id){
		case MCU_RECEIVE:
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
	printf("mcu receive handler with data size: %d bytes\n", size);
	
	
	if(strcmp(data, "Installed") == 0) printf("Installed\n");
	int s = 0;

	switch (s) {
		case SUCCESS:
		events[MCU].out_put(data);
		events[TOUCH].out_put(data);
		break;
		case ERROR:
		events[BLE].out_put(data);
		events[TOUCH].out_put(data);
		break;
		default:
		events[TOUCH].out_put(data);
		break;
	}
}

static void ble_receive_handler(char *data, int size){
	printf("bluetooth receive handler with data size: %d bytes\n", size);
	events[MCU].out_put(data);
}

static void touch_receive_handler(char *data){
	/* Handle touch event */
	printf("touch receive handler: %c\n", *data);
	/* Get the command from memory address map */
	char *command = "from_memory_map[*data]";
	/* Send command over serial to MCU */
	events[BLE].out_put(command);
	/* Send command over serial Bluetooth Module to connected devices */
	//    events[BLE].output(command);
}
