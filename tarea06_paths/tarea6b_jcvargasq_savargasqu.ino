/*
	moving square in a court
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
int ballDirectionX = 1;
int ballDirectionY = 1;

// ball parameters
int initBallX=5,initBallY=11;
int ballX=initBallX, ballY=initBallY, oldBallX, oldBallY;
int ballW=5,ballH=5;

// court parameters
int courtX=5,courtY=10,courtW=150,courtH=108;
int courtTopLine=0,courtBottomLine=0,courtLeftLine=0,courtRightLine=0;

// ball move state
enum state_t {	sx, sy};
state_t state=sx;

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
	courtLeftLine=courtX;
	courtRightLine=courtX+courtW;
	
	delay(2000);
	
}

void loop() {
	TFTscreen.drawRect(5, 10, 150, 108,0xF800);
	moveBall();
	delay(25);
}

int count=0;
void moveBall(){
	//static int count=0;
	switch(state){
		case sx:
			ballX += ballDirectionX;
			//ballY += ballDirectionY;
			if(ballX == (courtRightLine)/2 - 2){
				state=sy;
			} else if (ballX == (courtRightLine)-ballW-1){
				ballDirectionX=0;
				//state= NULL;
				//exit(0);
			}
			break;
		case sy:
			ballY += ballDirectionY;
			if(ballY > (courtBottomLine-ballH)-2){
				count++;
				ballDirectionY*=-1;
				if(count==3){
					state = sx;
				}
			} else  if(ballY < (courtTopLine)+2){
				ballDirectionY*=-1;
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
