
// Ejercicio: Crear segunda paleta

#include "../common.h"

#include "images.h"
#include "ball.h"
#include "paddle.h"

TFT TFTscreen = TFT(cs, dc, rst);
court_t court;


// variables for the position of the ball and paddle
int paddleY = 0;
Ball myBall(20,33,2,3);
Paddle myPaddleRight(145,0,1,200);

bool inPaddle(Ball t_ball,Paddle t_rightPaddle) {
	bool result = false;
	
	if ((t_ball.posX >= t_rightPaddle.posX && t_ball.posX <= (t_rightPaddle.posX + 5)))
	{
		if(t_ball.posY >= t_rightPaddle.posY && t_ball.posY <= (t_rightPaddle.posY + 20))
		{ 
			result = true;
		}
	}
	
	return result;
}


void setup() {
	// initialize the display
	TFTscreen.begin();
	// background color
	TFTscreen.background(0x0099);
	
	TFTscreen.drawRect(courtX, courtY, courtW,courtH,0xF800);
	
	// Court initialization
	court.top = COURT_Y;
	court.bottom = COURT_Y + COURT_H;
	court.left = COURT_X;
	court.right = COURT_X + COURT_W;
}

void loop() {
	TFTscreen.drawRect(5, 10, 150, 108,0xF800);
	
	// map the potentiometer value to the position of paddle's location
	paddleY = map(analogRead(A1), 0, 1023,97,11);
	
	myBall.move();
	myPaddleRight.move(paddleY);
	bool r=inPaddle(myBall,myPaddleRight);
	if(r){
		myBall.deltaX=-myBall.deltaX;
		myBall.deltaY=-myBall.deltaY;
	}
	
	delay(10);
	
}

