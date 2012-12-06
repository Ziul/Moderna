/*
 * disp.h
 *
 *  Created on: 07/02/2012
 *      Author: Limpinho
 */

#ifndef DISP_H_
#define DISP_H_


#include "lcd.h"
#include <math.h>
#include <msp430g2553.h>

#define DISP_W 132
#define DISP_H 176
#define CHAR_H 14
#define CHAR_W 8
#define TEXT_COL 16
#define TEXT_ROW 12


extern uint16_t textcolor;  // color of border around text
extern uint16_t backcolor;    // color of background

/////////////////////////////// function prototypes ////////////////////////


void lcd_clear(uint16_t color);
//void put_char(uint8_t x, uint8_t y, char c, uint8_t rot);
void lcd_set_pixel(uint16_t color,uint8_t x,uint8_t y);
void lcd_retangulo(uint16_t color,uint8_t x,uint8_t dx,uint8_t y,uint8_t dy);
void lcd_circulo(uint16_t color,uint8_t xc,uint8_t yc,uint8_t raio);

//#define  PHILLIPS
#define  EPSON

//********************************************************************
//
//          LCD Dimension Definitions
//
//********************************************************************
#define ROW_LENGTH  132
#define COL_HEIGHT  132
#define ENDPAGE     132
#define ENDCOL      130
#define DISP_W 		132
#define DISP_H		176
#define CHAR_H		14
#define CHAR_W		8
#define TEXT_COL	16
#define TEXT_ROW	12


//*******************************************************
//        12-Bit Color Definitions
//*******************************************************
#define WHITE  0xFFFF
#define BLACK  0x0000
#define RED    0xF00
#define GREEN  0x0F0
#define BLUE  0x00F
#define CYAN  0x0FF
#define MAGENTA  0xF0F
#define YELLOW  0xFF0
#define BROWN  0xB22
#define ORANGE  0xFA0
#define PINK  0xF6A




#endif /* DISP_H_ */
