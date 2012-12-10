
#include <disp.h>
#include <legacymsp430.h>
#include <uart.h>
#include <ploc.h>
#include <adc.h>
#include <msp430g2553.h>


#define DEBUG
#define		B1			BIT3	//BOTAO

const unsigned long SMCLK_FREQ = 16000000;
const unsigned long BAUD_RATE = 9600;

int
main (int argc, char *argv[])
{
	unsigned int i=0;
	unsigned int max=0,read;
	
	WDTCTL = WDTPW + WDTHOLD;               	// Stop WDT

    if (CALBC1_16MHZ != 0xFF) {
        /* Adjust this accordingly to your VCC rise time */
        __delay_cycles(100000);

        /* Follow recommended flow. First, clear all DCOx and MODx bits. Then
         * apply new RSELx values. Finally, apply new DCOx and MODx bit values.
         */
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_16MHZ;     /* Set DCO to 16MHz */
        DCOCTL = CALDCO_16MHZ;
    }

	P1DIR = 0;
	P1DIR |= BIT6+BIT0;
	P1OUT &= (~BIT6)+(~BIT0);
	
	// Espera pelo precionar do botão
	while(i<25)
	{
		for(i=0;i<50;i++)
			__delay_cycles(100000);
		P1OUT ^= BIT0;	
		if((P1IN & B1))			//se botão precionado
		{
			P1OUT |= BIT0;
			break;
		}
	}
	
#ifdef DEBUG
//	startup(9600);
	serial_initialize((SMCLK_FREQ + (BAUD_RATE >> 1)) / BAUD_RATE);
	printf("Debug preparado:\n");
	
	P1OUT ^= BIT0;								// ready
	for(i=0;i<10;i++)
			__delay_cycles(100000);
	P1OUT ^= BIT0;								// ready
#endif

	lcd_init();
	lcd_clear(0xffff);							// fill with white
	init_adc();

	while(1)
	{
		ploc(read_adc());
#ifdef DEBUG
		read=read_adc();
		if(read>max)
		{
			max=read;
			printf("\r Maior ate o momento: %d               \n",max);
		}
		printf("\r Leitura no canal 4[P1.4]: %d  ",read);
		P1OUT ^= BIT0;
		for(i=0;i<10;i++)
			__delay_cycles(100000);
#endif
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
