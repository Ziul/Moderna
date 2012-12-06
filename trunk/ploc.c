/************************************************************************************
*										ploc.c										*
*************************************************************************************/
#define COR 	0
#define BACK	0xffff
#include "disp.h"

int ploc(unsigned int value)
{
	static int x=0;
	static int x0,x1,x2,x3,antiga;
	
	x3=x2;
	x2=x1;
	x1=x0;
	x0=value/10; 					//to not overflow the display space
	
#ifdef __GRAFICO_BARRAS__
		lcd_retangulo(BACK,x,0,0,DISP_W);		// clean llast value here
//		lcd_retangulo(COR,x++,0,0,(x0+x1+x2+x3)/4);	// put new value and inc
		lcd_retangulo(COR,x++,0,0,x0);	// put the value (without filter)
#else
		lcd_retangulo(BACK,x,0,0,DISP_W);		// clean llast value here
//		lcd_set_pixel(COR,x++,(x0+x1+x2+x3)/4);	// with filter
		lcd_set_pixel(COR,x++,x0);				// without filter
#endif

	antiga = (x0+x1+x2+x3)/4;
	if(x==DISP_H)
	//	end of the display
	{
		x=0;
	}
	
	return 0;
}

