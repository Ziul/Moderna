#include <msp430g2553.h>
#include "display.h"
#include "base.h"


char vector_display[] = {'0','0','0','0'};												// vetor que guarda os digitos do display
int index_display = 0;																	// numero que representa o endereco de cada display

void init_display()																		// Inicia o display
{
	TACCR0 = 500;																		// Seta o tempo do timer
	TACCTL0 = CCIE;        																// Habilita o timer
	TACTL = TASSEL_2 + ID_3 + MC_1 + TACLR;  											// SMCLK, div 8, up mode, clear timer
	
	_enable_interrupt();  																// Note the singular name here!  It's not interrupts.
}

void stop_display()																		// Para o display
{
	TACCTL0 = !CCIE;        // Enable interrupts for CCR0.								// Interrompe o Timer
	P1OUT |= (DIS_A | DIS_B | DIS_C | DIS_D | DIS_E | DIS_F | DIS_G | DIS_PONTO);		// Desliga todos os displays
	
}

char itoc(int i) {																		// Converte um numero inteiro para um caracter que o representa
     switch (i) {
            case 0: return '0';
            case 1: return '1';
            case 2: return '2';
            case 3: return '3';
            case 4: return '4';
            case 5: return '5';
            case 6: return '6';
            case 7: return '7';
            case 8: return '8';
            case 9: return '9';
            case -1: return 'g';
     }
     return 0;
}

int pow_int(int pow_base , int pow_expoente)											// Funcao de potencia para inteiros
{
	int index_pow;
	int pow_acumulador = 1;
	for(index_pow = 0 ; index_pow < pow_expoente ; index_pow++)
	{
		pow_acumulador *= pow_base;
	}
	return pow_acumulador;
}


void func_numero(char digit)															// Funcao que transforma os digitos em seus equivalentes de 7 segmentos
{
	switch(digit)
	{
	case '0':
		P1OUT &= ~(DIS_A | DIS_B | DIS_C | DIS_D | DIS_E | DIS_F);
		P1OUT |= DIS_G;
		return;
	case '1':
		P1OUT &= ~(DIS_B | DIS_C);
		P1OUT |= (DIS_A | DIS_D | DIS_E | DIS_F | DIS_G);
		return;
	case '2':
		P1OUT &= ~(DIS_A | DIS_B | DIS_D | DIS_E | DIS_G);
		P1OUT |= (DIS_C | DIS_F);
		return;
	case '3':
		P1OUT &= ~(DIS_A | DIS_B | DIS_C | DIS_D | DIS_G);
		P1OUT |= (DIS_E | DIS_F);
		return;
	case '4':
		P1OUT &= ~(DIS_B | DIS_C | DIS_F | DIS_G);
		P1OUT |= (DIS_A | DIS_D | DIS_E);
		return;
	case '5':
		P1OUT &= ~(DIS_A | DIS_C | DIS_D | DIS_F | DIS_G);
		P1OUT |= (DIS_B | DIS_E);
		return;
	case '6':
		P1OUT &= ~(DIS_A | DIS_C | DIS_D | DIS_E | DIS_F | DIS_G);
		P1OUT |= (DIS_B);
		return;
	case '7':
		P1OUT &= ~(DIS_A | DIS_B | DIS_C);
		P1OUT |= (DIS_D | DIS_E | DIS_F | DIS_G);
		return;
	case '8':
		P1OUT &= ~(DIS_A | DIS_B | DIS_C | DIS_D | DIS_E | DIS_F | DIS_G);
		return;
	case '9':
		P1OUT &= ~(DIS_A | DIS_B | DIS_C | DIS_D | DIS_F | DIS_G);
		P1OUT |= (DIS_E);
		return;
	case 'A':
		P1OUT &= ~(DIS_A | DIS_B | DIS_C | DIS_E | DIS_F | DIS_G);
		P1OUT |= (DIS_D);
		return;
	case 'B':
		P1OUT &= ~(DIS_C | DIS_D | DIS_E | DIS_F | DIS_G);
		P1OUT |= (DIS_A | DIS_B);
		return;
	case 'C':
		P1OUT &= ~(DIS_A | DIS_D | DIS_E | DIS_F);
		P1OUT |= (DIS_B | DIS_C | DIS_G);
		return;
	case 'D':
		P1OUT &= ~(DIS_B | DIS_C | DIS_D | DIS_E | DIS_G);
		P1OUT |= (DIS_A | DIS_F);
		return;
	case 'E':
		P1OUT &= ~(DIS_A | DIS_D | DIS_E | DIS_F | DIS_G);
		P1OUT |= (DIS_B | DIS_C);
		return;
	case 'F':
		P1OUT &= ~(DIS_A | DIS_E | DIS_F | DIS_G);
		P1OUT |= (DIS_B | DIS_C | DIS_D);
		return;
	case 'G':
		P1OUT &= ~(DIS_A | DIS_B | DIS_C | DIS_D | DIS_F | DIS_G);
		P1OUT |= (DIS_E);
		return;
	case 'H':
		P1OUT &= ~(DIS_B | DIS_C | DIS_E | DIS_F | DIS_G);
		P1OUT |= (DIS_A | DIS_D);
		return;
	case 'I':
		P1OUT &= ~(DIS_E | DIS_F);
		P1OUT |= (DIS_A | DIS_B | DIS_C | DIS_D | DIS_G);
		return;
	case 'J':
		P1OUT &= ~(DIS_B | DIS_C | DIS_D | DIS_E);
		P1OUT |= (DIS_A | DIS_F | DIS_G);
		return;
	case 'L':
		P1OUT &= ~(DIS_D | DIS_E | DIS_F);
		P1OUT |= (DIS_A | DIS_B | DIS_C | DIS_G);
		return;
	case 'O':
		P1OUT &= ~(DIS_C | DIS_D | DIS_E | DIS_G);
		P1OUT |= (DIS_A | DIS_B | DIS_F);
		return;
	case 'P':
		P1OUT &= ~(DIS_A | DIS_B | DIS_E | DIS_F | DIS_G);
		P1OUT |= (DIS_C | DIS_D);
		return;
	case 'Q':
		P1OUT &= ~(DIS_A | DIS_B | DIS_C | DIS_F | DIS_G);
		P1OUT |= (DIS_D | DIS_E);
		return;
	case 'U':
		P1OUT &= ~(DIS_B | DIS_C | DIS_D | DIS_E | DIS_F);
		P1OUT |= (DIS_A | DIS_G);
		return;
	case 'a':
		P1OUT &= ~(DIS_A);
		P1OUT |= (DIS_B | DIS_C | DIS_D | DIS_E | DIS_F | DIS_G);
		return;
	case 'b':
		P1OUT &= ~(DIS_B);
		P1OUT |= (DIS_A | DIS_C | DIS_D | DIS_E | DIS_F | DIS_G);
		return;
	case 'c':
		P1OUT &= ~(DIS_C);
		P1OUT |= (DIS_A | DIS_B | DIS_D | DIS_E | DIS_F | DIS_G);
		return;
	case 'd':
		P1OUT &= ~(DIS_D);
		P1OUT |= (DIS_A | DIS_B | DIS_C | DIS_E | DIS_F | DIS_G);
		return;
	case 'e':
		P1OUT &= ~(DIS_E);
		P1OUT |= (DIS_A | DIS_B | DIS_C | DIS_D | DIS_F | DIS_G);
		return;
	case 'f':
		P1OUT &= ~(DIS_F);
		P1OUT |= (DIS_A | DIS_B | DIS_C | DIS_D | DIS_E | DIS_G);
		return;
	case 'g':
		P1OUT &= ~(DIS_G);
		P1OUT |= (DIS_A | DIS_B | DIS_C | DIS_D | DIS_E | DIS_F);
		return;
	}
}


void set_display_digit(char numero,int posicao)											// Funcao externa que possibilta alterar qualquer digito de qualquer display
{
	vector_display[posicao] = numero;
}

void set_display_number(int numb)														// Funcao externa que possibilita converter um numero inteiro e escrevelo nos displays
{
	int num = numb;
	int display_number_vector[4] = {0,0,0,0};
	int index_display_number = 0;

		if(!(numb >= -999 && numb <= 9999))												//confere se o numero de entrada esta dentro do possivel e se nao estiver retorna uma msg de erro
				{
					set_display_digit('F',3);
					set_display_digit('U',2);
					set_display_digit('U',1);
					set_display_digit('U',0);
				}
		else
			{

			num = abs(numb);

			for(index_display_number = 3 ; index_display_number > 0 ; index_display_number--)	//transforma cada parte do numero para cada um dos displays
			{
				if(num >= pow_int(10,index_display_number) )
				{
					display_number_vector[index_display_number] = num/pow_int(10,index_display_number);
					num = num - display_number_vector[index_display_number]*pow_int(10,index_display_number);
				}
			}
			display_number_vector[0] = num;


			if(numb < 0)
				display_number_vector[3] = -1;

			for(index_display_number = 0;index_display_number < 4 ; index_display_number++)
				set_display_digit(itoc(display_number_vector[index_display_number]),index_display_number);


		}

}

void atualizar_display()					// Atualiza o display a cada periodo do Timer
{
switch(index_display++){
	case 0:
		P2OUT &= ~(DIS_A | DIS_B);
		func_numero(vector_display[0]);
		return;
	case 1:
		P2OUT &= ~(DIS_B);
		P2OUT |= (DIS_A);
		func_numero(vector_display[1]);
		return;
	case 2:
		P2OUT &= ~(DIS_A);
		P2OUT |= (DIS_B);
		func_numero(vector_display[2]);
		return;
	case 3:
		P2OUT |= (DIS_A | DIS_B);
		func_numero(vector_display[3]);
		return;
	default:
		index_display = 0;
		return;
	}
}

interrupt(TIMER0_A0_VECTOR) CCR0_ISR(void) 	// Interrupcao do Timer
{
	atualizar_display();
}
