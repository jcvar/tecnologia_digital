/*
	moving square in a court
*/
#include "../common.h"
TFT TFTscreen = TFT(CS, DC, RST);

// ball direction parameters
int ballDirectionX = 1;
int ballDirectionY = 1;

// ball parameters
int initBallX=5,initBallY=11;
int ballX=initBallX, ballY=initBallY, oldBallX, oldBallY;
int ballW=5,ballH=5;

// court instance
court_t court;

// ball move state
enum state_t {	sx, sy};
state_t state=sx;

void setup() {
	// initialize the display
	TFTscreen.begin();
	// black background
	TFTscreen.background(0, 0, 0);
	// draw court
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);

	// court limits
	court.top=COURT_Y;
	court.bottom=COURT_Y+COURT_H;
	court.left=COURT_X;
	court.right=COURT_X+COURT_W;

	delay(2000);
}

void loop() {
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
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
			if(ballX == (court.right)/2 - 2){
				state=sy;
			} else if (ballX == (court.right)-ballW-1){
				ballDirectionX=0;
				//state= NULL;
				//exit(0);
			}
			break;
		case sy:
			ballY += ballDirectionY;
			if(ballY > (court.bottom-ballH)-2){
				count++;
				ballDirectionY*=-1;
				if(count==3){
					state = sx;
				}
			} else  if(ballY < (court.top)+2){
				ballDirectionY*=-1;
			}
			break;
	}

	// If the ball moved
	if (oldBallX != ballX || oldBallY != ballY) {
		// erase the ball's previous position
		TFTscreen.fill(0, 0, 0);
		TFTscreen.rect(oldBallX, oldBallY, ballW, ballH);
		// draw the ball's current position
		TFTscreen.fill(255, 255, 0);
		TFTscreen.rect(ballX, ballY, ballW, ballH);
	}
	oldBallX = ballX;
	oldBallY = ballY;
}
