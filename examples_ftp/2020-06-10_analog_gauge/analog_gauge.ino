#include "common.h"

#define MILLIS 200
#define ANALOG_PIN A0
#define CX 80
#define CY 96
#define RAD 20

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
		draw_gauge(sensor, CX, CY, RAD);
	}
}

void draw_gauge(int val, int cx, int cy, int rad) {	// val, center x, y
	static int angle = 0;
	static float arcx = 0;
	static float arcy = 0;
	
	angle = map(val, 0, 1023, PI, 0);
	arcx = cos(angle)*rad*rad + cx;
	arcy = sin(angle)*rad + cy;
	
	TFTscreen.drawLine(arcx, arcy, cx, cy, COLOR_RED);
}
