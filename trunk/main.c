
#include <disp.h>
#include <legacymsp430.h>
#include <uart.h>
#include <ploc.h>
#include <adc.h>
#include <msp430g2553.h>



#define DEBUG


const unsigned long SMCLK_FREQ = 16000000;
const unsigned long BAUD_RATE = 9600;

int
main (int argc, char *argv[])
{
	WDTCTL = WDTPW + WDTHOLD;               	// Stop WDT
//	BCSCTL1 = CALBC1_16MHZ;                    // Set DCO
	DCOCTL = CALDCO_16MHZ;

	lcd_init();
#ifdef DEBUG
//	startup(9600);
	serial_initialize((SMCLK_FREQ + (BAUD_RATE >> 1)) / BAUD_RATE);
	printf("Debug preparado:\n");
#endif
//	lcd_clear(0xffff);							// fill with white
//	P1OUT |= BIT6;								// ready
//	P1DIR |= BIT6;
//	init_adc();

	while(1)
	{
		//ploc(read_adc());
#ifdef DEBUG
		printf("\r%d",read_adc());
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
