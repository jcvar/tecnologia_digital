/*
Tarea: Pines Analogos
Tecnologia Digital
Universidad Nacional de Colombia

Juan Camilo Vargas Q. - jcvargasq@unal.edu.co
Sergio Alejandro Vargas Q. - savargasqu@unal.edu.co

2020-04-01
*/
#include "MAX7219.h"

MAX7219 displayA;
MAX7219 displayB;

// LED pins
const int R = 4;
const int G = 5;
const int B = 6;
const int Y = 7;

// Display pins
const int dA = 8;
const int dB = 9;

// Sensor pins
const int sensorA = A0;
const int sensorB = A1;

// Sketch variables 
int valA;
int valB;
int diff;

// Apagar todos los LEDs y encender el indicado como argumento
void turnON(int ledON){
	digitalWrite(R, LOW);
	digitalWrite(G, LOW);
	digitalWrite(B, LOW);
	digitalWrite(Y, LOW);
	
	digitalWrite(ledON, HIGH);
}

void setup() {
	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(Y, OUTPUT);
	
	displayA.begin(dA);
	displayB.begin(dB);
}

void loop() {
	valA = map(analogRead(sensorA), 0, 1023, 0, 300);
	valB = map(analogRead(sensorB), 0, 1023, 0, 200);
	
	displayA.printInteger(valA);
	displayB.printInteger(valB);
	
	if (valA >= 250){
		turnON(R);
	}else if (valA < 180){
		diff = valA - valB;
		if (diff > 30 && diff < 70){
			turnON(Y);
		}else if (diff >= 70){
			turnON(B);
		}else{
			turnON(G);
		}
	}else{
		turnON(G);
	}
}
