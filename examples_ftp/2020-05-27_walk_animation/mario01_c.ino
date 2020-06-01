/*
	draw image img1 96x80
*/
#include <SPI.h>
#include "mario_img.h"
#include "mario.h"

// court parameters
int courtX=5,courtY=10,courtW=150,courtH=108;
int courtTopLine=0,courtBottomLine=0,courtLeftLine=0,courtRightLine=0;

Mario myMario;

//========================= SETUP ==========================

void setup() {
	// initialize the display
	TFTscreen.begin();
	// white background
	TFTscreen.background(0, 0, 0);
	
	// draw court
	TFTscreen.drawRect(courtX, courtY, courtW,courtH,0xF800);
	
	// court limits
	courtTopLine=courtY;
	courtBottomLine=courtY+courtH;
	courtLeftLine=courtY;
	courtRightLine=courtX+courtW;
	
	myMario.posX=72;
	myMario.posY=50;
	myMario.jumpPosY=21;
	myMario.isJumping=false;
	myMario.state=t0;
	myMario.walkingDelay=150;
	
}

//========================= LOOP ==========================
void loop() {
	myMario.walking();
	myMario.jumping(2000,4);		
}
//=========================================================





