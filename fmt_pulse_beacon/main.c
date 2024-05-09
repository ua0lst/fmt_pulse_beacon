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

void TIMER_TCA1_init()
{
	TCA1.SINGLE.CNT = 10000;
	TCA1.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA1.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc | 1 << TCA_SINGLE_ENABLE_bp;
}


void TIMER_0_init()
{

	TCA0.SINGLE.EVCTRL = 1 << TCA_SINGLE_CNTBEI_bp | TCA_SINGLE_EVACTB_RESTART_ANYEDGE_gc;

	TCA0.SINGLE.INTCTRL = 0 << TCA_SINGLE_CMP0_bp   /* Compare 0 Interrupt: disabled */
	| 0 << TCA_SINGLE_CMP1_bp /* Compare 1 Interrupt: disabled */
	| 0 << TCA_SINGLE_CMP2_bp /* Compare 2 Interrupt: disabled */
	| 1 << TCA_SINGLE_OVF_bp; /* Overflow Interrupt: enabled */

	TCA0.SINGLE.CNT = 55100; /* Period: 0xffff */

	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc      /* System Clock */
	| 1 << TCA_SINGLE_ENABLE_bp    /* Module Enable: enabled */
	| 0 << TCA_SINGLE_RUNSTDBY_bp; /* RUN STANDBY: enabled */
}

/*ISR(TCA0_OVF_vect)
{

	if ( is_sig ) {
		is_sig = 0;
		PORTB.OUTCLR = PIN3_bm;
	} else { is_sig = 1; PORTB.OUTSET = PIN3_bm; }

	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}*/

ISR(TCA1_OVF_vect)
{

	if ( is_sig ) {
		is_sig = 0;
		PORTB.OUTCLR = PIN3_bm;
		} else { is_sig = 1; PORTB.OUTSET = PIN3_bm; }

		TCA1.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
		
		//PORTB.OUTCLR = PIN3_bm;
		//TCA1.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
		//TCA1.SINGLE.CNT = 0;
	}

ISR(PORTA_PORT_vect) {
	
	PORTB.OUTSET = PIN3_bm;
	//TCA1.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

int main(void)
{

	PORTB.DIRSET = PIN3_bm;
	PORTB.OUTSET = PIN3_bm;
	
	PORTB.DIRCLR = PIN2_bm;
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
	PORTB.PIN2CTRL |= PORT_ISC_BOTHEDGES_gc;
	
	PORTA.DIRCLR = PIN2_bm;
	//PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
	//PORTA.PIN2CTRL |= PORT_ISC_BOTHEDGES_gc;
	
	//TIMER_0_init();
	//TIMER_TCA1_init();
	sei();
	
    while(1)
    {
		if (PORTA.IN & PIN2_bm)
		{
			PORTB.OUTCLR = PIN3_bm;
			//TCA1.SINGLE.CNT = 0;
		} else { PORTB.OUTSET = PIN3_bm; }
    }
}