/*
 * handler.h
 *
 * Created: 2/20/2020 10:03:02 AM
 *  Author: angelpopov
 */ 


#ifndef HANDLER_H_
#define HANDLER_H_

#include <stdbool.h>

#define MPU_RECEIVE 0
#define MPU 0

#define BLE_RECEIVE 1
#define BLE 1

#define TOUCH_RECEIVE 2
#define TOUCH 2

#define SD 3

#define SUCCESS 1
#define ERROR -1

typedef void (*object_input)(char *buffer, int size);
typedef void (*object_output)(char *buffer);

struct object{
    int id;                         /*  
										Event id  
										An event has its own place on the event buffer
									*/
    
    object_input input;				 /*
                                        Function pointer to an object read point.
										When the scheduler process the event from its event queue buffer it uses that function
										pointer to get the process data from hardware interrupt handler when the event has been occurred.
                                    */
	
	object_output output;           /*
                                        Function pointer to an object output point.
                                        Typically it is some send function to transmit data or set function.
                                    */
};

void event_register(struct object *obj);
volatile bool event_trigger(int id, int data_size);
static inline void event_processing(int id, char *data, int size);
static void mcu_receive_handler(char *data, int size);
static void ble_receive_handler(char *data, int size);
static void touch_receive_handler(char *data);

#endif /* HANDLER_H_ */