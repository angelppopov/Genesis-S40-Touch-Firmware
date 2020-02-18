/*
 * timer.h
 *
 * Created: 2/12/2020 4:42:12 PM
 *  Author: angelpopov
 */ 


#ifndef TIMER_H_
#define TIMER_H_
#define sw_set				PORTQ.DIR &= ~(1<<PIN2_bp)
#define sw_pull_up			PORTQ.PIN2CTRL |= PORT_OPC_PULLUP_gc
#define sw_high				PORTQ_OUT |= (1<<PIN2_bp)
#define sw					!(PORTQ.IN & PIN2_bm)
void init_hardware_interrupts(void);

#endif /* TIMER_H_ */