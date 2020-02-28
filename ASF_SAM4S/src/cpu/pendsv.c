/*
 * pendsv.c
 *
 * Created: 2/24/2020 9:54:51 AM
 *  Author: Angel Popov
 */ 

#include "cpu.h"
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 0

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
	/* This is a naked function. */

// 	__ASM volatile
// 	(
// 	"	mrs r0, psp							\n"
// 	"	isb									\n"
// 	"										\n"
// 	"	ldr	r3, pxCurrentTCBConst			\n" /* Get the location of the current TCB. */
// 	"	ldr	r2, [r3]						\n"
// 	"										\n"
// 	"	tst r14, #0x10						\n" /* Is the task using the FPU context?  If so, push high vfp registers. */
// 	"	it eq								\n"
// 	"	vstmdbeq r0!, {s16-s31}				\n"
// 	"										\n"
// 	"	mrs r1, control						\n"
// 	"	stmdb r0!, {r1, r4-r11, r14}		\n" /* Save the remaining registers. */
// 	"	str r0, [r2]						\n" /* Save the new top of stack into the first member of the TCB. */
// 	"										\n"
// 	"	stmdb sp!, {r0, r3}					\n"
// 	"	mov r0, %0							\n"
// 	"	msr basepri, r0						\n"
// 	"	dsb									\n"
// 	"	isb									\n"
// 	"	bl vTaskSwitchContext				\n"
// 	"	mov r0, #0							\n"
// 	"	msr basepri, r0						\n"
// 	"	ldmia sp!, {r0, r3}					\n"
// 	"										\n" /* Restore the context. */
// 	"	ldr r1, [r3]						\n"
// 	"	ldr r0, [r1]						\n" /* The first item in the TCB is the task top of stack. */
// 	"	add r1, r1, #4						\n" /* Move onto the second item in the TCB... */
// 	"	ldr r2, =0xe000ed9c					\n" /* Region Base Address register. */
// 	"	ldmia r1!, {r4-r11}					\n" /* Read 4 sets of MPU registers. */
// 	"	stmia r2!, {r4-r11}					\n" /* Write 4 sets of MPU registers. */
// 	"	ldmia r0!, {r3-r11, r14}			\n" /* Pop the registers that are not automatically saved on exception entry. */
// 	"	msr control, r3						\n"
// 	"										\n"
// 	"	tst r14, #0x10						\n" /* Is the task using the FPU context?  If so, pop the high vfp registers too. */
// 	"	it eq								\n"
// 	"	vldmiaeq r0!, {s16-s31}				\n"
// 	"										\n"
// 	"	msr psp, r0							\n"
// 	"	bx r14								\n"
// 	"										\n"
// 	"	.align 4							\n"
// 	"pxCurrentTCBConst: .word pxCurrentTCB	\n"
// 	::"i"(configMAX_SYSCALL_INTERRUPT_PRIORITY)
// 	);
}


static void prvRestoreContextOfFirstTask( void )
{
	__ASM volatile
	(
	"	ldr r0, =0xE000ED08				\n" /* Use the NVIC offset register to locate the stack. */
	"	ldr r0, [r0]					\n"
	"	ldr r0, [r0]					\n"
	"	msr msp, r0						\n" /* Set the msp back to the start of the stack. */
	"	ldr	r3, pxCurrentTCBConst2		\n" /* Restore the context. */
	"	ldr r1, [r3]					\n"
	"	ldr r0, [r1]					\n" /* The first item in the TCB is the task top of stack. */
	"	add r1, r1, #4					\n" /* Move onto the second item in the TCB... */
	"	ldr r2, =0xe000ed9c				\n" /* Region Base Address register. */
	"	ldmia r1!, {r4-r11}				\n" /* Read 4 sets of MPU registers. */
	"	stmia r2!, {r4-r11}				\n" /* Write 4 sets of MPU registers. */
	"	ldmia r0!, {r3-r11, r14}		\n" /* Pop the registers that are not automatically saved on exception entry. */
	"	msr control, r3					\n"
	"	msr psp, r0						\n" /* Restore the task stack pointer. */
	"	mov r0, #0						\n"
	"	msr	basepri, r0					\n"
	"	bx r14							\n"
	"									\n"
	"	.align 4						\n"
	"pxCurrentTCBConst2: .word pxCurrentTCB	\n"
	);
}