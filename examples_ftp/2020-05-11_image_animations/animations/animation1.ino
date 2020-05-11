/*
	circles animation
*/

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

#include <pgmspace.h>

#include <avr/pgmspace.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

TFT tft = TFT(cs, dc, rst);

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

int cx[8]={10,30,50,70,90,110,130,150};  //circle centers

enum state_t {	s0,s1,s2};
state_t state=s0;


void setup() {
	// initialize the display
	tft.begin();
	// black background
	tft.background(0, 0, 0);
	
	for(int i=0;i<8;i++){
		tft.fillCircle(cx[i],64,10,COLOR_BLUE);		
	}	
}


void loop() {
	static int i=0;
	
	switch(state){
		case s0:												// init
			tft.fillCircle(cx[0],64,10,COLOR_YELLOW);
			tft.fillCircle(cx[1],64,10,COLOR_YELLOW);
			i=1;
			state=s1;
			break;
			
		case s1:	
			if(i<6){	// forward steps
				tft.fillCircle(cx[i],64,10,COLOR_YELLOW);     
				tft.fillCircle(cx[i+1],64,10,COLOR_YELLOW);
				tft.fillCircle(cx[i-1],64,10,COLOR_BLUE);
				i++;
			}else{  	// last forward step 
				tft.fillCircle(cx[i],64,10,COLOR_YELLOW);       
				tft.fillCircle(cx[i+1],64,10,COLOR_YELLOW);
				tft.fillCircle(cx[i-1],64,10,COLOR_BLUE);		// go reverse	
				i=5; state=s2;
			}
			break;
		case s2:												
			if(i>0){	// reverse steps
				tft.fillCircle(cx[i],64,10,COLOR_YELLOW);
				tft.fillCircle(cx[i+1],64,10,COLOR_YELLOW);
				tft.fillCircle(cx[i+2],64,10,COLOR_BLUE);
				i--;
			}else{	    // last reverse step 
				tft.fillCircle(cx[i],64,10,COLOR_YELLOW);
				tft.fillCircle(cx[i+1],64,10,COLOR_YELLOW);
				tft.fillCircle(cx[i+2],64,10,COLOR_BLUE);				
				i=1; state=s1;                                  // go forward
			}
			break;
	}
	
	delay(500);	
}

