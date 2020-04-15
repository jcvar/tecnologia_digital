/*
    Tarea 2: Sensores Analogos con display TFT
    Tecnologia Digital
    Universidad Nacional de Colombia
    
    Juan Camilo Vargas Q. - jcvargasq@unal.edu.co
    Sergio Alejandro Vargas Q. - savargasqu@unal.edu.co
    
    2020-04-13
*/

// TODO: Fix dimensional arguments

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

#define cs   10
#define ds   9
#define rst  8

TFT TFTscreen = TFT(cs, ds, rst);

// Strings to print to the screen
char printoutA[4];
char printoutB[4];

int aval;
int aval;

const int led_size = 32; // Size of LED representing circles

// RGB 565: RRRR RGGG GGGB BBBB
const int R = 0xF800;
const int G = 0x07E0;
const int B = 0x001F;
const int Y = 0xFFE0;
const int BG = 0x0000; // Background color

// Sensor pins
const int sensorA = A0;
const int sensorB = A1;

// Sketch variables 
int valA;
int valB;
int diff;

// Turn off all LED circles except for one. Pass color as argument
void turnON(int color, int x) {
	for (int i = led_size; i <= led_size*4; i += led_size) {
	        TFTscreen.fillCircle(50, 60, 5, BG);
		//ellipse(i, led_size, led_size, led_size); // (x, y, w, h);
	}
	TFTscreen.fillCircle(50, 60, 5, color);
	//ellipse(x, led_size, led_size, led_size); // (x, y, w, h);
}

void setup() {
	// Put this line at the beginning of every sketch that uses the GLCD:
	TFTscreen.begin();
	// clear the screen with a black background
	TFTscreen.background(0, 0, 0);
	// write the static text to the screen
	// set the font color to white
	TFTscreen.stroke(255, 255, 255);
	// set the font size
	TFTscreen.setTextSize(2);
	// write the text to the top left corner of the screen
	TFTscreen.text("Sensor 1 Value :\n ", 0, 0);
	// set the font size very large for the loop
	TFTscreen.setTextSize(1);
}

void loop() {
/****** Print valA and valB ******/
	// Read Sensors
	valA = map(analogRead(sensorA), 0, 1023, 0, 300);
	valB = map(analogRead(sensorB), 0, 1023, 0, 200);
	//convert to string
	printoutA = intToString(valA);
	printoutB = intToString(valB);
	// set the font color white
	TFTscreen.stroke(255, 255, 255);
	// print the sensor value
	TFTscreen.text(printoutA, 0, 20);
	TFTscreen.text(printoutB, 0, 20);
	// wait for a moment
	delay(250);
	// erase the text you just wrote
	TFTscreen.stroke(0, 0, 0);
	TFTscreen.text(printoutA, 0, 20);
	TFTscreen.text(printoutB, 0, 20);

/****** Draw corresponding LED circle *****/
	if (valA >= 250){
		turnON(R);
	} else if (valA < 180){
		diff = valA - valB;
		if (diff > 30 && diff < 70){
			turnON(Y);
		} else if (diff >= 70){
			turnON(B);
		} else {
			turnON(G);
		}
	} else {
		turnON(G);
	}
}

char* intToString (int aval){
	int mil, cen, dec, uni;
        char sensorPrintout[4];

	mil=(aval/1000) % 10;
	cen=(aval/100) % 10;
	dec=(aval/10) % 10;
	uni=aval % 10;

	sensorPrintout[0] = '0' + mil;
	sensorPrintout[1] = '0' + cen;
	sensorPrintout[2] = '0' + dec;
	sensorPrintout[3] = '0' + uni;

	if(mil == 0){
		sensorPrintout[0] = ' ';
		if(cen == 0){
			sensorPrintout[1] = ' ';
			if(dec == 0)sensorPrintout[2] = ' ';

		}
	}
	return sensorPrintout;
}

