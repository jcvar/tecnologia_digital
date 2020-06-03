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

#define ANALOG_PIN A0
#define MILLIS 200
const int strSize = 5;
char val_str[strSize] = {0, 0, 0, 0};
int sensor = 0;

// Function declatarions
void draw_mug(int);
void print_val();

void setup() {
	TFTscreen.begin();
	TFTscreen.background(0, 0, 0);
	
	// Draw court and ground
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	//TFTscreen.drawFastHLine()
	TFTscreen.stroke(COLOR_WHITE);
	TFTscreen.text("SENSOR: ", 90, 15);
	print_val();
}

void loop() {
	static unsigned long prev_ms = 0;
	if(millis() - prev_ms >= MILLIS){
		prev_ms = millis();
		sensor = analogRead(ANALOG_PIN);
		draw_mug(sensor);
		print_val();
	}
}

void draw_mug(int val){
	static int fill = 0;
	fill = map(val, 0, 1023, 0, 31);
	TFTscreen.fillRect(0, 0, 32, fill, COLOR_RED);
	TFTscreen.fillRect(0, fill, 32, 32-fill, COLOR_BLACK);
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
		}
	}
}

void print_val(){
	TFTscreen.setTextSize(1);
	TFTscreen.stroke(COLOR_BLACK);
	TFTscreen.text(val_str, 135, 15); // Must erase previous score
	int_to_str(val_str, sensor);
	TFTscreen.stroke(COLOR_WHITE);
	TFTscreen.text(val_str, 135, 15);
}

