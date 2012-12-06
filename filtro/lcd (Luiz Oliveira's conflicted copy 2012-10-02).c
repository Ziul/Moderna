#include "lcd.h"

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

void lcd_wrcmd16(uint16_t dat)
{
  lcd_wrcmd((dat>>8));
  lcd_wrcmd(dat);
}

void lcd_wrdat16(uint16_t dat)
{
  lcd_wrdata(dat>>8);
  lcd_wrdata(dat);
}

/*void lcd_init()   //Tentativa de incializar o L2F50
{
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

	volatile unsigned int i;


	    pinWrite(RESET, 0); // reset display
	    pinWrite(CS, 1);  // CS is high during reset release
	    pinWrite(RS, 1);  // RS is set to high
	    mswait(10);
	    pinWrite(RESET, 1);  // release reset
	    mswait(10);
	    pinWrite(CS, 0);  // select display

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

	    pinWrite(RS, 0);
	    lcd_wrdata(DISON);


	    pinWrite(CS, 1);

}

static void lcd_cspulse() {
    pinWrite(CS, 1);
    pinWrite(CS, 0);
}
*/


void lcd_init()
{
	PORT_DIR |= (RS | DATA | CLK | CS | RESET);

	pinWrite(RESET,0);

	pinWrite(CS,1);

	mswait(100); // wait a bit for everything to settle down... who knows

	pinWrite(CS,1);
	pinWrite(RS,1);
	pinWrite(RESET,0);

	mswait(50);

	pinWrite(RESET,1);
	mswait(50);
	pinWrite(CS,0);

	uint8_t index_init = 0;

	//INIT_0)


	static const uint16_t init_0[] = {0xFDFD,0xFDFD};
	for(index_init = 0 ; index_init < 2 ; index_init++)
		lcd_wrcmd16(init_0[index_init]);
	mswait(50); 													//important mswait of 50ms

	//INIT_1
	static const uint16_t init_1[] = {0xEF00,0xEE04,0x1B04,0xFEFE,0xFEFE,0xEF90,0x4A04,0x7F3F,0xEE04,0x4306};
	for(index_init = 0 ; index_init < 10 ; index_init++)
			lcd_wrcmd16(init_1[index_init]);
	mswait(7);														//important mswait of 7ms

	//INIT_2
	static const uint16_t init_2[] = {0xEF90,0x0983,0x0800,0x0BAF,0xA00,0x0500,0x0600,0x0700,0xEF00,0xEE0C,0xEF90,0x0080,
	0xEFB0,0x4902,0xEF00,0x7F01,0xE181,0xE202,0xE276,0xE183};
	for(index_init = 0 ; index_init < 20 ; index_init++)
			lcd_wrcmd16(init_2[index_init]);
	mswait(50);														//important mswait of 50ms

	//INIT_3
	static const uint16_t init_3[] = {0x8001,0xEF90,0x0000};
	for(index_init = 0 ; index_init < 3 ; index_init++)
			lcd_wrcmd16(init_3[index_init]);


	pinWrite(CS,1);													// were done, display doesnt need to listen anymore
}

void lcd_wrdata(uint8_t data)
 {
	 PORT_OUT &=~CS;		//enable glcd
	 PORT_OUT &= ~RS;

	 //cmd(data);
	 writeSPI(data);
	 //spi_send_data(data);

	 PORT_OUT |= CS;		//desable glcd
 }
 void lcd_wrcmd(uint8_t data)
 {
	 PORT_OUT &=~CS;		//enable glcd
	 PORT_OUT |= RS;
	//cmd(data);
	 writeSPI(data);
	 //spi_send_data(data);

	 PORT_OUT |= CS;		//desable glcd
 }

 void writeSPI(unsigned int data) {
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

