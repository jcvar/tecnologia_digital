/*
Tarea 2: Sensores Analogos con display TFT
Tecnologia Digital
Universidad Nacional de Colombia

Juan Camilo Vargas Q. - jcvargasq@unal.edu.co
Sergio Alejandro Vargas Q. - savargasqu@unal.edu.co

2020-04-13
*/

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

#define cs   10
#define ds   9
#define rst  8

TFT TFTscreen = TFT(cs, ds, rst);

const int led_size = 32; // Size of LED representing circles

// RGB 565: RRRR RGGG GGGB BBBB
const int R = 0xF800;
const int G = 0x07E0;
const int B = 0x001F;
const int Y = 0xFFE0;

// Sensor pins
const int sensorA = A0;
const int sensorB = A1;

// Sketch variables 
int valA;
int valB;
int diff;

// Turn off all LED circles except for one. Pass color as argument
// Apagar todos los LEDs y encender el indicado como argumento
void turnON(int color, int x, int y) {
	fill(0); // background color
	rect(0, 0, led_size*5, led_size); // hide circles
	fill(color);
	ellipse(x, y, led_size, led_size); // draw only one circle
}

void setup() {
	// Put this line at the beginning of every sketch that uses the GLCD:
	TFTscreen.begin();
	
	// clear the screen with a black background
	TFTscreen.background(0, 0, 0);
	
	TFTscreen.stroke(0, 0, 0); // white font color
	// set the font size
	TFTscreen.setTextSize(2);
	// write the text to the top left corner of the screen
	TFTscreen.text("Sensor 1 Value :\n ", 0, 0);
	// set the font size very large for the loop
	TFTscreen.setTextSize(1);
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

