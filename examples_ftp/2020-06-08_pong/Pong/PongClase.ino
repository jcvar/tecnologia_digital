#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

#include <pgmspace.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

TFT TFTscreen = TFT(cs, dc, rst);

// Some ready-made 16-bit ('565') color settings:
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_RED 0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_BLUE 0x001F
#define COLOR_CYAN 0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_YELLOW 0xFFE0
#define COLOR_ORANGE 0xFC00

#include "images.h"
#include "ball.h"
#include "paddle.h"

// variables for the position of the ball and paddle
int paddleY = 0;

int courtX=5,courtY=10,courtW=150,courtH=108;

int courtTopLine=0,courtBottomLine=0,courtLeftLine=0,courtRightLine=0;

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
	
	courtTopLine=courtY;
	courtBottomLine=courtY+courtH;
	courtLeftLine=courtY;
	courtRightLine=courtX+courtW;
	
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




