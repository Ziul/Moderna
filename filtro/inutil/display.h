#include <msp430g2553.h>
#include <math.h>
#ifndef DISPLAY_H_
#define DISPLAY_H_

#define abs(x)	fabs(x)

enum DISPLAY
{
	DIS_A=BIT0,
	DIS_B=BIT1,
	DIS_C=BIT2,
	DIS_D=BIT4,
	DIS_E=BIT5,
	DIS_F=BIT6,
	DIS_G=BIT7,
	DIS_PONTO
	
};

extern
void init_display();

extern
void stop_display();

extern
void set_display_digit(char numero,int posicao);

extern
void set_display_number(int numb);

extern
char itoc(int i);

void func_numero(char digit);

void atualizar_display();

#endif
