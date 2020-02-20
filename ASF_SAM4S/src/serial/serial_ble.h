/*
 * serial_ble.h
 *
 * Created: 2/20/2020 10:03:57 AM
 *  Author: angelpopov
 */ 


#ifndef SERIAL_BLE_H_
#define SERIAL_BLE_H_

#include <asf.h>

void serial_ble_init(void);
static void send(Uart *port, char* sp);

#endif /* SERIAL_BLE_H_ */