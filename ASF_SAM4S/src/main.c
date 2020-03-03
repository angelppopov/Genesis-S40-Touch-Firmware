#include <asf.h>
#include <delay.h>
#include "touch_panel/touch_panel.h"
#include "serial/serial_ble.h"
#include "serial/serial_mcu.h"
#include "scheduler/scheduler.h"
#include "memory/sd_card.h"
#include "memory/nvm.h"
#include "utils/utils.h"

void wtd_enable(void);
extern event_scheduler scheduler;


int main (void)
{
	/* 
		Note: In board_init function wtd_disable is commented out
	*/	
	board_init();						// Used for dev board
	sysclk_init();						// Initialize System Clock
	
	scheduler_init();
	serial_mcu_init();
	serial_ble_init();
	touch_panel_init();
	sd_card_init();
	nvmem_init();
	
	wtd_enable();						// Enable Watchdog timer
	
	printf("Working \n\n");
	
	char data[] = "strtok needs to be called several times to split a string strtok needs to be called several times to split a string strtok needs to be called several times to split a stringstrtok needs to be called several times to split a string strtok needs to be called several times to split a string strtok needs to be called several times to split a stringstrtok needs to be called several times to split a string strtok needs to be called several times to split a string strtok needs to be called several times to split a stringstrtok needs to be called several times to split a string strtok needs to be called several times to split a string strtok needs to be called several times to split a stringstrtok needs to be called several times to split a string strtok needs to be called several times to split a string strtok needs to be called several times to split a string";
	nodes *nodes_data = get_command_notes(data);
	free(nodes_data);
	
	
	while (1) {
		scheduler.process();
		wdt_restart(WDT);
		/* Is button pressed? Test SD Card Write */
		if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			/* Write on SD Card */
			scheduler.add(3, 1);
			delay_ms(1000);
		}
	}
}

void wtd_enable(void){
	//Calculate count value for 3s (3,000,000 us) Watchdog
	uint32_t wdtCount = wdt_get_timeout_value(3000000UL, BOARD_FREQ_SLCK_XTAL);
	
	//Initialize Watchdog Timer
	wdt_init(WDT, WDT_MR_WDRSTEN |      // Watchdog fault or underflow causes reset,
	WDT_MR_WDDBGHLT,                    // Debug pauses watchdog
	wdtCount,                           // Restart value of Watchdog Counter
	wdtCount);                          // Delta value, used to prevent reset loops, unused here
	
}


// 	ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
// 	delay_ms(500);
// 	ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);