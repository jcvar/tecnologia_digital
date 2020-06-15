/*
	moving square in a court
*/
#include "../common.h"
TFT TFTscreen = TFT(CS, DC, RST);

// ball direction parameters
int ballDirectionX = 1;
int ballDirectionY = 1;

// ball parameters
int initBallX=6,initBallY=11;
float ballX=initBallX, ballY=initBallY, oldBallX, oldBallY;
int ballW=5,ballH=5;

// court instance
court_t court;

// ball move state
enum state_t {	sx, sd, sy};
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
	court.bottom=COURT_Y+courtH;
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
float slope = (108.0/75.0);
void moveBall(){
	//static int count=0;
	switch(state){
		case sx:
			ballX += ballDirectionX;
			//ballY += ballDirectionY;
			if(ballX == (80) - 2 && ballDirectionX > 0){
				state=sd;
			} else if (ballX == (court.left)+1){
				state=sy;
			}
			break;
		case sd:
			ballX += ballDirectionX;
			ballY += slope* ballDirectionY;
			if(ballY > (court.bottom-ballH)-2){
				count++;
				ballDirectionX*=-1;
				ballDirectionY*=-1;
				if(count==3){
					state = sx;
				}
			} else  if(ballY < (court.top)+2){
				ballDirectionX*=-1;
				ballDirectionY*=-1;
			}
			break;
		case sy:
			ballY += ballDirectionY;
			if(ballY < (64)-2){
				ballDirectionY=0;
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
