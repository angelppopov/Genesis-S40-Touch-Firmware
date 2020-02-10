/*
 * serial_ble.c
 *
 * Created: 2/10/2020 9:37:17 AM
 *  Author: Angel Popov
 */ 

#include "serial_ble.h"
#include "../handler.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define USART_RX_BUFFER_SIZE 512     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_TX_BUFFER_SIZE 512     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1 )
#define USART_TX_BUFFER_MASK ( USART_TX_BUFFER_SIZE - 1 )

static volatile unsigned char WritingPositionOnTheBuffer;
static volatile unsigned char ReadingPositionOnTheBuffer;

static volatile char receive_buffer[USART_RX_BUFFER_SIZE];
static volatile char instruction[USART_RX_BUFFER_SIZE];

FILE stdout_on_port_e = FDEV_SETUP_STREAM(transmit, NULL, _FDEV_SETUP_WRITE);

static struct event_linker ble = {
	.endPoint = receive,
	.id = BLE_RECEIVE,
	.output = transmit,
};

void serial_ble_init(void){
	/* Register event */
	event_register(&ble);
	/* Serial on PORTC0 */
	//For interrupt-driven USART operation, global interrupts should be disabled during the initialization
	cli();
	PORTC.PIN3CTRL = PORT_OPC_PULLUP_gc;	// pull up
	PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc;	// pull up
	PORTC_OUT |= (1<<PIN3_bp);
	PORTC_OUT |= (1<<PIN2_bp);
	PORTC.OUTSET = PIN3_bm;
	PORTC.DIR = PIN3_bm;
	PORTC.DIRCLR = PIN2_bm;
	// Communication mode: Asynchronous USART on PORTE
	// Data bits: 8
	// Stop bits: 1
	// Parity: Disabled
	USARTC0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
	// Receive complete interrupt: Low Level
	// Transmit complete interrupt: Disabled
	// Data register empty interrupt: Disabled
	USARTC0.CTRLA = (USARTC0.CTRLA & (~(USART_RXCINTLVL_gm | USART_TXCINTLVL_gm | USART_DREINTLVL_gm))) |
	USART_RXCINTLVL_LO_gc | USART_TXCINTLVL_OFF_gc | USART_DREINTLVL_OFF_gc;
	// Required Baud rate: 9600 at 2MHz system peripheral clock
	USARTC0_BAUDCTRLB = 0;
	USARTC0_BAUDCTRLA = 34; //207
	// Receiver: On
	// Transmitter: On
	// Double transmission speed mode: On
	// Multi-processor communication mode: Off
	USARTC0.CTRLB=(USARTC0.CTRLB & (~(USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm | USART_MPCM_bm | USART_TXB8_bm))) |
	USART_RXEN_bm | USART_TXEN_bm;/* | USART_CLK2X_bm;*/
	//Enable All level Interrupts
	PMIC.CTRL = PMIC_HILVLEN_bm |PMIC_MEDLVLEN_bm|PMIC_LOLVLEN_bm;
		
}

ISR(USARTC0_RXC_vect){
	unsigned char data;
	unsigned char tmpWPosition;
	/* Read the received data */
	data = USARTC0.DATA;
	/* Determine the Write PositionOnTheBuffer */
	tmpWPosition = (WritingPositionOnTheBuffer + 1) & USART_RX_BUFFER_MASK;
	/* Store new index */
	WritingPositionOnTheBuffer = tmpWPosition;
	/* Store received data in buffer */
	receive_buffer[tmpWPosition] = data;
}

static void put_string(char* sp){
	while(*sp != 0x00)	//Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
	{
		transmit(*sp);	//Using the simple send function we send one char at a time
		sp++;			//We increment the pointer so we can read the next char
	}
}

static void transmit(unsigned char data){
	while ( !( USARTC0.STATUS & USART_DREIF_bm) ); // Wait until loop
	USARTC0.DATA = data;
}

static void receive(char* buffer, unsigned int buf_size ){
	unsigned char tmpWPosition;
	unsigned int i = 0;
	while ( WritingPositionOnTheBuffer == ReadingPositionOnTheBuffer ){				// Wait for incoming data /

	}
	_delay_ms(300);
	for( i=0; i < buf_size - 1; ++i )
	{
		tmpWPosition = ( ReadingPositionOnTheBuffer + 1 ) & USART_RX_BUFFER_MASK;	// Calculate buffer index /
		
		ReadingPositionOnTheBuffer = tmpWPosition;									// Store new index /
		
		buffer[i] = receive_buffer[tmpWPosition];								// Return data /
		if( WritingPositionOnTheBuffer == ReadingPositionOnTheBuffer )
		break;
	}
	buffer[i+1] = 0x00;
}