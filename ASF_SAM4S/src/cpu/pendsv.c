/*
 * pendsv.c
 *
 * Created: 2/24/2020 9:54:51 AM
 *  Author: Angel Popov
 */ 

#include "cpu.h"


__INLINE void xPortPendSVHandler(void)
{

	/* Lazy stacking http://infocenter.arm.com/help/topic/com.arm.doc.dai0298a/DAI0298A_cortex_m4f_lazy_stacking_and_context_switching.pdf */
	/* 
		During context switching, which usually takes place within the PendSV exception, the
		processor executes the VSTM instruction (also known as the Floating-point Store Multiple
		command), which stores multiple floating-point registers to memory to store the S16 to S31
		registers to the context data in the process stack or Task Control Block (TCB), depending on OS
		implementation
	*/
	
// 	__ASM volatile
// 	(
// 	"       mrs r0, psp                     \n"
// 	"       isb                             \n"
// 	"                                       \n"
// 	"       ldr     r3, pxCurrentTCBConst   \n" /* Get the location of the current TCB. */
// 	"       ldr     r2, [r3]                \n"
// 	"                                       \n"
// 	"       tst r14, #0x10                  \n" /* Is the task using the FPU context?  If so, push high vfp registers. */
// 	"       it eq                           \n"
// 	"       vstmdbeq r0!, {s16-s31}         \n"
// 	"                                       \n"
// 	"       stmdb r0!, {r4-r11, r14}        \n" /* Save the core registers. */
// 	"       str r0, [r2]                    \n" /* Save the new top of stack into the first member of the TCB. */
// 	"                                       \n"
// 	"       stmdb sp!, {r0, r3}             \n"
// 	"       mov r0, %0                      \n"
// 	"       msr basepri, r0                 \n"
// 	"       dsb                             \n"
// 	"       isb                             \n"
// 	"       bl vTaskSwitchContext           \n"
// 	"       mov r0, #0                      \n"
// 	"       msr basepri, r0                 \n"
// 	"       ldmia sp!, {r0, r3}             \n"
// 	"                                       \n"
// 	"       ldr r1, [r3]                    \n" /* The first item in pxCurrentTCB is the task top of stack. */
// 	"       ldr r0, [r1]                    \n"
// 	"                                       \n"
// 	"       ldmia r0!, {r4-r11, r14}        \n" /* Pop the core registers. */
// 	"                                       \n"
// 	"       tst r14, #0x10                  \n" /* Is the task using the FPU context?  If so, pop the high vfp registers too. */
// 	"       it eq                           \n"
// 	"       vldmiaeq r0!, {s16-s31}         \n"
// 	"                                       \n"
// 	"       msr psp, r0                     \n"
// 	"       isb                             \n"
// 	"                                       \n"
// 	"                                       \n"
// 	"       bx r14                          \n"
// 	"                                       \n"
// 	"       .align 4                        \n"
// 	"pxCurrentTCBConst: .word pxCurrentTCB  \n"
// 	::"i"(configMAX_SYSCALL_INTERRUPT_PRIORITY)
// 	);
}

int pxCurrentTCBConst(){
	
}

int pxCurrentTCB(){
	
}