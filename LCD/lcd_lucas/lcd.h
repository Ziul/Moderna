/*
 * lcd.h
 *
 *  Created on: 07/02/2012
 *      Author: Limpinho
 */

#ifndef LCD_H_
#define LCD_H_

#include <inttypes.h>
#include <msp430g2553.h>

#define PORT_DIR P2DIR
#define PORT_OUT P2OUT

#define RS	BIT0
#define RESET	BIT1
#define CS	BIT2
#define CLK   	BIT3
#define DATA    BIT4

# define S65_WIDTH            (176)
# define S65_HEIGHT           (132)

	//Descomente para usar o L2F50

#define DATCTL 0xBC         // Data Control (data handling in RAM)
#define DISCTL 0xCA         // Display Control
#define GCP64 0xCB          // pulse set for 64 gray scale
#define GCP16 0xCC          // pulse set for 16 gray scale
#define OSSEL 0xD0          // Oscillator select
#define GSSET 0xCD          // set for gray scales
#define ASCSET 0xAA         // aerea scroll setting
#define SCSTART 0xAB        // scroll start setting
#define DISON 0xAF          // Display ON (no parameter)
#define DISOFF 0xAE         // Display OFF (no parameter)
#define DISINV 0xA7         // Display Invert (no parameter)
#define DISNOR 0xA6         // Display Normal (no parameter)
#define SLPIN  0x95         // Display Sleep (no parameter)
#define SLPOUT 0x94         // Display out of sleep (no parameter)
#define RAMWR 0x5C          // Display Memory write
#define PTLIN 0xA8          // partial screen write

#define SD_CSET 0x15        // column address setting
#define SD_PSET 0x75        // page address setting



 void mswait(uint16_t ms);

 void lcd_wrdat16(uint16_t);
 void lcd_cspulse();
 void lcd_write(uint8_t data[2]);

 void lcd_init();			// LCD initialise
 void lcd_cspulse();

 void lcd_wrdata(uint8_t data);
 void lcd_wrcmd(uint8_t data);

 void writeSPI(uint8_t data);
 void pinWrite(unsigned int bit, unsigned char val);


#endif /* LCD_H_ */
