#include <msp430g2553.h>
#include "disp.h"
/*
 * main.c
 */
#define B1 BIT3

uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;


void main(void) {
	 WDTCTL = WDTPW + WDTHOLD;
	 P1REN |= B1;
	 P1OUT |= B1;
	 P1IES |= B1;   // high -> low is selected with IES.x = 1.
	 P1IFG &= ~B1;  // To prevent an immediate interrupt, clear the flag for
	                  // P1.3 before enabling the interrupt.
	 P1IE |= B1;    // Enable interrupts for P1.3

	 _enable_interrupt();  // Note the singular name here!  It's not interrupts.


	 lcd_init();																	//Inicia o LCD, a parte acima é para usar o botao nao é necessario para o LCD
	 lcd_clear(backcolor);															//Limpa a tela com a cor desejada, essa é definida no disp.c


	 lcd_linha(BLUE,20,170,60,150,1);				//Exemplo de linha
	 lcd_linha(RED,60,170,20,150,1);

	 lcd_circulo(PINK,40,100,20,1);					//Exemplo de um circulo, na ordem de comandos: cor, x, y, rotacao ; essa ultima é pra escolher a forma de ver o display se deitado ou em pé


	 lcd_retangulo(WHITE,10,10,100,10,0);			//exemplos de retangulos com as cores definidas
	 lcd_retangulo(BLACK,20,10,100,10,0);
	 lcd_retangulo(RED,30,10,100,10,0);
	 lcd_retangulo(GREEN,40,10,100,10,0);
	 lcd_retangulo(BLUE,50,10,100,10,0);
	 lcd_retangulo(CYAN,60,10,100,10,0);
	 lcd_retangulo(YELLOW,70,10,100,10,0);
	 lcd_retangulo(BROWN,80,10,100,10,0);
	 lcd_retangulo(ORANGE,90,10,100,10,0);
	 lcd_retangulo(PINK,100,10,100,10,0);
	 lcd_retangulo(GOLD,110,10,100,10,0);
	 lcd_retangulo(SILVER,120,10,100,10,0);



	 while(1)
	 {
		lcd_retangulo(red<<11,10,10,50,10,1);										//Retangulos que mostra a variacao de cores quando o botao B1 é apertado
		lcd_retangulo(green<<5,30,10,50,10,1);
		lcd_retangulo(blue,50,10,50,10,1);

		lcd_put_numero(red<<11,0,0,red<<11,1);											//Exemplo de uso de um numero, este pode ser usado de -32767 até 32767
		lcd_put_numero(green<<5,0,(1*CHAR_H),green<<5,1);								//se quiser usar de 0 até 62536, é necessario colocar unsigned na entrada num da funcao definida
		lcd_put_numero(blue,0,(2*CHAR_H),blue,1);
	 }

}



#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void) {
    switch(P1IFG&B1) {
        case B1:
            P1IFG &= ~B1;    // clear the interrupt flag
            red++;
            blue++;
            green++;


            return;
        default:
            P1IFG = 0;    // probably unnecessary, but if another flag occurs
                          // in P1, this will clear it.  No error handling is
                          // provided this way, though.
            return;
    }
} // P1_ISR
