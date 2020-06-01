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

Mario myMario;

//========================= SETUP ==========================
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
//========================= LOOP ==========================
void loop() {
	myMario.walking();
	myMario.jumping(2000,4);		
}
//=========================================================
