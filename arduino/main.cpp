/**
 * Test model for Arduino - UNO
 */


#include <Arduino.h>
//#include <SD/SD.h>
#include <SPI.h> //inclusão da biblioteca SPI
#include <Ethernet/Ethernet.h> //inclusão da biblioteca Ethernet
#include <stdio.h>
#include <stdlib.h>

/// The setup() method runs once, when the sketch starts
void setup()
{
	// initialize the default debug output:
	Serial.begin(9600);
	//Serial.print("#Initializing serial support...if you hear me, debug is ready!\n");
	// Seeding the rand
	srand(9600);

}

/// the loop() method runs over and over again,
/// as long as the Arduino has power
void loop()
{
static int i=0;
char buf[20];

	//sprintf(buf,"%d %d\n",i++,5*analogRead(0)/1023 );	//! Valores convertidos
	sprintf(buf,"%d %d\n",i++,analogRead(0) );			//! valores brutos
	Serial.print(buf);
	//delay(1000);                  // wait for a second
}

/**
 * This is the standard main function. On Arduino, it's NEVER changes.
 * @return the standard returns that the main should return.
 */
int main(void)
{

	init();	//! Initialize the pins I/O and standard configures.
	setup();	//! Call the user's setup function.

	//! The eternal loop
	while(true) {
	loop();
	}

	return 0;
}
