
#include <disp.h>
#include <legacymsp430.h>
#include <uart.h>


int
main (int argc, char *argv[])
{
	WDTCTL = WDTPW + WDTHOLD;               	// Stop WDT
	
	return 0;
}

// ADC10 interrupt service routine
interrupt(ADC10_VECTOR) ADC10_ISR (void)
{
	P1OUT |= BIT6;								// to show that the chip has waked
	__bic_SR_register_on_exit(CPUOFF);			// Clear CPUOFF bit from 0(SR)
	P1OUT &= ~BIT6;
}
