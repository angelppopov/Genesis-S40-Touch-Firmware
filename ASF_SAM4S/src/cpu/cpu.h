/*
 * cpu.h
 *
 * Created: 2/24/2020 9:45:35 AM
 *  Author: Angel Popov
 */ 


#ifndef CPU_H_
#define CPU_H_

#include <asf.h>

#define vPortSVCHandler             SVC_Handler
#define xPortPendSVHandler          PendSV_Handler
#define xPortSysTickHandler         SysTick_Handler

#define portNVIC_INT_CTRL_REG		( * ( ( volatile uint32_t * ) 0xe000ed04 ) )
#define portNVIC_PENDSVSET_BIT		( 1UL << 28UL )

//#define portENABLE_INTERRUPTS() 	__asm volatile ( "MVTIPL #0" )
//#define portDISABLE_INTERRUPTS() 	__asm volatile ( "MVTIPL #0" )
static void prvRestoreContextOfFirstTask( void );

__INLINE void xPortPendSVHandler(void);
void xPortSysTickHandler(void);
int configMAX_SYSCALL_INTERRUPT_PRIORITY(void);

#endif /* CPU_H_ */