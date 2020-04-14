#include "MAX7219.h"
//SID:Pin 11; SCK:Pin 13

MAX7219 display;

const int CS=10;
int sensorRead;

void setup()
{
	display.begin(CS); 
	display.printInteger(0);
}

void loop()
{
	sensorRead=analogRead(A0);
	display.printInteger(sensorRead);
	delay(100);
}
