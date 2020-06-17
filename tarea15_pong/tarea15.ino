/*
Tarea 15
Pong

Juan Camilo Vargas Q.
Sergio Alejandro Vargas Q.

Tecnologia Digital
Universidad Nacional de Colombia
2020-06-17
*/
#include "../common.h"
#include "images.h"
TFT TFTscreen = TFT(CS, DC, RST);

#include "paddle.h"
#include "ball.h"

#define MILLIS_BALL 50
#define MILLIS_PADDLE 50


court_t court = {COURT_Y, COURT_Y + COURT_H, COURT_X, COURT_X + COURT_W};

//Ball ball = new Ball 

void setup() {
	// Initialize screen
	TFTscreen.begin();
	TFTscreen.background(COLOR_BLACK);
	// Draw court
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
}

void loop() {
	static unsigned long millis_ball = 0;
	static unsigned long millis_paddle = 0;

	if(millis() - millis_ball > MILLIS_BALL){
		
	}
}

