/*
 * serial_mcu.c
 *
 * Created: 2/10/2020 9:37:39 AM
 *  Author: Angel Popov
 */ 

#include "serial_mcu.h"
#include "../handler.h"
#include "../scheduler.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define USART_RX_BUFFER_SIZE 512     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_TX_BUFFER_SIZE 512     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1 )
#define USART_TX_BUFFER_MASK ( USART_TX_BUFFER_SIZE - 1 )

typedef struct {
	volatile char buffer[USART_RX_BUFFER_SIZE];
	volatile int size;
}event_data;

static void send(char* sp);
static void transmit(unsigned char data);
static void receive(char* buffer, unsigned int buf_size);
static volatile void get_the_data_from_the_ring_buffer(event_data *data);
static volatile void event_occurred(void);
static volatile event_data* get_event();

static volatile unsigned char WritingPositionOnTheBuffer;
static volatile unsigned char ReadingPositionOnTheBuffer;
static volatile int bytes_received;

static volatile char receive_buffer[USART_RX_BUFFER_SIZE];
static volatile char instruction[USART_RX_BUFFER_SIZE];

FILE stdout_on_port_e = FDEV_SETUP_STREAM(transmit, NULL, _FDEV_SETUP_WRITE);

static struct event_linker mcu = {
	.id = MCU_RECEIVE,
	.input = receive,
	.output = send,
};

static q_event *this_event;
extern event_scheduler scheduler;

static volatile event_data *events_data;
static int event_number;
static uint8_t current_event_read;

void serial_mcu_init(void){
	/* Register event */
	event_register(&mcu);
	this_event->id = MCU_RECEIVE;
	/* Serial on PORTE0 */
	//For interrupt-driven USART operation, global interrupts should be disabled during the initialization
	cli();
	PORTE.PIN3CTRL = PORT_OPC_PULLUP_gc;	// pull up
	PORTE.PIN2CTRL = PORT_OPC_PULLUP_gc;	// pull up
	PORTE_OUT |= (1<<PIN3_bp);
	PORTE_OUT |= (1<<PIN2_bp);
	PORTE.OUTSET = PIN3_bm;
	PORTE.DIR = PIN3_bm;
	PORTE.DIRCLR = PIN2_bm;
	// Communication mode: Asynchronous USART on PORTE
	// Data bits: 8
	// Stop bits: 1
	// Parity: Disabled
	USARTE0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
	// Receive complete interrupt: Low Level
	// Transmit complete interrupt: Disabled
	// Data register empty interrupt: Disabled
	USARTE0.CTRLA = (USARTE0.CTRLA & (~(USART_RXCINTLVL_gm | USART_TXCINTLVL_gm | USART_DREINTLVL_gm))) |
	USART_RXCINTLVL_LO_gc | USART_TXCINTLVL_OFF_gc | USART_DREINTLVL_OFF_gc;
	// Required Baud rate: 9600 at 2MHz system peripheral clock
	USARTE0_BAUDCTRLB = 0;
	USARTE0_BAUDCTRLA = 34; //207
	// Receiver: On
	// Transmitter: On
	// Double transmission speed mode: On
	// Multi-processor communication mode: Off
	USARTE0.CTRLB=(USARTE0.CTRLB & (~(USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm | USART_MPCM_bm | USART_TXB8_bm))) |
	USART_RXEN_bm | USART_TXEN_bm;/* | USART_CLK2X_bm;*/
	//Enable All level Interrupts
	PMIC.CTRL = PMIC_HILVLEN_bm |PMIC_MEDLVLEN_bm|PMIC_LOLVLEN_bm;	
}

/*
	This USARTE0_RXC_vect with number _VECTOR(58) is triggered when a char arrives on the Serial PORTE.
	The ISR reads the data from USART data register 0x0AA0 and appends it to a ring buffer with size of 512 bytes.
	An event gets issued from the ISR when the value of data register 0x0AA0 is 0x0A.
	
	TODO: Implement DMA driven data read;
*/

ISR(USARTE0_RXC_vect){
	unsigned char data;
	unsigned char tmpWPosition;
	/* Read the received data */
	data = USARTE0.DATA;
	/* Count the number of bytes received */
	bytes_received++;
	/* If data is terminated by a new line tell the scheduler that has new data to be read */
	if(data == '\n'){
		/* Register the event occurrence */
		event_occurred();
		/* Get the data from the ring buffer in order to be accessed when the scheduler executes this event */
		get_the_data_from_the_ring_buffer(get_event());
		/* Add the event to the scheduler in order to be executed */
		scheduler.add(this_event);
	}else{
		/* Determine the Write PositionOnTheBuffer */
		tmpWPosition = (WritingPositionOnTheBuffer + 1) & USART_RX_BUFFER_MASK;
		/* Store new index */
		WritingPositionOnTheBuffer = tmpWPosition;
		/* Store received data in buffer */
		receive_buffer[tmpWPosition] = data;
	}
}

/*
	This is a output function to the event system.
	The function will be called as a result of a processed event by the event system 
	when needs to send data over this bus.
*/

static void send(char* sp){
	while(*sp != 0x00)	//Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
	{
		transmit(*sp);	//Using the simple send function we send one char at a time
		sp++;			//We increment the pointer so we can read the next char
	}
}

static void transmit(unsigned char data){
	while ( !( USARTE0.STATUS & USART_DREIF_bm) ); // Wait until loop
	USARTE0.DATA = data;
}

/*
	This is a input function to the event system.
	This function is called when the scheduler executes this event.
*/

static void receive(char* buffer, unsigned int buf_size ){
	
	event_data *data_to_be_read = (events_data + current_event_read);
	
	for (int i = 0; i < data_to_be_read->size; i++)
	{
		*(buffer + i) = data_to_be_read->buffer[i];
	}
	
	current_event_read++;
	if (current_event_read == event_number) {
		current_event_read = event_number = 0;
		free(events_data);
	}
}

static volatile void get_the_data_from_the_ring_buffer(event_data *data){
	unsigned char tmpWPosition;
	unsigned int i = 0;
	for(i = 0; i < data->size - 1; ++i ){
		tmpWPosition = ( ReadingPositionOnTheBuffer + 1 ) & USART_RX_BUFFER_MASK;	// Calculate buffer index /
		ReadingPositionOnTheBuffer = tmpWPosition;									// Store new index /
		data->buffer[i] = receive_buffer[tmpWPosition];								// Return data /
		if( WritingPositionOnTheBuffer == ReadingPositionOnTheBuffer )
		break;
	}
	data->buffer[i+1] = 0x00;
}

static volatile void event_occurred(void){
	event_number++;
	/* If this is first event */
	if(event_number <= 1){
		/* Allocate memory for the new data to be read when this event gets executed from scheduler */
		events_data = (event_data *)malloc(sizeof(event_data));		
		events_data->size = bytes_received;
	}else{
		/* If the previous event is not read from the event system save the event data */
		/* Resize the memory block if has been allocated */
		events_data = (event_data *)realloc(events_data, sizeof(event_data) * event_number);
		event_data *last_data = (events_data + event_number - 1);
		last_data->size = bytes_received;
	}
	
	this_event->data_size = bytes_received;
	bytes_received = 0;
}

static volatile event_data* get_event(){
	return (events_data + event_number - 1);
}