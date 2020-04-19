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
const int led_colors[4] = {R, G, B, Y};

// Size of circle representing the LED
#define led_size   16
#define led_pos    40
#define led_offset 20

// Sensor pins
const int sensorA = A0;
const int sensorB = A1;

// Strings to print to the screen (one extra char, Arduino ref.)
const int strSize = 4;
char printoutA[strSize] = {0, 0, 0};
char printoutB[strSize] = {0, 0, 0};

// Sketch variables 
int valA;
int valB;
int oldA = -1;
int oldB = -1;
int diff;

int new_led;
int old_led = -1;

// Convert integer value to string
void int_to_str(char arr[], int val){
	int num;
	for(int i = strSize - 2; i >=0; i--){
		num = val%10;
		arr[i] = '0' + num;
		val/=10;
	}
	for(int i = 0; i < strSize - 2; i++){
		if(arr[i] == '0'){
			arr[i] = ' ';
		}
	}
}

// Turn off all LED circles except for one. Pass color as argument
void turnON(int led) {
	if (led != old_led){
		old_led = led;
		for (int i = 0; i < 4; i ++) {
			TFTscreen.fillCircle(i*led_pos + led_offset, led_offset, led_size, K);
			TFTscreen.drawCircle(i*led_pos + led_offset, led_offset, led_size, led_colors[i]);
		}
		TFTscreen.fillCircle(led*led_pos + led_offset, led_offset, led_size, led_colors[led]);
	}
}

void write_values(int vA, int vB){
	if (vA != oldA){
		oldA = vA;
		TFTscreen.stroke(0, 0, 0);
		TFTscreen.text(printoutA,  60, 40);
		int_to_str(printoutA, vA);
		TFTscreen.stroke(255, 255, 255);
		TFTscreen.text(printoutA,  60, 40);
	}
	if (vB != oldB){
		oldB = vB;
		TFTscreen.stroke(0, 0, 0);
		TFTscreen.text(printoutB, 140, 40);	
		int_to_str(printoutB, vB);
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
		new_led = 0;
	} else if (valA < 180){
		diff = valA - valB;
		if (diff > 30 && diff < 70){
			new_led = 3;
		} else if (diff >= 70){
			new_led = 2;
		} else {
			new_led = 1;
		}
	} else {
		new_led = 1;
	}
	turnON(new_led);
	// Plot pixel
	TFTscreen.drawPixel(map(valA, 0, 300, 20, 140), map(valB, 0, 200, 127, 47), led_colors[new_led]);
}