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

#define CENTER_X 80
#define CENTER_Y 64

#define BALL_W 5
#define BALL_H 5
#define PADDLE_W 5
#define PADDLE_H 20

#define PIN_LEFT A0
#define PIN_RIGHT A1
#define PIN_START 4

#define MAX_SCORE 5 // MAX: 9

TFT TFTscreen = TFT(CS, DC, RST);
court_t court = {COURT_Y + 5 + 1, COURT_Y + 5 + COURT_H - 1, COURT_X + 1, COURT_X + COURT_W - 1};

// Ball(pX, pY, dX, dY);
Ball ball(CENTER_X, CENTER_Y, -1, 1);

// Paddle(pX, pY);
Paddle left(court.left, court.top);
Paddle right(court.right - PADDLE_W, court.top);

// Game variables
bool start = false;
bool game_over = true;

const int strSize = 2;
int score_left;
int score_right;
char score_str_left[strSize] = {0};
char score_str_right[strSize] = {0};

bool check_collision(Ball b, Paddle p){
	if(p.posX <= b.posX + b.w &&
		p.posX + p.w >= b.posX &&
	p.posY <= b.posY + b.h &&
	p.posY + p.h >= b.posY){
		return true;
	}
	return false;
}

bool update_score(int result){
	if(result == -1){ // Goal on left side
		score_right += 1;

		TFTscreen.stroke(COLOR_BLACK);
		TFTscreen.text(score_str_right, 114, 4); // Must erase previous score

		score_str_right[0] = '0' + score_right;

		TFTscreen.stroke(COLOR_WHITE);
		TFTscreen.text(score_str_right, 114, 4);
		return true;
	} else if(result == 1){ // Goal on right side
		score_left += 1;

		TFTscreen.stroke(COLOR_BLACK);
		TFTscreen.text(score_str_left, 40, 4); // Must erase previous score

		score_str_left[0] = '0' + score_left;

		TFTscreen.stroke(COLOR_WHITE);
		TFTscreen.text(score_str_left, 40, 4);
		return true;
	}
	return false;
}

void reset_score(){
	score_left = -1;
	score_right = -1;
	update_score(-1);
	update_score(1);
}

void setup() {
	// Initialize screen
	TFTscreen.begin();
	TFTscreen.background(COLOR_BLACK);
	// Draw court
	TFTscreen.drawRect(COURT_X, COURT_Y + 5, COURT_W, COURT_H, COLOR_RED);

	TFTscreen.stroke(COLOR_WHITE);
	TFTscreen.text("PONG", 68, 4);
	reset_score();
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

		if(!game_over){
			// RUN GAME LOGIC
			ball.move(court);
			ball.draw(TFTscreen);
			game_over = update_score(ball.check_goal(court));
			if(check_collision(ball, left) || check_collision(ball, right)){
				ball.speedX *= -1;
			}
		} else if(score_left < MAX_SCORE && score_left < MAX_SCORE){
			// NEW ROUND: RESET BALL
			TFTscreen.fillRect(ball.posX, ball.posY, BALL_W, BALL_H, COLOR_BLACK);
			ball.posX = CENTER_X;
			ball.posY = CENTER_Y;
			ball.speedX *= -1;
			game_over = false;
		} else if(start){
			// NEW GAME: RESET BALL AND SCOREBOARD
			start = false;
			reset_score();
			TFTscreen.fillRect(ball.posX, ball.posY, BALL_W, BALL_H, COLOR_BLACK);
			ball.posX = CENTER_X;
			ball.posY = CENTER_Y;
			ball.speedX *= -1;
			ball.draw(TFTscreen);
			game_over = false;
		} else if(game_over && digitalRead(PIN_START)){
			start = true;
		}
	}
}

