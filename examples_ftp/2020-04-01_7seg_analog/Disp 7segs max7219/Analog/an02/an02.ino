#include "MAX7219.h"
//SID:Pin 11; SCK:Pin 13

MAX7219 display;

const int CS=10;
int sensorRead,val;

void setup()
{
	display.begin(CS); 
	display.printInteger(0);
}

void loop()
{
	sensorRead=analogRead(A0);
	val = map(sensorRead, 0, 1023, 5, 10);
	display.printInteger(val);
	delay(100);
}
