/*
 * dma.c
 *
 * Created: 2/18/2020 7:53:39 AM
 *  Author: Angel Popov
 */ 

#include "dma.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void dma_init(void){
	/* Memory Block Copy */
	
	/*
	Set   the   Enable bit (ENABLE) in the DMA Control register (CTRL) 
	to enable the DMAC with default settings for priority.
	*/
	DMA.CTRL |= DMA_CH_ENABLE_bm;
	/*
	Set   the   Transfer Mode bitfield (BURSTLEN) in Channel 0 Control Register (CTRL) 
	equal to 0x03 to select eight-byte burst mode.
	*/
	DMA.CH0 |= DMA_CH_BURSTLEN_gm;
	/*
	ADDRCTRL – Address Control register pg.64 of XMEGA AU datasheet register description
	
	BIT: 7           6  5        4  3            2  1         0
         SRCRELOAD[1:0] SRCDIR[1:0] DESTRELOAD[1:0] DESTDIR[1:0]
	
	*/
	/*
	Set   the   Source Address Mode bitfield (SRCDIR) in Channel 0 Address Control register (ADDRCTRL) 
	equal to 0x01 to make the source address increment after each byte access.
	*/
	DMA.CH0.ADDRCTRL |= DMA_CH_SRCDIR1_bm;
    /* 
	Set   the   Destination Address Mode bitfield (DESTDIR) in Channel 0 Address Control register (ADDRCTRL) 
	equal to 0x01 to make the destination address increment after each byte access. 
    */
	DMA.CH0.ADDRCTRL |= DMA_CH_DESTDIR1_bm;
    /*
	Set   the   Channel 0 Trigger Source register (TRIGSRC) equal to 0x00 to select manual trigger source.
    */
	DMA.CH0.TRIGSRC &= ~DMA_CH_TRIGSRC0_bm;
    /*
	Set   the   Channel 0 Block Transfer Count register (TRFCNT) 
	equal to 0xFF to set the block size to 255 bytes.
    */
	DMA.CH0.TRFCNT = 0xFF;
	/*
	Set   the   Channel 0 Source Address register (SRCADDR) to point to the start of the source array
    */
	DMA.CH0.SRCADDR0 = 0x1;
    /*
	Set   the   Channel 0 Destination Address register (DESTADDR) 
	to point to the start of the destination array
	*/
	DMA.CH0.DESTADDR0 = 0x0;
	/*
	Set   the   Enable bit (ENABLE) in Channel 0 Control register (CTRL) to enable DMA channel 0.
    */
	DMA.CH0 |= DMA_CH_ENABLE_bm;
    /*
    Set   the   Transfer Request bit (TRFREQ) in Channel 0 Control register (CTRL) to start the transaction.
    */
	DMA.CH0.CTRLA = DMA_CH_TRFREQ_bm;

    /*
	Wait for the Transaction Complete Interrupt Flag bit for channel 0 (CH0TRNIF) 
	in the Transfer Interrupt Status register (INTFLAGS) to be set, 
	indicating that the transaction is finished. 
	Clear the flag afterwards by writing logic one to it.
	*/

}

ISR(DMA_CH0_vect){
	
}