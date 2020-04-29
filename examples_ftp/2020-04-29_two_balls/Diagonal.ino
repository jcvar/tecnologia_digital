/*
	moving square in a court: diagonal 
*/

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

TFT TFTscreen = TFT(cs, dc, rst);

// ball direction parameters
float ballDirectionX = 1;
float ballDirectionY = 103.0/145;

// ball parameters
int initBallX=5,initBallY=10;
int ballX=initBallX, ballY=initBallY, oldBallX=0, oldBallY=0;
int ballW=5,ballH=5;
float floatBallX=initBallX,floatBallY=initBallY;

// court parameters
int courtX=5,courtY=10,courtW=150,courtH=108;
int courtTopLine=0,courtBottomLine=0,courtLeftLine=0,courtRightLine=0;

// ball move state
enum state_t {	s0,s1};
state_t state=s0;

void setup() {
	// initialize the display
	TFTscreen.begin();
	// black background
	TFTscreen.background(0, 0, 0);
	
	// draw court
	TFTscreen.drawRect(courtX, courtY, courtW,courtH,0xF800);
	
	// court limits
	courtTopLine=courtY;
	courtBottomLine=courtY+courtH;
	courtLeftLine=courtY;
	courtRightLine=courtX+courtW;
	
	// initial position
	ballX=initBallX; ballY=initBallY;
	
	delay(2000);
	
}

void loop() {
	TFTscreen.drawRect(5, 10, 150, 108,0xF800);
	moveBall();
	delay(25);
}

void moveBall(){	
	switch(state){
		case s0:
			floatBallX+=ballDirectionX;
			floatBallY+=ballDirectionY;
			ballX = floatBallX;
			ballY = floatBallY;	
			if(ballX > (courtRightLine-ballW)-2){
				state=s1;
			}		
			break;			
		case s1:
			floatBallX-=ballDirectionX;
			floatBallY-=ballDirectionY;
			ballX = floatBallX;
			ballY = floatBallY;	
			if(ballX<initBallX+2){
				state=s0;
			}	
			break;			
	}
	
	// erase the ball's previous position
	TFTscreen.fill(0, 0, 0);
	
	if (oldBallX != ballX || oldBallY != ballY) {
		TFTscreen.rect(oldBallX, oldBallY, ballW, ballH);
	}
	
	// draw the ball's current position
	TFTscreen.fill(255, 255, 0);
	TFTscreen.rect(ballX, ballY, ballW, ballH);
	
	oldBallX = ballX;
	oldBallY = ballY;
	
}




