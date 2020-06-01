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
	int w;
	int h;
	word color;
} block;


block block_g;
block block_r;


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

	block_g = {
		COURT_X + COURT_W - BLOCK_SIZE - 1,
		COURT_Y + COURT_H - BLOCK_SIZE - 1,
		BLOCK_SIZE,
		BLOCK_SIZE,
		COLOR_GREEN,
	};

	block_r = {
		COURT_X + COURT_W - BLOCK_SIZE - 1,
		COURT_Y + COURT_H - BLOCK_SIZE - 1,
		BLOCK_SIZE,
		BLOCK_SIZE,
		COLOR_RED,
	};
	
	// Draw court and ground
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	//TFTscreen.drawFastHLine()
}


void loop() {
	myMario.walking();
	myMario.jumping(2000,4);		
	move_blocks();
}


void move_blocks() {

	typedef enum block_state {s0, s1, s2};
	static block_state bs = s0;
	switch(bs) {
		case s0:
			if (block_g.posX > 
		break;
		case s0:
		break;
		case s0:
		break;
		case s0:
		break;
	}
	

}
