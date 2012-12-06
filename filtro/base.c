/********************************************
*					base.c					*
*********************************************/

#include "base.h"

void delay(unsigned long d) // max = 8388607 or get a overflow
{
	BCSCTL1= CALBC1_16MHZ;		//Set up the processor to run at a calibrated 16MHz rate.
	DCOCTL= CALDCO_16MHZ;

	//d*=512;

    while (d--)
    {
        __delay_cycles(16000);//30
    }
}

/*****************************************
			Interrupções
*****************************************/

int init_interrupt (unsigned int code)
{

	switch(code)
	{
		case OFF:
			__disable_interrupt();		
			return 0;
		break;
		
		case P1_IO:
			P1DIR &= ~(B1|B2);
			P1REN |= B1|B2;
			P1OUT |= B1|B2;
			P1IES |= B1|B2;   // high -> low is selected with IES.x = 1.
			P1IFG &= ~(B1|B2);  // To prevent an immediate interrupt, clear the flag for
		                 // P1.3 before enabling the interrupt.
			P1IE |= B1|B2;    // Enable interrupts for P1.3		
		break;

		case P2_IO:
			P2DIR &= ~(B1|B2);
			P2REN |= B1|B2;
			P2OUT &= ~(B1|B2);
			P2IES |= B1|B2;   // high -> low is selected with IES.x = 1.
			P2IFG &= ~(B1|B2);  // To prevent an immediate interrupt, clear the flag for
		                 // P1.3 before enabling the interrupt.
			P2IE |= B1|B2;    // Enable interrupts for P1.3		
		break;

		case TIMER0:
			BCSCTL1= CALBC1_16MHZ;		//Set up the processor to run at a calibrated 1MHz rate.
			DCOCTL= CALDCO_16MHZ;
			TA0CTL |= TASSEL_2 | MC_1 | ID_3 | TAIE | TACLR;    // Reset it, first. Up to TACCR
			TA0CCTL0 = CCIE;
		break;
		
		case TIMER1:
			BCSCTL1= CALBC1_16MHZ;		//Set up the processor to run at a calibrated 1MHz rate.	
			DCOCTL= CALDCO_16MHZ;
			TA1CTL |= TASSEL_2 | MC_1 | ID_3 | TAIE | TACLR;    // Reset it, first. Up to TACCR
			TA1CCTL0 = CCIE;
		break;
		
		default:
			return -1;		//code unknow
	}

	__enable_interrupt();		// Enable global interrupt

	return 0;
}

// P1_ISR
/*
interrupt(PORT1_VECTOR) P1_ISR(void)
{
    switch(P1IFG&(B1|B2)) 
    {
        case B1:
            P1IFG &= ~B1;    // clear the interrupt flag
            P1OUT &= ~(BIT0|BIT6);
            return;
            
        case B2:
            P1IFG &= ~B2;    // clear the interrupt flag
            P1OUT &= ~(BIT0|BIT6);
            return;
            
        default:			// two or more glafs is up
            P1IFG = 0;    // probably unnecessary, but if another flag occurs
                          // in P1, this will clear it.  No error handling is
                          // provided this way, though.
            return;
    }
}
*/

// P2_ISR
/*
interrupt(PORT2_VECTOR) P2_ISR(void) 
{
    switch(P2IFG&(B1|B2)) 
    {
        case B1:
            P2IFG &= ~B1;    // clear the interrupt flag
 //           P2OUT &= ~(BIT0|BIT6);
 			estado=verde;
            return;
            
        case B2:
            P2IFG &= ~B2;    // clear the interrupt flag
//            P2OUT &= ~(BIT0|BIT6);
			estado=vermelho;
            return;
            
        default:			// two or more glafs is up
            P1IFG = 0;    // probably unnecessary, but if another flag occurs
                          // in P1, this will clear it.  No error handling is
                          // provided this way, though.
            return;
    }
} 
*/

//void _RESET(void);	// to yourself made reset function.

/*
interrupt(TIMER0_A0_VECTOR) CCR0_ISR(void)
{
//	LED_OUT ^= (LED0 + LED1); //Toggle both LEDs
	
	TACTL &= ~TAIFG;	// clear flag
	TA1CTL |= TACLR;	// clear TAR0
}
*/
