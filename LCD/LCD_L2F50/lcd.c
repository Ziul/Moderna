#include "lcd.h"
#include "disp.h"
/*
 * lcd.c
 *
 *  Created on: 07/02/2012
 *      Author: Limpinho
 */


void mswait(uint16_t ms)
{
	BCSCTL1= CALBC1_16MHZ;		//Set up the processor to run at a calibrated 16MHz rate.
	DCOCTL= CALDCO_16MHZ;

	//d*=512;
	uint16_t time = ms;

    while (time--)
    {
        __delay_cycles(16000);//30
    }
}


void lcd_write(uint8_t data[2]) {
   writeSPI(data[0]);
   writeSPI(data[1]);
}

void lcd_wrcmd(uint8_t dat) {
	unsigned char ddd[2];

	    ddd[0] = dat;
	    ddd[1] = 0;
	    pinWrite(RS,0);
	    //lcd_set_pin(LCD_RS, 0);
	    lcd_write(ddd);
	    pinWrite(RS,1);
	    //lcd_set_pin(LCD_RS, 1);
}

void lcd_wrdata(uint8_t dat) {
	uint8_t ddd[2];

	    ddd[0] = dat;
	    ddd[1] = 0;
	    lcd_write(ddd);
}


void lcd_wrdat16(uint16_t dat) {
	uint8_t ddd[2];

	  ddd[0] = (dat>>8);
	  ddd[1] = dat;
	  lcd_write(ddd);
}

void lcd_cspulse() {
    pinWrite(CS,1);
    __delay_cycles(1);
	//lcd_set_pin(LCD_CS, 1);
    pinWrite(CS,0);
    //lcd_set_pin(LCD_CS, 0);
}



void lcd_init()
{

	PORT_DIR |= (RS | DATA | CLK | CS | RESET);
	volatile unsigned int i;

	static unsigned char disctl[9] = {0x4C, 0x01, 0x53, 0x00, 0x02, 0xB4, 0xB0, 0x02, 0x00};
	static unsigned char gcp64_0[29] =
	                    {0x11,0x27,0x3C,0x4C,0x5D,0x6C,0x78,0x84,0x90,0x99,0xA2,0xAA,0xB2,0xBA,
	                     0xC0,0xC7,0xCC,0xD2,0xD7,0xDC,0xE0,0xE4,0xE8,0xED,0xF0,0xF4,0xF7,0xFB,
	                     0xFE};
	static unsigned char gcp64_1[34] =
	                   {0x01,0x03,0x06,0x09,0x0B,0x0E,0x10,0x13,0x15,0x17,0x19,0x1C,0x1E,0x20,
	                    0x22,0x24,0x26,0x28,0x2A,0x2C,0x2D,0x2F,0x31,0x33,0x35,0x37,0x39,0x3B,
	                    0x3D,0x3F,0x42,0x44,0x47,0x5E};
	static unsigned char gcp16[15] =
	                    {0x13,0x23,0x2D,0x33,0x38,0x3C,0x40,0x43,0x46,0x48,0x4A,0x4C,0x4E,0x50,0x64};

	    pinWrite(RESET,0);
		//lcd_set_pin(LCD_RESET, 0); // reset display
	    pinWrite(CS,1);
	    //lcd_set_pin(LCD_CS, 1);  // CS is high during reset release
	    pinWrite(RS,1);
	    //lcd_set_pin(LCD_RS, 1);  // RS is set to high
	    mswait(10);
	    //OSTimeDly(10);
	    pinWrite(RESET,1);
	    //lcd_set_pin(LCD_RESET, 1);  // release reset
	    mswait(10);
	    //OSTimeDly(10);
	    pinWrite(CS,0);
	    //lcd_set_pin(LCD_CS, 0);  // select display

	    lcd_wrcmd(DATCTL);
	    lcd_wrdata(0x2A);  // 0x2A=565 mode, 0x0A=666mode, 0x3A=444mode

	    lcd_cspulse();

	    lcd_wrcmd(DISCTL);
	    for (i=0; i<9; i++) {
	       lcd_wrdata(disctl[i]);
	    }

	    lcd_wrcmd(GCP64);
	    for (i=0; i<29; i++) {
	       lcd_wrdata(gcp64_0[i]);
	       lcd_wrdata(0x00);
	    }
	    for (i=0; i<34; i++) {
	       lcd_wrdata(gcp64_1[i]);
	       lcd_wrdata(0x01);
	    }

	    lcd_wrcmd(GCP16);
	    for (i=0; i<15; i++) {
	       lcd_wrdata(gcp16[i]);
	    }

	    lcd_wrcmd(GSSET);
	    lcd_wrdata(0x00);

	    lcd_wrcmd(OSSEL);
	    lcd_wrdata(0x00);

	    lcd_wrcmd(SLPOUT);

	    //  _delay_ms(7);

	    lcd_wrcmd(SD_CSET);
	    lcd_wrdata(0x08);
	    lcd_wrdata(0x01);
	    lcd_wrdata(0x8B);
	    lcd_wrdata(0x01);

	    lcd_wrcmd(SD_PSET);
	    lcd_wrdata(0x00);
	    lcd_wrdata(0x8F);


	    lcd_wrcmd(ASCSET);
	    lcd_wrdata(0x00);
	    lcd_wrdata(0xAF);
	    lcd_wrdata(0xAF);
	    lcd_wrdata(0x03);

	    lcd_wrcmd(SCSTART);
	    lcd_wrdata(0x00);

	    pinWrite(RS,0);
	    //lcd_set_pin(LCD_RS, 0);
	    lcd_wrdata(DISON);


	    pinWrite(CS,1);
	    //lcd_set_pin(LCD_CS, 1);
}



 void writeSPI(uint8_t data) {
    unsigned int  mask;

    for(mask=0x80; mask != 0; mask = mask >> 1)
    {
       //digitalWrite(CLK, LOW);
       pinWrite(CLK,0);
       if(mask & data)
       {
          //digitalWrite(DATA, HIGH);
          pinWrite(DATA,1);
       }
       else
       {
          //digitalWrite(DATA, LOW);
          pinWrite(DATA,0);
       }
       //digitalWrite(CLK, HIGH);
       pinWrite(CLK,1);
    }
    //digitalWrite(CLK, LOW);
    pinWrite(CLK,0);
 }

 void pinWrite(unsigned int bit, unsigned char val)
 {
   if (val){
     PORT_OUT |= bit;
   } else {
     PORT_OUT &= ~bit;
   }
 }

