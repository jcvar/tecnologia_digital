/*
Tarea 12
Mario

Juan Camilo Vargas Q.
Sergio Alejandro Vargas Q.

Tecnologia Digital
Universidad Nacional de Colombia
2020-06-01
*/
// img size: 96x80
#include <SPI.h>
#include "mario_img.h"
#include "mario.h"

// court parameters
#define COURT_X 5
#define COURT_Y 10
#define COURT_W 150
#define COURT_H 108
// court limits
//int courtTopLine=0,courtBottomLine=0,courtLeftLine=0,courtRightLine=0;
//courtTopLine=courtY;
//courtBottomLine=courtY+courtH;
//courtLeftLine=courtY;
//courtRightLine=courtX+courtW;

#define BLOCK_SIZE 10

Mario myMario;

typedef struct {
	int posX;
	int posY;
	int oldX;
	int oldY;
	int w;
	int h;
	word color;
} block;


block block_g = {
		COURT_X + COURT_W - BLOCK_SIZE - 1,
		COURT_Y + COURT_H - BLOCK_SIZE - 1,
		COURT_X + COURT_W - BLOCK_SIZE - 1,
		COURT_Y + COURT_H - BLOCK_SIZE - 1,
		BLOCK_SIZE,
		BLOCK_SIZE,
		COLOR_GREEN
	};

block block_r = {
		COURT_X + COURT_W - BLOCK_SIZE - 1,
		COURT_Y + COURT_H - BLOCK_SIZE - 1,
		COURT_X + COURT_W - BLOCK_SIZE - 1,
		COURT_Y + COURT_H - BLOCK_SIZE - 1,
		BLOCK_SIZE,
		BLOCK_SIZE,
		COLOR_RED
	};


void setup() {
	TFTscreen.begin();
	TFTscreen.background(0, 0, 0);
	
	// Mario initialization
	myMario.posX=20;
	myMario.posY=88;
	myMario.jumpPosY=60;
	myMario.isJumping=false;
	myMario.state=t0;
	myMario.walkingDelay=150;
	
	// Draw court and ground
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	//TFTscreen.drawFastHLine()
}


void loop() {
	myMario.walking();
	myMario.jumping(2000,4);		
	move_blocks();
}

void draw_rect(block b){
	TFTscreen.fillRect(b.oldX, b.oldY, b.w, b.h, COLOR_BLACK);
	TFTscreen.fillRect(b.posX, b.posY, b.w, b.h, b.color);
}

typedef enum {	s0, s1, s2}  block_state;

void move_blocks() {
	static block_state bs = s0;
	switch(bs) {
		case s0:
			block_g.posX -= 1;
			if (block_g.posX == 80){
				bs = s1;
			}
			break;
			
		case s1:
			block_g.posX -= 1;
			block_r.posX -= 1;
			break;
			
		case s2:
			// GAME OVER
			break;
	}
	
	if (block_g.posX != block_g.oldX || block_g.posY != block_g.oldY){
		draw_rect(block_g);
		block_g.oldX = block_g.posX;
		block_g.oldY = block_g.posY;
	}
	if (block_r.posX != block_r.oldX || block_r.posY != block_r.oldY){
		draw_rect(block_r);
		block_r.oldX = block_r.posX;
		block_r.oldY = block_r.posY;
	}
}

