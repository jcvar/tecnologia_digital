/*
	bar
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

// court parameters
int courtX=5,courtY=10,courtW=150,courtH=108;
int courtTopLine=0,courtBottomLine=0,courtLeftLine=0,courtRightLine=0;

//analog variables
int aValue=0,old_aValue=0;

//time variables
unsigned long previousMillisBar=0,currentMillis=0;

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
}

void loop() {
	TFTscreen.drawRect(5, 10, 150, 108,0xF800);	
	
	TFTscreen.drawFastVLine(20, 12, 104,COLOR_GREEN);
	TFTscreen.drawFastVLine(140, 12, 104,COLOR_GREEN);
	
	bar();
}


void bar(){
	currentMillis=millis();
	if(currentMillis-previousMillisBar>200){
		previousMillisBar=currentMillis;
		aValue = map(analogRead(A0), 0, 1023,0,120);
		if(aValue >= old_aValue){
			old_aValue=aValue;
			TFTscreen.fillRect(20, 59, aValue, 10, COLOR_GREEN);
		}else{
			old_aValue=aValue;
			TFTscreen.fillRect(20+aValue, 59, 120-aValue, 10, COLOR_BLACK);	
		}
	}	
}








