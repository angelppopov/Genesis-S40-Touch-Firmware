#include <asf.h>
#include <delay.h>
#include "serial/serial_ble.h"
#include "serial/serial_mcu.h"

void wtd_enable(void);

int main (void)
{
	/* 
		Note: In board_init function wtd_disable is commented out
	*/	
	board_init();						// Used for dev board
	sysclk_init();						// Initialize System Clock
	wtd_enable();						// Enable Watchdog timer
	
	serial_mcu_init();
	serial_ble_init();
   
	ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
	delay_ms(500);
	ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
	while (1) {
		delay_ms(10);
		wdt_restart(WDT);
	}
}

void wtd_enable(void){
	//Calculate count value for 1s (1,000,000 us) Watchdog
	uint32_t wdtCount = wdt_get_timeout_value(1000000UL, BOARD_FREQ_SLCK_XTAL);
	
	//Initialize Watchdog Timer
	wdt_init(WDT, WDT_MR_WDRSTEN |      // Watchdog fault or underflow causes reset,
	WDT_MR_WDDBGHLT,           // Debug pauses watchdog
	wdtCount,                  // Restart value of Watchdog Counter
	wdtCount);                 // Delta value, used to prevent reset loops, unused here
	
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

