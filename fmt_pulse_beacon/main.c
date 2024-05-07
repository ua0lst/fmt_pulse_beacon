/*
 * main.c
 *
 * Created: 4/30/2024 11:52:47 PM
 *  Author: ua0lst
 */ 

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

uint8_t is_sig = 0;

void initComparator() {

	CCL.LUT0CTRLB = CCL_INSEL0_IN0_gc     /* IO pin LUTn-IN0 input source */
	| CCL_INSEL1_MASK_gc; /* Masked input */

	CCL.LUT0CTRLA = CCL_CLKSRC_CLKPER_gc     /* Peripheral Clock */
	| CCL_EDGEDET_DIS_gc     /* Edge detector is disabled */
	| CCL_FILTSEL_DISABLE_gc /* Filter disabled */
	| 0 << CCL_ENABLE_bp     /* LUT Enable: disabled */
	| 1 << CCL_OUTEN_bp;     /* Output Enable: enabled */

	CCL.TRUTH2 = 2; /* Truth 2: 2 */

	CCL.LUT2CTRLB = CCL_INSEL0_IN0_gc     /* IO pin LUTn-IN0 input source */
	| CCL_INSEL1_MASK_gc; /* Masked input */

	CCL.LUT2CTRLA = CCL_CLKSRC_CLKPER_gc     /* Peripheral Clock */
	| CCL_EDGEDET_DIS_gc     /* Edge detector is disabled */
	| CCL_FILTSEL_DISABLE_gc /* Filter disabled */
	| 1 << CCL_ENABLE_bp     /* LUT Enable: enabled */
	| 1 << CCL_OUTEN_bp;     /* Output Enable: enabled */

	CCL.CTRLA = 1 << CCL_ENABLE_bp      /* Enable: enabled */
	| 0 << CCL_RUNSTDBY_bp; /* Run in Standby: disabled */
}

void EVENT_SYSTEM_0_init()
{
	EVSYS.CHANNEL0 = EVSYS_CHANNEL0_PORTA_PIN6_gc; /* Configurable Custom Logic LUT2 */
	EVSYS.USERTCA0CNTA = EVSYS_USER_CHANNEL0_gc; /* Connect user to event channel 0 */
	EVSYS.USERTCA0CNTB = EVSYS_USER_CHANNEL0_gc;
}

void TIMER_0_init()
{

	TCA0.SINGLE.EVCTRL = 1 << TCA_SINGLE_CNTBEI_bp | TCA_SINGLE_EVACTB_RESTART_HIGHLVL_gc;

	TCA0.SINGLE.INTCTRL = 0 << TCA_SINGLE_CMP0_bp   /* Compare 0 Interrupt: disabled */
	| 0 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: disabled */
	| 0 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: disabled */
	| 1 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt: enabled */

	TCA0.SINGLE.CNT = 500; /* Period: 0xffff */

	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc      /* System Clock */
	| 1 << TCA_SINGLE_ENABLE_bp    /* Module Enable: enabled */
	| 0 << TCA_SINGLE_RUNSTDBY_bp; /* RUN STANDBY: enabled */
}

ISR(TCA0_OVF_vect)
{

	if ( is_sig ) {
		is_sig = 0;
		PORTB.OUTCLR = PIN3_bm;
	} else { is_sig = 1; PORTB.OUTSET = PIN3_bm; }

	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}


int main(void)
{

	PORTB.DIRSET = PIN3_bm;
	PORTB.OUTSET = PIN3_bm;
	
	PORTB.DIRCLR = PIN2_bm;
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
	//PORTB.PIN2CTRL |= PORT_ISC_FALLING_gc;
	
	initComparator();
	EVENT_SYSTEM_0_init();
	TIMER_0_init();
	sei();
	
    while(1)
    {	
    }
}