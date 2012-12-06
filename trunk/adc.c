#include <adc.h>
#include <legacymsp430.h>
#include <disp.h>


void init_adc(){

	WDTCTL = WDTPW + WDTHOLD;                	// Stop WDT
	
	ADC10CTL1 = CONSEQ_2 + INCH_1;          	// Repeat single channel, A1
	ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE; // ADC10ON, interrupt enable
	ADC10DTC1 = 0;								// no reads yet
	ADC10AE0 |= 0x02; 							// pin P1.1

}

int read_adc(){
	ADC10DTC1 = 1;							// one read
	ADC10CTL0 &= ~ENC;
	while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
	ADC10SA = 0x200;                        // Data buffer address
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	return ADC10MEM;
}
