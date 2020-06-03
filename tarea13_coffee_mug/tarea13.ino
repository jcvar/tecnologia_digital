/*
Tarea 13
Coffee Mug

Juan Camilo Vargas Q.
Sergio Alejandro Vargas Q.

Tecnologia Digital
Universidad Nacional de Colombia
2020-06-03
*/
#include "common.h"
#include "coffee_mug.h"

// Image params
#define X 64
#define Y 48
#define W 32
#define H 32

// Fill params
#define FX 71
#define FY 58
#define FW 18
#define FH 18


#define ANALOG_PIN A0
#define MILLIS 200

const int strSize = 5;
char val_str[strSize] = {0, 0, 0, 0};
int sensor = 0;

// Function declatarions
void draw_image(int, int, int, int);
void draw_mug(int, int, int, int, int);
void print_val(char[], int);

void setup() {
	TFTscreen.begin();
	TFTscreen.background(0, 0, 0);
	
	// Draw court and mug
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	draw_image(X, Y, W, H);
	
	// Print sensor info
	TFTscreen.stroke(COLOR_WHITE);
	TFTscreen.text("SENSOR: ", 80, 12);
	print_val(val_str, sensor);
}

void loop() {
	static unsigned long prev_ms = 0;
	if(millis() - prev_ms >= MILLIS){
		prev_ms = millis();
		sensor = analogRead(ANALOG_PIN);
		print_val(val_str, sensor);
		draw_mug(sensor, FX, FY, FW, FH);
		
	}
}

void draw_mug(int val, int fx, int fy, int fw, int fh){
	static int fill = 0;
	fill = map(val, 0, 1023, 0, fh);
	TFTscreen.fillRect(fx, fy, fw, fh-fill, COLOR_BLACK);
	TFTscreen.fillRect(fx, fy+(fh - fill), fw, fill, COLOR_ORANGE);
	
	if(fill == fh){
		draw_image(X, Y, W, 9);
	} else {
		TFTscreen.fillRect(X, Y, W, 9, COLOR_BLACK);
	}
}

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
		} else {
			break;
		}
	}
}

void print_val(char str[], int pr_val){
	TFTscreen.setTextSize(1);
	TFTscreen.stroke(COLOR_BLACK);
	TFTscreen.text(str, 128, 12); // Must erase previous score
	int_to_str(str, pr_val);
	TFTscreen.stroke(COLOR_WHITE);
	TFTscreen.text(str, 128, 12);
}

void draw_image(int x, int y, int w, int h){
	for (int row=0; row<h; row++){
		for (int col=0; col<w; col++){
			word p = pgm_read_word(coffee_mug + (w * row + col));
			TFTscreen.drawPixel(x + col, y + row, p);
		}
	}
}

