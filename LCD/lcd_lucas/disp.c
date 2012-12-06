/*
 * disp.c
 *
 *  Created on: 07/02/2012
 *      Author: Limpinho
 */
#include "disp.h"



uint16_t backcolor=WHITE;    // color of background
//uint16_t textcolor=WHITE;


void lcd_clear(uint16_t color)					//Funcao usada para mudar toda a tela pra um cor determinada, é um pouco lenta entao é bom usar so no inicio do programa
{
    uint16_t i;

    pinWrite(CS,0);
    //PORTB &= ~_BV(LCD_CS);  // select display

    lcd_wrcmd(SD_CSET);
    lcd_wrdata(0x08);  // start is 8, not 0
    lcd_wrdata(0x01);
    lcd_wrdata(0x8B);  // is 00x8B = 0x83+8
    lcd_wrdata(0x01);

    lcd_wrcmd(SD_PSET);
    lcd_wrdata(0x00);
    lcd_wrdata(0xAF);

    lcd_wrcmd(RAMWR);

    // start data transmission
    for (i=0; i<DISP_W*DISP_H; i++)
      lcd_wrdat16(color);

    pinWrite(CS,1);
    //PORTB |= _BV(LCD_CS);  // deselect display
}



void lcd_set_pixel(uint16_t color,uint8_t x,uint8_t y,uint8_t rot)				//Funcao mais importante para setar um pixel, o rot é de rotacao, para escolher se a tela esta sendo
{																				//vista deitada ou em pé

	uint8_t var;

	if(!rot)
		{
			var = x;
			x = y;
			y = var;
		}

		if((x >= DISP_W) || (y >= DISP_H))
		{
		    return;
		}

	pinWrite(CS,0);
	  //lcd_set_pin(LCD_CS, 0);  // select display

	  lcd_wrcmd(SD_CSET);
	  lcd_wrdata(0x08+x);  // start is 8, not 0
	  lcd_wrdata(0x01);
	  lcd_wrdata(0x08+x);  // end is 00x8B = 0x83+8
	  lcd_wrdata(0x01);

	  lcd_wrcmd(SD_PSET);
	  lcd_wrdata(y);
	  lcd_wrdata(y);

	  lcd_wrcmd(RAMWR);

	  lcd_wrdat16(color);

	  //lcd_wrdat16(pix);
	  //pix = ((pix < 8)|(pix >> 8));
	  //lcd_write((unsigned char *)&color);

	  pinWrite(CS,1);
	  //lcd_set_pin(LCD_CS, 1);  // deselect display
}

void lcd_retangulo(uint16_t color,uint8_t x,int16_t dx,uint8_t y,int16_t dy,uint8_t rot)			//Funcao para desenhar um retangulo preenchido
{
	uint8_t index_x,index_y;
	if(dx >= 0 && dy >= 0)
	for(index_x = x;index_x <= x+dx ; index_x++)
		for(index_y = y; index_y <= y+dy; index_y++)
			lcd_set_pixel(color,index_x,index_y,rot);

	else if(dx <= 0 && dy >= 0)
	for(index_x = x;index_x >= x+dx ; index_x--)
		for(index_y = y; index_y <= y+dy; index_y++)
			lcd_set_pixel(color,index_x,index_y,rot);

	else if(dx >= 0 && dy <= 0)
	for(index_x = x;index_x <= x+dx ; index_x++)
		for(index_y = y; index_y >= y+dy; index_y--)
			lcd_set_pixel(color,index_x,index_y,rot);

	else
	for(index_x = x;index_x >= x+dx ; index_x--)
		for(index_y = y; index_y >= y+dy; index_y--)
			lcd_set_pixel(color,index_x,index_y,rot);
}


void lcd_circulo(uint16_t color,uint8_t xc,uint8_t yc,uint8_t raio,uint8_t rot)						//Funcao para desenhar um circulo
{
	int index_x = xc,index_y = yc;

	//lcd_set_pixel(color,xc,yc,rot);
/*
		for(index_x = - raio; index_x <= raio ; index_x++)
		{
			index_y = arredondamento(sqrt((raio*raio) - (index_x*index_x)));
			lcd_set_pixel(color,xc + index_x,yc + index_y,rot);
			lcd_set_pixel(color,xc + index_x,yc - index_y,rot);
			lcd_set_pixel(color,xc + index_y,yc + index_x,rot);
			lcd_set_pixel(color,xc - index_y,yc + index_x,rot);
		}
		
*/
}

int arredondamento(double num)										//Funcao usada no circulo para deixar ele mais suave
{
	int num_int = num;
	if( (num - num_int) >= 0.5) return (num_int +1);
	else return num_int;
}

void lcd_linha(uint16_t color, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t rot)
{
	int16_t dx, dy, dx2, dy2, err, stepx, stepy;
	uint8_t var;
	if(!rot)
	{
		var = x0;
		x0 = y0;
		y0 = var;

		var = x1;
		x1 = y1;
		y1 = var;
	}

	  if(x0 == x1)
		 lcd_retangulo(color,x0,0,y0,y1-y0,rot);
	  else if(y0 == y1)
	    lcd_retangulo(color,x0,x1-x0,y0,0,rot);
	  else
	  {
	    //calculate direction
	    dx = x1 - x0;
	    dy = y1 - y0;
	    if(dx < 0) { dx = -dx; stepx = -1; } else { stepx = +1; }
	    if(dy < 0) { dy = -dy; stepy = -1; } else { stepy = +1; }
	    dx2 = dx << 1;
	    dy2 = dy << 1;
	    //draw line
	    lcd_set_pixel(color,x0, y0,rot);
	    if(dx > dy)
	    {
	      err = dy2 - dx;
	      while(x0 != x1)
	      {
	        if(err >= 0)
	        {
	          y0  += stepy;
	          err -= dx2;
	        }
	        x0  += stepx;
	        err += dy2;
	        lcd_set_pixel(color,x0, y0, rot);
	      }
	    }
	    else
	    {
	      err = dx2 - dy;
	      while(y0 != y1)
	      {
	        if(err >= 0)
	        {
	          x0  += stepx;
	          err -= dy2;
	        }
	        y0  += stepy;
	        err += dx2;
	        lcd_set_pixel(color, x0, y0, rot);
	      }
	    }
	  }



}

void lcd_put_numero(uint16_t color,uint8_t x, uint8_t y, int numb, uint8_t rot)    			//Funcao para colocar um numero em qualquer posicao da tela
{																						//Se quiser um numero somente positivo mude para unsigned para aumentar o range
	int num = numb;
	int display_number_vector[5] = {' ' - 48,' ' - 48,' ' - 48,' ' - 48,' ' - 48};
	int index_display_number = 0;
	uint8_t posic = 0;
	uint8_t posic2 = 0;
	uint8_t flag_num = 0;

			num = abs(numb);
/*
			for(index_display_number = 4 ; index_display_number > 0 ; index_display_number--)
			{
					if(num >= pow(10,index_display_number) || flag_num)
					{
						flag_num = 1;
						display_number_vector[index_display_number] = num/pow(10,index_display_number);
						num = num - display_number_vector[index_display_number]*pow(10,index_display_number);
						posic++;
					}
			}
			display_number_vector[0] = num;

			if(numb < 0)
				{
					lcd_put_char(color,x,y,'-',rot);
					posic2++;
				}
				

			for(index_display_number = 0; index_display_number <= posic ; index_display_number++)
					lcd_put_char(color,x+(CHAR_W*(index_display_number + posic2)),y,display_number_vector[posic - index_display_number]+48,rot);
			for(index_display_number = 5 ; index_display_number > (posic + posic2) ; index_display_number--)
				lcd_put_char(color,x+(CHAR_W*(index_display_number)),y,' ',rot);
*/
	
}

void lcd_put_char(uint16_t color,uint8_t x, uint8_t y, char c, uint8_t rot)						//devido a pouca memoria do msp so foi possivel adicionar alguns caracteres
{																							//foram esses o: '*' , '+' , '´' , '-' , '.' '/' , e os numeros de 0 a 9;
																							//Se muito necessario podem ser adicionadas algumas letras mas é bom ter cuidado
																							//com o tamanho do codig
																							//Ainda nao foi possivel implementar a funcao de rotacao nessa funcao do L2F50
	// ascii table, starting with character blank (32)
// size is 8x14
unsigned char ascii_tab[16][14]={
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00, 0x00, 0x00}, // *
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00}, // +
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00}, // ´
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // -
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00}, // .
{ 0x00, 0x00, 0x00, 0x02, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00}, // /
{ 0x00, 0x00, 0x00, 0x38, 0x6c, 0xc6, 0xc6, 0xd6, 0xc6, 0xc6, 0x6c, 0x38, 0x00, 0x00}, // 0 (48-32)
{ 0x00, 0x00, 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc6, 0xfe, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0x06, 0x3c, 0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x0c, 0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x0c, 0x1e, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xfe, 0xc0, 0xc0, 0xc0, 0xfc, 0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x38, 0x60, 0xc0, 0xc0, 0xfc, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0xfe, 0xc6, 0x06, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00},
{ 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0x06, 0x0c, 0x78, 0x00, 0x00},
};

uint8_t h,ch,p,mask,he;

  pinWrite(CS,0);
  //PORTB &= ~_BV(LCD_CS);  // select display

  lcd_wrcmd(SD_CSET);
  lcd_wrdata(0x08+x);  // start is 8, not 0
  lcd_wrdata(0x01);
  lcd_wrdata(0x08+x+CHAR_W-1);  // end is 00x8B = 0x83+8
  lcd_wrdata(0x01);

  lcd_wrcmd(SD_PSET);
  lcd_wrdata(y);
  if (y < DISP_H-CHAR_H)
  {
    he=CHAR_H;
    lcd_wrdata(y+CHAR_H-1);
  }
  else
  {
    he=DISP_H-y;
    lcd_wrdata(DISP_H-1);
  }

  lcd_wrcmd(RAMWR);

  for (h=0; h<he; h++) // every column of the character
  {
        ch=ascii_tab[ c-32-10 ][h];

        mask=0x80;
        for (p=0; p<CHAR_W; p++)  // write the pixels
        {
          if (ch&mask && c != ' ')
          {
            lcd_wrdat16(color);
          }
          else
          {
            lcd_wrdat16(backcolor);
          }
          mask=mask/2;
        }  // for p
  }
  pinWrite(CS,1);
  //PORTB |= _BV(LCD_CS);  // deselect display
}
