#include "MAX7219.h"
//SID:Pin 11; SCK:Pin 13

MAX7219 display;

const int CS=10;
int sensorRead;
float temp;

void setup()
{
	display.begin(CS); 
	display.printFloat(0,1);
}

void loop()
{
	sensorRead=analogRead(A0);
	temp=((float)sensorRead*100)/1023;
	display.printFloat(temp,1);
	delay(100);
}
