/*
Tarea 00
Title

Juan Camilo Vargas Q.
Sergio Alejandro Vargas Q.

Tecnologia Digital
Universidad Nacional de Colombia
2020-00-00
*/
#include "../common.h"

typedef unsigned short hu // shorthand

TFT TFTscreen = TFT(CS, DC, RST);
court_t court;

hu hour, minute;
unsigned long curMillis;

void setup() {
	// Initialize screen
	TFTscreen.begin();
	TFTscreen.background(COLOR_BLACK);
	// Court initialization
	//court.top = COURT_Y;
	//court.bottom = COURT_Y + COURT_H;
	//court.left = COURT_X;
	//court.right = COURT_X + COURT_W;
	// Draw court
	//TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
}

void loop() {


}

void update_time(hu *hour, hu *minute) {


	hour 

}

void draw_digit() {}




