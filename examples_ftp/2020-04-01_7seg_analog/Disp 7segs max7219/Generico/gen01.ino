#include "MAX7219.h"

MAX7219 display;

long k=340;
float q=72.067;

void setup()
{
	display.begin(10);
}

void loop()
{
	display.printInteger(k);
	delay(1000);
	display.printFloat(q,2);
	delay(1000);
}
