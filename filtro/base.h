/********************************************
*					base.h					*
*********************************************/
#ifndef __BASE_MSP430__
#define __BASE_MSP430__

#include<msp430g2553.h>
#include <msp430.h>

	#ifndef __MSP430G2553__
		#define __MSP430G2553__
	#endif
	
	#if defined(__GNUC__)  &&  defined(__MSP430__)
		#include <legacymsp430.h>
		#define _enable_interrupt() __enable_interrupt()
	#else
		#define interrupt(x)  #pragma vector= (x)\
								__interrupt void
		#define __enable_interrupt() _enable_interrupt()								
	#endif

#define 	B1 					BIT0
#define		B2					BIT1
#define     LED_OUT             P1OUT

typedef
enum boolean
{
	false,
	true
}bool;

enum Interrupts
// to define the interrupts inits
{
	OFF,
	P1_IO,
	P2_IO,
	TIMER0,
	TIMER1,
	AD0,

};

#define delay_ms( d) delay(d)
extern
void delay(unsigned long d);

extern
int init_interrupt (unsigned int code);

#endif
