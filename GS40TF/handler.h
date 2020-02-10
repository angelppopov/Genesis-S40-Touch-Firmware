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
#define MCU 0

#define BLE_RECEIVE 1
#define BLE 1

#define TOUCH_RECEIVE 2
#define TOUCH 1

#define SUCCESS 1
#define ERROR -1

typedef void (*event_endPoint)(char *buffer, int size);
typedef void (*object_outputPoint)(char *buffer);

typedef struct objects_io{
    object_outputPoint send;
}objects_io;

typedef struct event_linker {
    int id;                         /*  Event id  */
    
    object_outputPoint output;      /*
                                        Function pointer to object output point.
                                        Typically it is some send function to transmit data or set function.
                                    */
    
    event_endPoint endPoint;        /*
                                        Function pointer ot object end point.
                                        This is an object endpoint to return processed data from hardware interrupt handler.
                                    */
}event_linker;

void event_register(struct event_linker *evt);
bool event_trigger(int id, int data_size);
static void event_processing(int id, char *data, int size);
static void mcu_receive_handler(char *data, int size);
static void ble_receive_handler(char *data, int size);
static void touch_receive_handler(char *data);

#endif /* HANDLER_H_ */
