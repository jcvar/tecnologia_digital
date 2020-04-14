#include "MAX7219.h"
//SID:Pin 11; SCK:Pin 13

MAX7219 display;

const int CS=10;
const int button1Pin=4;
const int button2Pin=5;
int count;

enum ButtonStateType {
	lowValue,
	highValue
}; 

ButtonStateType button1State=lowValue;
ButtonStateType button2State=lowValue;

void setup()
{
	display.begin(CS); 
	count=0;
	display.printInteger(count);
}

void loop()
{
	// SWITCH BUTTON 1
	switch(button1State) {
		case lowValue:
			if(digitalRead(button1Pin)==HIGH){
				count++;
				button1State=highValue;
			}
			break;
		case highValue:
			if(digitalRead(button1Pin)==LOW){
				button1State=lowValue;
			}
			break;
	}
	
	// SWITCH BUTTON 2
	switch(button2State) {
		case lowValue:
			if(digitalRead(button2Pin)==HIGH){
				if(count>0) count--;
				button2State=highValue;
			}
			break;
		case highValue:
			if(digitalRead(button2Pin)==LOW){
				button2State=lowValue;
			}
			break;
	}
	display.printInteger(count);
	delay(100);
}
