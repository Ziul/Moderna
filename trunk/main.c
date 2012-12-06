
#include <disp.h>
#include <legacymsp430.h>
#include <uart.h>
#include <ploc.h>
#include <adc.h>


#define DEBUG


int
main (int argc, char *argv[])
{
	WDTCTL = WDTPW + WDTHOLD;               	// Stop WDT

	lcd_init();
#ifdef DEBUG
	startup(9600);
#endif
	lcd_clear(0xffff);							// fill with white
	P1OUT |= BIT6;								// ready
	P1DIR |= BIT6;
	init_adc();

	while(1)
	{
		ploc(read_adc());
#ifdef DEBUG
		printf("%d",read_adc());
#endif
	}
	return 0;
	
	return 0;
}

// ADC10 interrupt service routine
interrupt(ADC10_VECTOR) ADC10_ISR (void)
{
	P1OUT |= BIT6;								// to show that the chip has waked
	__bic_SR_register_on_exit(CPUOFF);			// Clear CPUOFF bit from 0(SR)
	P1OUT &= ~BIT6;
}
