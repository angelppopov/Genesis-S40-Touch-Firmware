#include "handler.h"
#include <stdlib.h>

static event_linker events[16];

//static enum event_types type;

static bool locked = false;

void event_register(struct event_linker *evt){
	events[evt->id].endpoint = evt->endpoint;
	events[evt->id].flag = evt->flag;
}

bool event_trigger(int id, int data_size){
	if(locked)
		return false;
	/* Lock event trigger */
	locked = true;		
	/* Request buffer for the data */
	char *buffer = (char *)malloc(sizeof(char) * data_size);
	/* Get the event result */
	events[id].endpoint(buffer, data_size);
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
	
}

static void ble_receive_handler(char *data, int size){
	
}

static void touch_receive_handler(char *data){
	printf("%c\n", *data);
}
