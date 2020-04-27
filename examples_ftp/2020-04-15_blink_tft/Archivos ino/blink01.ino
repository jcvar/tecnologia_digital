//TFT ejemplo: indicador rojo parpadeando cada segundo 

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define ds   9
#define rst  8

// create an instance of the library
TFT TFTscreen = TFT(cs, ds, rst);

void setup() {
	
	// Put this line at the beginning of every sketch that uses the GLCD:
	TFTscreen.begin();
	// clear the screen with a black background
	TFTscreen.background(0, 0, 0);
	// red off
	TFTscreen.drawCircle(50,60,10,0xF8000);
}

void loop() {
	//filled circle color: 1111  1000 0000 0000: red max
	TFTscreen.fillCircle(50,60,10,0xF800);
	delay(500);
	//erase the filled circle
	TFTscreen.fillCircle(50,60,10,0x0000);
	//draw circle contour	
	TFTscreen.drawCircle(50,60,10,0xF800);
	delay(500);
}

