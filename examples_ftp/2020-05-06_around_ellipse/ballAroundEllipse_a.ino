/*
	moving square around a ellipse
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

// ball parameters
int ballX=50, ballY=0, oldBallX=0, oldBallY=0;
int ballW=5,ballH=5;
float fballY=0,fballX=50;
//coordenadas del centro
float cx=80,cy=64;

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
	
	drawEllipse(80,64,50,30,COLOR_CYAN);
	
	delay(1000);	
}

void loop() {
	TFTscreen.drawRect(5, 10, 150, 108,0xF800);
	moveBall();
	drawEllipse(80,64,50,30,COLOR_CYAN);
	delay(20);
}

//**************************   move ball   **********************
void moveBall(){	
	static float theta=0; //radianes
	static int alpha=0;   //degrees
	
	fballX=50*cos(theta)+cx;
	fballY=cy-30*sin(theta);  //reverse Y coordinates
	
	ballX=fballX;
	ballY=fballY;
	
	if(alpha<360) alpha+=1;
	else alpha=0;
	
	theta=PI*alpha/180;
	
	// erase the ball's previous position
	TFTscreen.fill(0, 0, 0);
	
	if (oldBallX != ballX || oldBallY != ballY) {
		TFTscreen.rect(oldBallX, oldBallY, ballW, ballH);
	}
	
	// draw the ball's current position
	TFTscreen.fill(255,255, 0);
	TFTscreen.rect(ballX, ballY, ballW, ballH);
	
	oldBallX = ballX;
	oldBallY = ballY;
}

//************************** draw ellipse ********************
void drawEllipse(int x,int y,int u,int v,word thecolor){	
	float theta=0; //radianes
	int alpha=0;   //degrees
	for(alpha=0;alpha<=360;alpha++){
		fballX=u*cos(theta)+x;
		fballY=y-v*sin(theta);  //reverse Y coordinates
		
		ballX=fballX;
		ballY=fballY;
		
		if(alpha<360) alpha+=1;
		else alpha=0;
		
		theta=PI*alpha/180;
		
		TFTscreen.drawPixel(ballX,ballY,thecolor);
	}	
}

//************************************************************

