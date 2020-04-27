//TFT example

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define ds   9
#define rst  8


// create an instance of the library
TFT TFTscreen = TFT(cs, ds, rst);

// char array to print to the screen
char sensorPrintout[4];

int aval;

void setup() {
	
	// Put this line at the beginning of every sketch that uses the GLCD:
	TFTscreen.begin();
	
	// clear the screen with a black background
	TFTscreen.background(0, 0, 0);
	
	// write the static text to the screen
	// set the font color to magenta
	TFTscreen.stroke(255,0, 255);
	// set the font size
	TFTscreen.setTextSize(2);
	// write the text to the top left corner of the screen
	TFTscreen.text("Sensor 1 Value :\n ", 0, 0);
	// set the font size very large for the loop
	TFTscreen.setTextSize(1);
}

void loop() {
	
/*------------------------------ analog value to display ---------------------*/	
	// Read the value of the sensor on A0
	aval=(analogRead(A0));
	//convert to char array:sensorPrintout
	intToChar(aval);
	// set the font color white
	TFTscreen.stroke(255, 255, 255);
	// print the sensor value
	TFTscreen.text(sensorPrintout, 0, 20);
	// wait for a moment
	delay(250);
	// erase the text you just wrote
	TFTscreen.stroke(0, 0, 0);
	TFTscreen.text(sensorPrintout, 0, 20);
	
	
/*---------------------------now drawing geometric figures: ------------------*/	
	//filled circle color: 1111  1000 0000 0000: red max
	TFTscreen.fillCircle(50,60,5,0xF800);
	//circle color: 0000  0111 1110 0000: green max
	TFTscreen.drawCircle(80,60,10,0x07E0);
	//filled rectangle, color: 0000  0000 0001 1111: blue max
	TFTscreen.fillRect(110, 60, 10,15, 0x001F);
	//rect color (128,128,0) : yellow middle
	TFTscreen.fill(128,128,0);
	TFTscreen.rect(50,90,15,10);
	//rect color (0xF800) : red max
	TFTscreen.fill(0xF800);
	TFTscreen.rect(70,90,15,10);	
	
}

void intToChar (int aval){
	
	int mil,cen,dec,uni;
	
	mil=(aval/1000)%10;
	cen=(aval/100)%10;
	dec=(aval/10)%10;
	uni=aval%10;	
	
	sensorPrintout[0]='0'+mil;
	sensorPrintout[1]='0'+cen;
	sensorPrintout[2]='0'+dec;
	sensorPrintout[3]='0'+uni;
	
	if(mil==0){
		sensorPrintout[0]=' ';
		if(cen==0){
			sensorPrintout[1]=' ';
			if(dec==0)sensorPrintout[2]=' ';
		}
	}
	
}