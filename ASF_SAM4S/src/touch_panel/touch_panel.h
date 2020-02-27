/*
 * touch_panel.h
 *
 * Created: 2/25/2020 11:40:06 AM
 *  Author: Angel Popov
 */ 


#ifndef TOUCH_PANEL_H_
#define TOUCH_PANEL_H_

void touch_panel_init(void);
static void touch_detected(char *pin, int size);
static void leds(char *data);
/* Setup hardware */
static void enable_interrupt(void);

#endif /* TOUCH_PANEL_H_ */