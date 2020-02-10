/*
 * handler.h
 *
 * Created: 2/10/2020 10:37:45 AM
 *  Author: Angel Popov
 */ 

#ifndef HANDLER_H_
#define HANDLER_H_

#include <stdbool.h>

#define MCU_RECEIVE 0
#define BLE_RECEIVE 1
#define TOUCH_RECEIVE 2


typedef void (*event_endpoint)(char *buffer, int size);

typedef struct event_linker {
	int id;
	int	flag;
	event_endpoint endpoint; // function pointer
}event_linker;

void event_register(struct event_linker *evt);
bool event_trigger(int id, int data_size);
static void event_processing(int id, char *data, int size);
static void mcu_receive_handler(char *data, int size);
static void ble_receive_handler(char *data, int size);
static void touch_receive_handler(char *data);

#endif /* HANDLER_H_ */