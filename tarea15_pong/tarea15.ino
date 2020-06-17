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
#include "paddle.h"
#include "ball.h"

#define MILLIS_BALL 50
#define MILLIS_PADDLE 50

#define BALL_W 5
#define BALL_H 5
#define PADDLE_W 5
#define PADDLE_H 20

#define PIN_LEFT A0
#define PIN_RIGHT A1

TFT TFTscreen = TFT(CS, DC, RST);
court_t court = {COURT_Y + 5 + 1, COURT_Y + 5 + COURT_H - 1, COURT_X + 1, COURT_X + COURT_W - 1};

// Ball(pX, pY, dX, dY);
Ball ball(80, 64, 1, 1);

// Paddle(pX, pY);
Paddle left(court.left, court.top);
Paddle right(court.right - PADDLE_W, court.top);

bool check_collision(Ball b, Paddle p){
	if(p.posX <= b.posX + b.w &&
		p.posX + p.w >= b.posX &&
	p.posY <= b.posY + b.h &&
	p.posX + p.h >= b.posX){
		return true;
	}
	return false;
}


void setup() {
	// Initialize screen
	TFTscreen.begin();
	TFTscreen.background(COLOR_BLACK);
	// Draw court
	TFTscreen.drawRect(COURT_X, COURT_Y + 5, COURT_W, COURT_H, COLOR_RED);
}

void loop() {
	static unsigned long millis_ball = 0;
	static unsigned long millis_left = 0;
	static unsigned long millis_right = 0;
	
	if(millis() - millis_left > MILLIS_PADDLE){
		millis_left = millis();
		left.move(map(analogRead(PIN_LEFT), 1023, 0, court.top, court.bottom - PADDLE_H));
		left.draw(TFTscreen);
	}
	if(millis() - millis_right > MILLIS_PADDLE){
		millis_right = millis();
		right.move(map(analogRead(PIN_RIGHT), 1023, 0, court.top, court.bottom - PADDLE_H));
		right.draw(TFTscreen);
	}
	
	if(millis() - millis_ball > MILLIS_BALL){
		millis_ball = millis();
		ball.move(court);
		ball.draw(TFTscreen);
		if(check_collision(ball, left) || check_collision(ball, right)){
			ball.speedX *= -1;
		}
	}
}

