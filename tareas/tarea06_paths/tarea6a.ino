/*
	moving square in a court
*/
#include "../common.h"
TFT TFTscreen = TFT(CS, DC, RST);

// ball direction parameters
int ballDirectionX = 1;
int ballDirectionY = 1;

// ball parameters
int initBallX=5,initBallY=12;
int ballX=initBallX, ballY=initBallY, oldBallX, oldBallY;
int ballW=5,ballH=5;

// court instance
court_t court;

// ball move state
enum state_t {	s0,s1,s2,s3, s4};
state_t state=s0;

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
		case s0:
			ballX += ballDirectionX;
			//ballY += ballDirectionY;
			if(ballX > (court.right-ballW)/3 - 2){
				state=s1;
			} else if (ballX < (court.left)){
				ballDirectionX *= -1;
				ballDirectionY *= -1;
			}
			break;
		case s1:
			ballY += ballDirectionY;
			if(ballY > (court.bottom-ballH)-2){
				state=s2;
			} else  if(ballY < (court.top)+2){
				state=s0;
			}
			break;
		case s2:
			ballX += ballDirectionX;
			if(ballX >  2*(court.right-ballW)/3 - 2){
				state=s3;
			} else if (ballX < (court.right-ballW)/3 - 2){
				state=s1;
			}
			break;
		case s3:
			ballY -= ballDirectionY;
			if(ballY < (court.top)+2){
				state=s4;
			}else if(ballY > (court.bottom-ballH)-2){
				state=s2;
			}
			break;
		case s4:
			ballX += ballDirectionX;
			if(ballX > (court.right-ballW) - 2){
				ballDirectionX*= -1;
				ballDirectionY*= -1;
			}else if (ballX <  2*(court.right-ballW)/3 - 2){
				state=s3;
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
