//TFT ejemplo: seleccion de color enviada serialmente

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define ds   9
#define rst  8

// create an instance of the library
TFT tft = TFT(cs, ds, rst);

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

char ch='\0'; //received color
int posX=80,posY=64,radio=20; //circle specs

enum state_t {	s0,s1,s2};
state_t state=s0;

void setup() {
	Serial.begin(9600);
	// Put this line at the beginning of every sketch that uses the GLCD:
	tft.begin();
	// clear the screen with a black background
	tft.background(0, 0, 0);
	// draw circle contour
	tft.drawCircle(posX,posY,radio,COLOR_WHITE);
}

void loop() {
	
	switch(state){
		case s0:
			//print send color
			tft.setCursor(10, 10);
			tft.setTextColor(COLOR_WHITE);
			tft.setTextSize(1);	
			tft.println("Send Color: ");	
			//repeat while serial reception different from 'G' or 'R'		
			do{
				while(Serial.available() == 0){	} //wait for serial data
				ch = Serial.read();	
				//check wrong selection
				if(ch != 'G' && ch != 'R') {	
					tft.setCursor(85, 10);
					tft.setTextColor(COLOR_RED);
					tft.setTextSize(1);	
					tft.println("try again... ");		
				}
			}while (ch != 'G' && ch != 'R');	
			//goto print selected color	
			state=s1;
			break;	
		case s1:
			//delete try again
			tft.setCursor(85, 10);
			tft.setTextColor(COLOR_BLACK);
			tft.setTextSize(1);	
			tft.println("try again... ");	
			//print selected color
			tft.setCursor(90,10);			
			tft.setTextSize(1);
			tft.setTextColor(COLOR_WHITE);
			tft.print(ch);
			delay(100);
			//goto blinking
			state=s2;
			break;
		case s2:
			//filled circle with selected color
			if (ch=='G') tft.fillCircle(posX,posY,radio,COLOR_GREEN);
			else if (ch=='R') tft.fillCircle(posX,posY,radio,COLOR_RED);
			delay(500);
			//erase the filled circle
			tft.fillCircle(posX,posY,radio,0x0000);
			//draw circle contour	
			tft.drawCircle(posX,posY,radio,COLOR_WHITE);
			delay(500);						
	}	
}

