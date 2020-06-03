/*
Tarea 13
Coffee Mug

Juan Camilo Vargas Q.
Sergio Alejandro Vargas Q.

Tecnologia Digital
Universidad Nacional de Colombia
2020-06-03
*/
#include <TFT.h>
//#include <SPI.h>
#include <avr/pgmspace.h>
#include "coffee_mug.h"

// Court parameters
#define COURT_X 5
#define COURT_Y 10
#define COURT_W 150
#define COURT_H 108

#define ANALOG_PIN A0
#define MILLIS 200
const int strSize = 5;
char val_str[strSize] = {0, 0, 0, 0};
int sensor = 0;

// Function declatarions
void draw_mug(int);
void draw_val();

void setup() {
	TFTscreen.begin();
	TFTscreen.background(0, 0, 0);
	
	// Draw court and ground
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	//TFTscreen.drawFastHLine()
	TFTscreen.stroke(COLOR_WHITE);
	TFTscreen.text("SENSOR: ", 90, 15);
	draw_val();
}

void loop() {
	static unsigned long prev_ms = 0;
	if(millis() - prev_ms >= MILLIS){
		prev_ms = millis();
		sensor = analogRead(ANALOG_PIN);
		draw_mug(sensor);
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
		}
	}
}

void draw_val(){
	TFTscreen.setTextSize(1);
	TFTscreen.stroke(COLOR_BLACK);
	TFTscreen.text(val_str, 135, 15); // Must erase previous score
	int_to_str(val_str, sensor);
	TFTscreen.stroke(COLOR_WHITE);
	TFTscreen.text(score_str, 135, 15);
}

