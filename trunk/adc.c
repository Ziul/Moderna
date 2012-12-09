#include <adc.h>
#include <legacymsp430.h>
#include <disp.h>

#ifdef MY_ADC
void init_adc()
{

	WDTCTL = WDTPW + WDTHOLD;                	// Stop WDT
	
	ADC10CTL1 = CONSEQ_2 + INCH_4;          	// Repeat single channel, A4
	ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE; // ADC10ON, interrupt enable
	ADC10DTC1 = 0;								// no reads yet
	P1DIR &= ~BIT4;								// set as input
	ADC10AE0 |= BIT4; 							// pin P1.4

}
#else
void init_adc()
{
    /* disable ADC10 during initialization */
    ADC10CTL0 &= ~ENC;

    /* 
     * Control Register 0
     * 
     * ~ADC10SC -- No conversion
     * ~ENC -- Disable ADC
     * ~ADC10IFG -- Clear ADC interrupt flag
     * ADC10IE -- Enable ADC interrupt
     * ADC10ON -- Switch On ADC10
     * ~REFON -- Disable ADC reference generator
     * ~REF2_5V -- Set reference voltage generator = 1.5V
     * ~MSC -- Disable multiple sample and conversion
     * ~REFBURST -- Reference buffer on continuously
     * ~REFOUT -- Reference output off
     * ~ADC10SR -- Reference buffer supports up to ~200 ksps
     * ADC10SHT_0 -- 4 x ADC10CLKs
     * SREF_0 -- VR+ = VCC and VR- = VSS
     * 
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    ADC10CTL0 = ADC10IE + ADC10ON + ADC10SHT_0 + SREF_0;
    
    /* 
     * Control Register 1
     * 
     * ~ADC10BUSY -- No operation is active
     * CONSEQ_0 -- Single channel single conversion
     * ADC10SSEL_0 -- ADC10OSC
     * ADC10DIV_0 -- Divide by 1
     * ~ISSH -- Input signal not inverted
     * ~ADC10DF -- ADC10 Data Format as binary
     * SHS_0 -- ADC10SC
     * INCH_4 -- ADC Channel 4
     * 
     * Note: ~<BIT> indicates that <BIT> has value zero
     */
    ADC10CTL1 = CONSEQ_0 + ADC10SSEL_0 + ADC10DIV_0 + SHS_0 + INCH_4;
    
    /* enable ADC10 */
    ADC10CTL0 |= ENC;
   
}
#endif

int read_adc(){
	ADC10DTC1 = 1;							// one read
	ADC10CTL0 &= ~ENC;
	while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
	ADC10SA = 0x200;                        // Data buffer address
	ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	return ADC10MEM;
}
