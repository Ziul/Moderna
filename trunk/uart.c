/**
 * @file
 * @author  Luiz Oliveira <ziuloliveira@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This firmware will allow any MSP430 with USCI to send/recive data under RS232;
 * It was tested only under MSP430G2553
 */

#include <myerrno.h>

#ifndef __UART_MSP__
#define __UART_MSP__  


//#ifdef __MSP430_HAS_USCI__ 
#include <uart.h>


/**
 * Start up the UART
 *
 * @param	baud	The baud rate of the UART
 * @return			SUCCESS if the UART is Started up right
 *
 * An inline equation @f$ e^{\pi i}+1 = 0 @f$
 *
 * A displayed equation: @f[ N=\frac{BRCLK}{Baud Rate} @f]
 *
 */
int startup (long int baud)
{
//  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 = CALBC1_16MHZ;                    // Set DCO
  DCOCTL = CALDCO_16MHZ;
 
  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = (long int)(16000000/baud); //104;                         // 16MHz 9600
  UCA0BR1 = ((long int)(16000000/baud) ) >> 8;                        // 16MHz 9600
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
//	IE2 |= UCA0TXIE;                       // Enable USCI_A0 TX interrupt
//	IE2 &= ~UCA0RXIE;                       // Disable USCI_A0 RX interrupt
	
	return SUCCESS;
}

/**
 * serial_initialize(bitclk_divisor) - configure USCI UCA0 as async serial port
 *  uses SMCLK as bitclock source and fractional divisor with over sampling mode.
 *
 * @params:
 *   uint32_t bitclk_divisor - should be (SMCLK_FREQ + (BPS >> 1)) / BPS
 *
 * Thanks to Kevin for original code from 43oh.com:
 * @see http://www.43oh.com/forum/viewtopic.php?f=10&t=2493
 *
 * P1.1 - RX
 * P1.2 - TX
 *
 */
void serial_initialize(const uint32_t bitclk_divisor)
{
    UCA0CTL1 = UCSWRST; // Hold USCI in reset to allow configuration
    UCA0CTL0 = 0; // No parity, LSB first, 8 bits, one stop bit, UART (async)
    UCA0BR1 = (bitclk_divisor >> 12) & 0xFF; // High byte of whole divisor
    UCA0BR0 = (bitclk_divisor >> 4) & 0xFF; // Low byte of whole divisor
    UCA0MCTL = ((bitclk_divisor << 4) & 0xF0) | UCOS16; // Fractional divisor, over sampling mode
    UCA0CTL1 = UCSSEL_2; // Use SMCLK for bit rate generator, release reset

    P1SEL = BIT1 | BIT2; // P1.1=RXD, P1.2=TXD
    P1SEL2 = BIT1 | BIT2; // P1.1=RXD, P1.2=TXD
}

void Initialize(void)
{


}

/**
 * Function used to send a char on the standard output. Used by the functions from stdio.h.
 * This function define which will be the standard output
 *
 * @param	c	 Char which will be print
 * @return			The char printed as a integer.
 */
int putchar(volatile int c)
{
	if(UCA0CTL1 & UCSWRST) return EOF; // USCI not active

	while (!(IFG1 & UCA0TXIFG)); // wait for USART TX buffer ready
	UCA0TXBUF = c;

IE2 |= UCA0TXIE;                       // Enable USCI_A0 TX interrupt
__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled

	return c;
}


/**
 * Function used to get a char from the standard input. Used by the functions from stdio.h.
 * This function define which will be the standard input
 *
 * @return			The char got as a integer.
 */
int getchar()
{
	while (!(IFG2&UCA0RXIFG));			// Wait recive a data

	return UCA0RXBUF=='\r'?'\n' : UCA0RXBUF;
}


/**
 * Function used to send a char to the standard output.
 *
 * @param	*msg	 String which will be print.
 * @param	size	 Size from the string.
 * @return			SUCCESS if printed. EINVAL if *msg is NULL
 */
int send (const char *msg,int size)
{
int count=0;
	if(!msg)								/* Null msg */
		return EINVAL;

//	while(msg[count] != '\0' )
	while(count != size )
		putchar(msg[count++]);

	IE2 &= ~UCA0TXIE;                       // Disable USCI_A0 TX interrupt

	return SUCCESS;
}


/**
 * Function used to recive a char to the standard input. 
 *
 * @param[in,out] *msg	 String which will be read froms inpust standard.
 * @param		size	 Size max of the string.
 * @return				How many characters it got.
 */
int recive (char *msg, int size)
{
 int i=0;
 
 	while(i<size)
 		msg[i++]=getchar();
 		
 	return i;

}

/**
 * Function used to recive a char to the standard input. 
 * \bug The sizeof() is returning a wrong size. That is why still needing the size of the pointer
 *
 * @param[in]	*s	 String which will be read froms inpust standard.
 * @param		size	 Size max of the string.
 * @return				How the string got.
 */

char *gets(char *s,int size)
{
	int b=0;
	int i=0;
	
	if(!s)
		return s;
	size--;
	do
	{
		b=getchar();
		if(b!=8)			//backspace
			s[i++]=b;
		else
			i--;
#ifdef PROMP_OUT			
		if(b==8)
		{
			putchar(b);		//back
			putchar(' ');	//clean the char
			putchar(b);		//back again
		}
		else
			putchar(b);
#endif
		if( (s[i-1]=='\n') )
			break;
	}while(i<size);
	
	s[i]='\0';

	return s;
}


/**
 * Interrupr function from TX of USCI 0
 * It's just commute P1.0 ( OUTPUT LED )
 */
interrupt(USCIAB0TX_VECTOR) USCI0TX_ISR(void)
{
	P1OUT ^= BIT0;
	__bic_SR_register_on_exit(LPM0_bits + GIE);
}

//#endif

/*#ifndef __MSP430_HAS_USCI__ 
int putchar(volatile int c)
{return ENXIO;}

int getchar()
{return ENXIO;}

int startup (long int baud)
{return ENODEV;}
#endif
*/

#endif
