include "common.h"

TFT TFTscreen = TFT(CS, DC, RST);

void setup() {
	// Initialize screen
	TFTscreen.begin();
	TFTscreen.background(0, 0, 0);
	// Draw court
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
}

void loop() {

}

