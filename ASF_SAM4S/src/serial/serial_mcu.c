/*
 * serial_mcu.c
 *
 * Created: 2/20/2020 10:03:28 AM
 *  Author: Angel Popov
 
 * ref: https://www.inverseproblem.co.nz/Guides/index.php?n=ARM.ExUART
 */ 

#include "serial_mcu.h"
#include "../scheduler/scheduler.h"
#include "../scheduler/handler.h"
#include <stdio.h>
#include <stdlib.h>

#define USART_RX_BUFFER_SIZE 256     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_TX_BUFFER_SIZE 256     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1 )
#define USART_TX_BUFFER_MASK ( USART_TX_BUFFER_SIZE - 1 )

typedef struct {
	volatile char buffer[USART_RX_BUFFER_SIZE];
	volatile int size;
}event_data;

static void send(char* sp);
static void receive(char* buffer, unsigned int buf_size);
static volatile void get_the_data_from_the_ring_buffer(event_data *data);
static volatile void event_occurred(void);
static volatile event_data* get_event();

static volatile char receive_buffer[USART_RX_BUFFER_SIZE];
static volatile unsigned char WritingPositionOnTheBuffer;
static volatile unsigned char ReadingPositionOnTheBuffer;
static volatile int bytes_received;

static struct object mpu = {
	.id = MPU,
	.input = receive,
	.output = send,
};

static volatile event_data *events_data;
static int event_number;
static uint8_t current_event_read;

extern volatile event_scheduler scheduler;

//FILE stdout_on_port_e = FDEV_SETUP_STREAM(uart_write, NULL, _FDEV_SETUP_WRITE);


void serial_mcu_init(void){
	
	pio_set_peripheral(PIOB, PIO_PERIPH_A, PIO_PB2A_URXD1 | PIO_PB3A_UTXD1);				// Allow UART to control PB2 and PB3
	sysclk_enable_peripheral_clock(ID_UART1);												// Enable UART1 Clock
	const sam_uart_opt_t uart1Settings = {													// Configures desired baudrate and sets no parity
		sysclk_get_cpu_hz(),							// Get system cpu clock
		115200UL,										// Set desired baudrate
		UART_MR_PAR_NO									// Set no parity
	};	
	uart_init(UART1, &uart1Settings);														// Initialize UART1
	uart_enable_interrupt(UART1, UART_IER_RXRDY);											// Enable Rx Ready Interrupt on UART1
	irq_register_handler(UART1_IRQn, 0);													// Register Interrupt with Priority 0
	
	event_register(&mpu);
}

/*

*/
ISR(UART1_Handler)
{
	unsigned char data;
	unsigned char tmpWPosition;
	
	/* If data received */
	if((uart_get_status(UART1) & UART_SR_RXRDY) == UART_SR_RXRDY)
	{
		uart_read(UART1, &data);							/* Read the received data */
		while (!(UART1->UART_SR & UART_SR_TXRDY));			// Wait for tx free
		/* Count the number of bytes received */
		bytes_received++;
		/* If data is terminated by a new line tell the scheduler that has new data to be read */
		if(data == '\n'){
			/* Register the event occurrence */
			event_occurred();
			/* Get the data from the ring buffer in order to be accessed when the scheduler executes this event */
			get_the_data_from_the_ring_buffer(get_event());
			/* Add the event to the scheduler in order to be executed */
			scheduler.add(MPU_RECEIVE, bytes_received);
			bytes_received = 0;
		}else{
			/* Determine the Write PositionOnTheBuffer */
			tmpWPosition = (WritingPositionOnTheBuffer + 1) & USART_RX_BUFFER_MASK;
			/* Store new index */
			WritingPositionOnTheBuffer = tmpWPosition;
			/* Store received data in buffer */
			receive_buffer[tmpWPosition] = data;
		}
	}
}


/*
	This is a output function to the event system.
	The function will be called as a result of a processed event by the event system 
	when needs to send data over this bus.
*/

static void send(char* sp){
	int breaking = 0;										// Flag to prevent running in infinite loop
	while(*sp != 0x00)										// Execute until 0x00 is found
	{
		while (!(UART1->UART_SR & UART_SR_TXRDY));
		uart_write(UART1,*sp);								// Using the simple send function we send one char at a time
		sp++;												// Increment the pointer to read the next char
		breaking++;											// Checking for chars send
		if(breaking == 193) break;							// Break if 0x00 is not found
	}
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
}

static volatile event_data* get_event(){
	return (events_data + event_number - 1);
}


void mpu_write(char *data){
	send(data);
}