/************************************************************************************
*										main.c										*
*************************************************************************************/
#include "disp.h"
#include "ploc.h"
#include "legacymsp430.h"


int main (int argc, char *argv[])
{
	WDTCTL = WDTPW + WDTHOLD;               	// Stop WDT
	ADC10CTL1 = CONSEQ_2 + INCH_1;          	// Repeat single channel, A1
	ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE; // ADC10ON, interrupt enable
	ADC10DTC1 = 0;								// no reads yet
	ADC10AE0 |= 0x02;                       	// P1.1 ADC option select
	
	P1DIR |= BIT6;								// Set P1.0 to output direction
	lcd_init();
	lcd_clear(0xffff);							// fill with white
	P1OUT |= BIT6;								// ready
	while(1)
	{
		ploc(ADC10MEM);							// print adc value on display
		ADC10DTC1 = 1;							// one read
		ADC10CTL0 &= ~ENC;
		while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
		ADC10SA = 0x200;                        // Data buffer address
		ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
		__bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
	}
	return 0;
}

// ADC10 interrupt service routine
interrupt(ADC10_VECTOR) ADC10_ISR (void)
{
	P1OUT |= BIT6;								// to show that the chip has waked
	__bic_SR_register_on_exit(CPUOFF);			// Clear CPUOFF bit from 0(SR)
	P1OUT &= ~BIT6;
}
