/*
 * serial_ble.h
 *
 * Created: 2/10/2020 9:37:25 AM
 *  Author: angelpopov
 */ 


#ifndef SERIAL_BLE_H_
#define SERIAL_BLE_H_
/* Serial on PORTC0 */
void serial_ble_init(void);
static void send(char* sp);
static void transmit(unsigned char data);
static void receive(char* buffer, unsigned int buf_size );

#endif /* SERIAL_BLE_H_ */