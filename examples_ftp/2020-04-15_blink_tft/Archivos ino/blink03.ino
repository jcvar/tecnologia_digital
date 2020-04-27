//TFT ejemplo: indicador rojo parpadeando cada segundo 

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define ds   9
#define rst  8

// create an instance of the library
TFT TFTscreen = TFT(cs, ds, rst);

enum redState_t {	s0,s1};  //simulador solo acepta declaraciones globales de tipo enum
redState_t redState=s0;

void setup() {
	
	// Put this line at the beginning of every sketch that uses the GLCD:
	TFTscreen.begin();	
	// clear the screen with a black background
	TFTscreen.background(0, 0, 0);
	turnRedOff();
	
}

void loop() {	
	redBlink();
}


void redBlink(){
	
	long currentMillis=millis();
	static long previousMillis=0;
	switch(redState){
		case s0:	//off
			if ((currentMillis-previousMillis)>=500){
				turnRedOn();
				previousMillis=currentMillis;
				redState=s1;
			}
			break;
		case s1:	//on	
			if ((currentMillis-previousMillis)>=500){
				turnRedOff();
				previousMillis=currentMillis;
				redState=s0;
			}
			break;
	}
	
	
}

void turnRedOn(){
	//filled circle color: 1111  1000 0000 0000: red max
	TFTscreen.fillCircle(50,60,10,0xF800);
}

void turnRedOff(){
	//erase the filled circle
	TFTscreen.fillCircle(50,60,10,0x0000);
	//draw circle contour	
	TFTscreen.drawCircle(50,60,10,0xF800);	
}