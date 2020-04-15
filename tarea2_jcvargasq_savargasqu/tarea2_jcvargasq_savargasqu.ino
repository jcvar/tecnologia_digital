/*
    Tarea 2: Sensores Analogos con display TFT
    Tecnologia Digital
    Universidad Nacional de Colombia
    
    Juan Camilo Vargas Q. - jcvargasq@unal.edu.co
    Sergio Alejandro Vargas Q. - savargasqu@unal.edu.co
    
    2020-04-13

// TODO: Fix dimensional arguments
*/
#include <TFT.h>
#include <SPI.h>

// Display parameters
#define cs 10
#define ds 9
#define rst 8
// Declare display
TFT TFTscreen = TFT(cs, ds, rst);

//  RGB 565 = RRRR RGGG GGGB BBBB
const int R = 0xF800;
const int G = 0x07E0;
const int B = 0x001F;
const int Y = 0xFFE0;
// Background color
const int K = 0x0000;

// Size of circle representing the LED
#define led_size   16
#define led_pos    40
#define led_offset 20

// Sensor pins
const int sensorA = A0;
const int sensorB = A1;

// Strings to print to the screen
const int strSize = 4;
char printoutA[strSize] = {0, 0, 0};
char printoutB[strSize] = {0, 0, 0};

// Sketch variables 
int valA;
int valB;
int oldA = 0;
int oldB = 0;
int diff;

int pos;
int new_clr;
int old_clr = K;

// Convert integer value to string
void ints_to_strings(int vA, int vB){
	int numA, numB;
	for(int i = 2; i>=0; i--){
		numA = vA%10;
		numB = vB%10;
		printoutA[i] = '0' + numA;
		printoutB[i] = '0' + numB;
		vA/=10;
		vB/=10;
	}	
}

// Turn off all LED circles except for one. Pass color as argument
void turnON(int color, int x) {
	if (color != old_clr){
		old_clr = color;
		for (int i = 0; i < 4; i ++) {
			TFTscreen.fillCircle(i*led_pos + led_offset, led_offset, led_size, K);
			//ellipse(i, led_size, led_size, led_size); // (x, y, w, h);
		}
		
		TFTscreen.drawCircle(0*led_pos + led_offset, led_offset, led_size, R);
		TFTscreen.drawCircle(1*led_pos + led_offset, led_offset, led_size, G);
		TFTscreen.drawCircle(2*led_pos + led_offset, led_offset, led_size, B);
		TFTscreen.drawCircle(3*led_pos + led_offset, led_offset, led_size, Y);
		TFTscreen.fillCircle(x*led_pos + led_offset, led_offset, led_size, color);
		//ellipse(x, led_size, led_size, led_size); // (x, y, w, h);
	}
}

void write_values(int vA, int vB){
	if (vA != oldA){
		oldA = vA;
		TFTscreen.stroke(0, 0, 0);
		TFTscreen.text(printoutA,  60, 40);
		ints_to_strings(vA, vB);
		TFTscreen.stroke(255, 255, 255);
		TFTscreen.text(printoutA,  60, 40);
	}
	if (vB != oldB){
		oldB = vB;
		TFTscreen.stroke(0, 0, 0);
		TFTscreen.text(printoutB, 140, 40);	
		ints_to_strings(vA, vB);
		TFTscreen.stroke(255, 255, 255);
		TFTscreen.text(printoutB, 140, 40);
	}
}

void setup() {
	TFTscreen.begin();
	TFTscreen.background(0, 0, 0);
	TFTscreen.setTextSize(1);
	
	TFTscreen.stroke(255, 255, 255);
	TFTscreen.text("val A: ",  20, 40);
	TFTscreen.text("val B: ", 100, 40);
}

void loop() {
	// Read Sensors
	valA = map(analogRead(sensorA), 0, 1023, 0, 300);
	valB = map(analogRead(sensorB), 0, 1023, 0, 200);
	write_values(valA, valB);
	
/****** Draw corresponding LED circle *****/	
	if (valA >= 250){
		new_clr = R; pos = 0;
	} else if (valA < 180){
		diff = valA - valB;
		if (diff > 30 && diff < 70){
			new_clr = Y; pos = 3;
		} else if (diff >= 70){
			new_clr = B; pos = 2;
		} else {
			new_clr = G; pos = 1;
		}
	} else {
		new_clr = G; pos = 1;
	}
	turnON(new_clr, pos);
	// Plot pixel
	TFTscreen.drawPixel(map(valA, 0, 300, 20, 140), map(valB, 0, 200, 127, 47), new_clr);
}