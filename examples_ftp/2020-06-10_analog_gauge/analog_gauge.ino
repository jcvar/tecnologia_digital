#include "common.h"

#define MILLIS 200
#define ANALOG_PIN A0
#define X 80
#define Y 96
#define RAD 64

int sensor = 0;

TFT TFTscreen = TFT(cs, dc, rst);

void setup() {
	// Initialize screen
	TFTscreen.begin();
	TFTscreen.background(0, 0, 0);
	// Draw court
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
}


void loop() {
	static unsigned long prev_ms = 0;
	if (millis() - prev_ms >= MILLIS) {
		prev_ms = millis();
		sensor = analogRead(ANALOG_PIN);
		draw_gauge(sensor, X, Y, RAD);
	}
}

double angle = 0;
double arcx = 0;
double arcy = 0;

void draw_gauge(int val, int cx, int cy, int rad) {	// val, center x, y
	//static int angle = 0;
	//static float arcx = 0;
	//static float arcy = 0;
	
	TFTscreen.fillCircle(arcx, arcy, 4, COLOR_BLACK);
	TFTscreen.fillCircle(arcx, arcy, 4, COLOR_BLACK);
	TFTscreen.fillCircle(arcx, arcy, 4, COLOR_BLACK);
	TFTscreen.fillCircle(arcx, arcy, 4, COLOR_BLACK);
	
	if(angle >= PI*3/4){
		TFTscreen.drawLine(arcx, arcy, cx, cy, COLOR_BLACK);
	} else if(angle >= PI/2){
		TFTscreen.drawLine(arcx, arcy, cx, cy, COLOR_BLACK);
		TFTscreen.drawLine(cx, cy, arcx, arcy, COLOR_BLACK);
	} else if(angle >= PI/4){
		TFTscreen.drawLine(cx, cy, arcx, arcy, COLOR_BLACK);
		TFTscreen.drawLine(arcx, arcy, cx, cy, COLOR_BLACK);
	} else if(angle >= 0){
		TFTscreen.drawLine(cx, cy, arcx, arcy, COLOR_BLACK);
	}
	
	angle = PI-PI*(val/1023.0);
	arcx = cos(angle)*rad + cx;
	arcy = -sin(angle)*rad + cy;
	
	
	TFTscreen.fillCircle(arcx, arcy, 4, COLOR_GREEN);
	if(angle >= PI*3/4){
		TFTscreen.drawLine(arcx, arcy, cx, cy, COLOR_RED);
	} else if(angle >= PI/2){
		TFTscreen.drawLine(arcx, arcy, cx, cy, COLOR_RED);
		TFTscreen.drawLine(cx, cy, arcx, arcy, COLOR_RED);
	} else if(angle >= PI/4){
		TFTscreen.drawLine(arcx, arcy, cx, cy, COLOR_RED);
		TFTscreen.drawLine(cx, cy, arcx, arcy, COLOR_RED);
	} else if(angle >= 0){
		TFTscreen.drawLine(cx, cy, arcx, arcy, COLOR_RED);
	}
	
}
