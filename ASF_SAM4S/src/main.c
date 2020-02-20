#include <asf.h>
#include "serial/serial_ble.h"
#include "serial/serial_mcu.h"

int main (void)
{
	
	board_init();						// Used for dev board
	sysclk_init();						// Initialize System Clock
	wdt_disable(WDT);					// Disable watchdog timer
	//Peripheral Setup
	serial_mcu_init();
	serial_ble_init();
	
	while (1) {

	}
}





// 		/* Is button pressed? */
// 		if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
// 			/* Yes, so turn LED on. */
// 			ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
// 			send(UART1, data);
// 			send(UART0, data);
// 		} else {
// 			/* No, so turn LED off. */
// 			ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
// 		}

