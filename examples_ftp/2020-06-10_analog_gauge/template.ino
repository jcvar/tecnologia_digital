include "common.h"

#define MILLIS 200
#define ANALOG_PIN A0

int sensor = 0

TFT TFTscreen = TFT(cs, dc, rst);

void setup() {
	// Initialize screen
	TFTscreen.begin();
	TFTscreen.background(0, 0, 0);
	// Draw court
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
}


void loop() {



}
