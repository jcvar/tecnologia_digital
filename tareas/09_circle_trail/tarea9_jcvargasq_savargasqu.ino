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

int cx[8]={10,30,50,70,90,110,130,150};
int cy[6]={14,34,54,74,94,114};


enum state_t {	s0,s1,s2,s3};
state_t state=s0;

void setup() {
	// initialize the display
	tft.begin();
	// black background
	tft.background(0, 0, 0);
	
	for(int i=0;i<8;i++){
		tft.fillCircle(cx[i],cy[0],10,COLOR_BLUE);
		tft.fillCircle(cx[i],cy[5],10,COLOR_BLUE);		
	}
	
	for(int i=0;i<6;i++){
		tft.fillCircle(cx[0],cy[i],10,COLOR_BLUE);	
		tft.fillCircle(cx[7],cy[i],10,COLOR_BLUE);		
	}
	tft.fillCircle(cx[0],cy[0],10,COLOR_YELLOW);	
}

int x = 0;
int y = 0;

void loop() {
	static unsigned long prevMillis = 0;
	
	if (millis() >= prevMillis + 100) {
		prevMillis = millis();
		fsm();
	}
}


void fsm() {
	switch(state){
		case s0: // GO RIGHT
			// Erase prev
			if(x==0)
				tft.fillCircle(cx[x],cy[y+1],10,COLOR_BLUE);
			else	
				tft.fillCircle(cx[x-1],cy[y],10,COLOR_BLUE);
			// Draw next
			tft.fillCircle(cx[x+1],cy[y],10,COLOR_YELLOW);
			
			x++;
			if(x==7)
				state = s1;
			break;
			
		case s1: // GO DOWN
			// Erase prev
			if(y==0)
				tft.fillCircle(cx[x-1],cy[y],10,COLOR_BLUE);
			else
				tft.fillCircle(cx[x],cy[y-1],10,COLOR_BLUE);
			// Draw next
			tft.fillCircle(cx[x],cy[y+1],10,COLOR_YELLOW);
			
			y++;
			if(y==5)
				state = s2;
			break;
			
		case s2: // GO LEFT
			// Erase prev
			if(x==7)
				tft.fillCircle(cx[7],cy[y-1],10,COLOR_BLUE);
			else
				tft.fillCircle(cx[x+1],cy[y],10,COLOR_BLUE);
			// Draw next
			tft.fillCircle(cx[x-1],cy[y],10,COLOR_YELLOW);
			
			x--;
			if(x==0)
				state = s3;
			
			break;
			
		case s3: // GO UP
			// Erase prev
			if(y==5)
				tft.fillCircle(cx[x+1],cy[y],10,COLOR_BLUE);
			else
				tft.fillCircle(cx[x],cy[y+1],10,COLOR_BLUE);
			// Draw next
			tft.fillCircle(cx[x],cy[y-1],10,COLOR_YELLOW);
			
			y--;
			if(y==0)
				state = s0;
			break;
	}
}