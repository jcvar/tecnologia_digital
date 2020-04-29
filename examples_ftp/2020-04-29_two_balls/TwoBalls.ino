/*
	moving two squares in a court 
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

// ball direction parameters
int ballIncrementX = 1;
int ballIncrementY = 1;

// ball parameters struct
struct ball_t{
	int initX;
	int initY;
	int X;
	int Y;
	int oldX; 
	int oldY;
	int W;
	int H;
	unsigned long previousMillis;
};

ball_t ballA,ballB;  // instancias

// court parameters
int courtX=5,courtY=10,courtW=150,courtH=108;
int courtTopLine=0,courtBottomLine=0,courtLeftLine=0,courtRightLine=0;

// ballA move state
enum ballA_state_t {	s0,s1,s2,s3};
ballA_state_t ballAstate=s0;

// ballB move state
enum ballB_state_t {	t0,t1,t2,t3};
ballB_state_t ballBstate=t0;

//---------------------------------------------------------
//*********************  setup ****************************
void setup() {
	// initialize the display
	TFTscreen.begin();
	// black background
	TFTscreen.background(0, 0, 0);
	
	// draw court
	TFTscreen.drawRect(courtX, courtY, courtW,courtH,0xF800);
	
	//ballA parameters:
	ballA.initX=5;
	ballA.initY=62;
	ballA.X=ballA.initX;
	ballA.Y=ballA.initY;
	ballA.W=5;
	ballA.H=5;
	ballA.oldX=0;
	ballA.oldY=0;
	ballA.previousMillis=0;
	
	//ballB parameters:
	ballB.initX=78;
	ballB.initY=10+2;
	ballB.X=ballB.initX;
	ballB.Y=ballB.initY;
	ballB.W=5;
	ballB.H=5;
	ballB.oldX=0;
	ballB.oldY=0;
	ballB.previousMillis=0;
	
	// court limits
	courtTopLine=courtY;
	courtBottomLine=courtY+courtH;
	courtLeftLine=courtY;
	courtRightLine=courtX+courtW;
	
	delay(2000);	
}

//---------------------------------------------------------
//***********************  loop  **************************
unsigned long currentMillis=0;
void loop() {
	TFTscreen.drawRect(5, 10, 150, 108,0xF800);
	currentMillis=millis();
	if(currentMillis-ballA.previousMillis>25){
		ballA.previousMillis=currentMillis;
		ballA_move();
	}
	
	if(currentMillis-ballB.previousMillis>10){
		ballB.previousMillis=currentMillis;
		ballB_move();
	}
	
	//ballB_move();
	//delay(25);
}

//---------------------------------------------------------
//******************  ballB_move  *************************

int ballBCount=0;
void ballB_move(){
	switch(ballBstate){
		case t0:
			ballB.Y += ballIncrementY;	
			if(ballB.Y > (courtBottomLine-ballB.H)-2){
				ballBstate=t1;
			}		
			break;			
		case t1:
			ballB.Y -= ballIncrementY;
			if(ballB.Y < courtTopLine+2){
				ballBstate=t0;
			}	
			break;		
	}
	
	// erase the ball's previous position
	TFTscreen.fill(0, 0, 0);
	
	if (ballB.oldX != ballB.X || ballB.oldY != ballB.Y) {
		TFTscreen.rect(ballB.oldX, ballB.oldY, ballB.W, ballB.H);
	}
	
	// draw the ball's current position
	TFTscreen.fill(0, 255, 0);  // green
	TFTscreen.rect(ballB.X, ballB.Y, ballB.W, ballB.H);
	
	ballB.oldX = ballB.X;
	ballB.oldY = ballB.Y;
	
}

//---------------------------------------------------------
//******************  ballA_move  *************************

int ballACount=0;
void ballA_move(){
	switch(ballAstate){
		case s0:
			ballA.X += ballIncrementX;
			if(ballA.X > (courtRightLine-ballA.W)-2){
				ballAstate=s1;
			}		
			break;			
		case s1:
			ballA.Y += ballIncrementY;
			if(ballA.Y > (courtBottomLine-ballA.H)-2){
				ballAstate=s2;
			}	
			break;			
		case s2:
			ballA.Y -= ballIncrementY;
			if(ballA.Y < ballA.initY){
				ballACount++;
				ballAstate=s1;
			}
			if(ballACount > 2){
				ballACount=0;
				ballAstate=s3;
			}	
			break;	
		case s3:
			ballA.X -= ballIncrementX;
			if(ballA.X < (courtLeftLine)-2){
				ballAstate=s0;
			}			
			break;						
	}
	
	// erase the ball's previous position
	TFTscreen.fill(0, 0, 0);
	
	if (ballA.oldX != ballA.X || ballA.oldY != ballA.Y) {
		TFTscreen.rect(ballA.oldX, ballA.oldY, ballA.W, ballA.H);
	}
	
	// draw the ball's current position
	TFTscreen.fill(255, 255, 0);  //yellow
	TFTscreen.rect(ballA.X, ballA.Y, ballA.W, ballA.H);
	
	ballA.oldX = ballA.X;
	ballA.oldY = ballA.Y;
	
}

//---------------------------------------------------------


