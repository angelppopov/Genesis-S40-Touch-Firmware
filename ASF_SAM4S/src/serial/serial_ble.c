/*
 * serial_ble.c
 *
 * Created: 2/20/2020 10:03:47 AM
 *  Author: Angel Popov
 */ 

#include "serial_ble.h"

void serial_ble_init(void){
	//Enable Rx Ready Interrupt on UART0
	uart_enable_interrupt(UART0, UART_IER_RXRDY);
	//Register Interrupt with Priority 0
	irq_register_handler(UART0_IRQn, 1);
	
	//Allow UART to control PA9 and PA10
	pio_set_peripheral(PIOA, PIO_PERIPH_A, PIO_PA9A_URXD0 | PIO_PA10A_UTXD0);

	//Enable UART1 Clock
	sysclk_enable_peripheral_clock(ID_UART0);

	//UART1 Settings (configures desired baudrate and sets no parity)
	const sam_uart_opt_t uart0Settings = {sysclk_get_cpu_hz(), 9600UL, UART_MR_PAR_NO};
	//Setup UART1
	uart_init(UART0, &uart0Settings);

	//Enable Rx Ready Interrupt on UART1
	uart_enable_interrupt(UART0, UART_IER_RXRDY);
	//Register Interrupt with Priority 0
	irq_register_handler(UART0_IRQn, 1);
	
	
}

ISR(UART0_Handler){
	uint8_t rxData;
	//If data received
	if((uart_get_status(UART0) & UART_SR_RXRDY) == UART_SR_RXRDY)
	{
		//Read in the byte
		uart_read(UART0, &rxData);
		//Wait for tx free
		while (!(UART0->UART_SR & UART_SR_TXRDY));
		//Echo the received byte
		uart_write(UART0, rxData);
	}
}