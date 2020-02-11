/*
 * touch_panel.h
 *
 * Created: 2/10/2020 9:49:31 AM
 *  Author: Angel Popov
 */ 


#ifndef TOUCH_PANEL_H_
#define TOUCH_PANEL_H_

void touch_panel_init(void);
static void touch_detected(char *pin, int size);
static void leds(char *data);
/* Setup hardware */
static void interruptPortB(void);

#endif /* TOUCH_PANEL_H_ */