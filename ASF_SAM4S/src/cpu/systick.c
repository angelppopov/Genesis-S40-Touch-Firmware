/*
 * systick.c
 *
 * Created: 2/24/2020 9:44:43 AM
 *  Author: Angel Popov
 */ 

#include "cpu.h"

#define  pdFALSE true

static void portDISABLE_INTERRUPTS();
static void portENABLE_INTERRUPTS();
static bool xTaskIncrementTick();


void xPortSysTickHandler( void )
{
    /* The SysTick runs at the lowest interrupt priority, so when this interrupt
    executes all interrupts must be unmasked.  There is therefore no need to
    save and then restore the interrupt mask value as its value is already
    known. */
	
    portDISABLE_INTERRUPTS();
    {
        /* Increment the RTOS tick. */
        if( xTaskIncrementTick() != pdFALSE )
        {
            /* A context switch is required.  Context switching is performed in
            the PendSV interrupt.  Pend the PendSV interrupt. */
            portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;
        }
    }
    portENABLE_INTERRUPTS();
}


static bool xTaskIncrementTick(){
	
}

static void portDISABLE_INTERRUPTS(){
	
}
static void portENABLE_INTERRUPTS(){
	
}