#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

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

TFT tft = TFT(cs, dc, rst);

#define MENU1 0
#define MENU2 1
#define MENU3 2
#define MENU4 3
#define NONE  4

const int length_MENU1 =  250;
const int length_MENU2 =  500;
const int length_MENU3 = 1000;
const int length_MENU4 = 2000;

int buttonEnterPin=6;
int buttonNextPin=7;

enum buttonEnter_t {	lowValue,highValue};
buttonEnter_t buttonEnter=lowValue;
buttonEnter_t buttonNext=lowValue;

enum state_t {	s0,s1,s2};
state_t state=s0;

enum loop_t {	loopS0,loopS1,loopS2};
loop_t loopState=loopS0;

uint8_t menuSel=MENU1;
bool enterButtonPressed=false;
bool nextButtonPressed=false;
bool mnIdle=false;
bool flagMenuFinish=false;

void setup(void) {
	tft.begin();	
	tft.fillScreen(COLOR_BLACK);
	pinMode(buttonEnterPin,INPUT_PULLUP);	
}

void loop() {	
	switch(loopState){
		case loopS0:
			nextPress();
			menuSel = readButton();
			mainMenu();
			if(enterButtonPressed){
				tft.fillScreen(COLOR_BLACK);
				loopState=loopS1;
			}
			break;
		case loopS1:
			tft.fillScreen(COLOR_BLACK);
			loopState=loopS2;
		case loopS2:
			mnIdle=true;
			break;
	}
	
	dispatch();
	delay(10);
}

void dispatch(){
	enterPress();
	switch(state){
		case s0:
			if(enterButtonPressed){
				if(mnIdle){
					enterButtonPressed=false;
					state=s1;
				}			
			}	
			break;				
		case s1:
			tft.fillScreen(COLOR_BLACK);
			state=s2;
		case s2:
			if(menuSel==MENU1) menu1Fcn();
			if(menuSel==MENU2) menu2Fcn();
			if(menuSel==MENU3) menu3Fcn();
			if(menuSel==MENU4) menu4Fcn();
			if(flagMenuFinish){
				flagMenuFinish=false;
				mnIdle=false;
				state=s0;
			}
			break;
	}
}

void menu1Fcn(){
	tft.setCursor(0, 10);
	tft.setTextColor(COLOR_GREEN);
	tft.setTextSize(2);
	tft.println("Seleccion 1");
	
	static unsigned long blink_millis = millis();
	static bool blink = true;
	if(blink){
		if((millis() - blink_millis) >= length_MENU1){
			blink_millis = millis();
			blink = false;
			tft.fillCircle(80,64,20,COLOR_BLACK);
		}
	} else {
		if((millis() - blink_millis) >= length_MENU1){
			blink_millis = millis();
			blink = true;
			tft.fillCircle(80,64,20,COLOR_GREEN);
		}
	}
	
	enterPress();
	if(enterButtonPressed){
		tft.fillScreen(COLOR_BLACK);
		enterButtonPressed=false;
		flagMenuFinish=true;
		loopState=loopS0;
	}	
}

void menu2Fcn(){
	tft.setCursor(0, 10);
	tft.setTextColor(COLOR_YELLOW);
	tft.setTextSize(2);
	tft.println("Seleccion 2");
	
	static unsigned long blink_millis = millis();
	static bool blink = true;
	if(blink){
		if((millis() - blink_millis) >= length_MENU2){
			blink_millis = millis();
			blink = false;
			tft.fillRect(40,42,80,40,COLOR_BLACK);
		}
	} else {
		if((millis() - blink_millis) >= length_MENU2){
			blink_millis = millis();
			blink = true;
			tft.fillRect(40,42,80,40,COLOR_YELLOW);
		}
	}
	
	enterPress();
	if(enterButtonPressed){
		tft.fillScreen(COLOR_BLACK);
		enterButtonPressed=false;
		flagMenuFinish=true;
		loopState=loopS0;
	}	
}

void menu3Fcn(){
	tft.setCursor(0, 10);
	tft.setTextColor(COLOR_MAGENTA);
	tft.setTextSize(2);
	tft.println("Seleccion 3");	
	
	static unsigned long blink_millis = millis();
	static bool blink = true;
	if(blink){
		if((millis() - blink_millis) >= length_MENU3){
			blink_millis = millis();
			blink = false;
			tft.fillTriangle(60, 40, 40, 90, 100, 120, COLOR_BLACK);
		}
	} else {
		if((millis() - blink_millis) >= length_MENU3){
			blink_millis = millis();
			blink = true;
			tft.fillTriangle(60, 40, 40, 90, 100, 120, COLOR_MAGENTA);
		}
	}
	
	
	enterPress();
	if(enterButtonPressed){
		tft.fillScreen(COLOR_BLACK);
		enterButtonPressed=false;
		flagMenuFinish=true;
		loopState=loopS0;
	}	
}

void menu4Fcn(){
	tft.setCursor(0, 10);
	tft.setTextColor(COLOR_BLUE);
	tft.setTextSize(2);
	tft.println("Seleccion 4");
	
	static unsigned long blink_millis = millis();
	static bool blink = true;
	if(blink){
		if((millis() - blink_millis) >= length_MENU4){
			blink_millis = millis();
			blink = false;
			tft.fillRect(50,60,60,20,COLOR_BLACK);
			tft.fillRect(70,40,20,60,COLOR_BLACK);
		}
	} else {
		if((millis() - blink_millis) >= length_MENU4){
			blink_millis = millis();
			blink = true;
			tft.fillRect(50,60,60,20,COLOR_BLUE);
			tft.fillRect(70,40,20,60,COLOR_BLUE);
		}
	}
	
	enterPress();
	if(enterButtonPressed){
		tft.fillScreen(COLOR_BLACK);
		enterButtonPressed=false;
		flagMenuFinish=true;
		loopState=loopS0;
	}	
}

void mainMenu()
{
	tft.setCursor(0, 10);	
	
	if(menuSel==MENU1)tft.setTextColor(COLOR_RED);
	else tft.setTextColor(COLOR_WHITE);
	tft.setTextSize(2);
	tft.println("--- MENU 1 -");
	
	if(menuSel==1)tft.setTextColor(COLOR_RED);
	else tft.setTextColor(COLOR_WHITE);		
	tft.setTextSize(2);
	tft.println("--- MENU 2 -");
	
	if(menuSel==2)tft.setTextColor(COLOR_RED);
	else tft.setTextColor(COLOR_WHITE);	
	tft.setTextSize(2);	
	tft.println("--- MENU 3 -");
	
	if(menuSel==3)tft.setTextColor(COLOR_RED);
	else tft.setTextColor(COLOR_WHITE);
	tft.setTextSize(2);
	tft.println("--- MENU 4 -");
	
	tft.setCursor(0, 100);	
	tft.setTextColor(COLOR_WHITE);
	tft.setTextSize(1);
	tft.println("Press enter to select\nPress next to navigate");	
}

void enterPress(){
	switch(buttonEnter) {
		case lowValue:
			if(digitalRead(buttonEnterPin)==HIGH){
				enterButtonPressed=true;
				buttonEnter=highValue;
			}
			break;
		case highValue:
			if(digitalRead(buttonEnterPin)==LOW){
				buttonEnter=lowValue;
			}
			break;
	}	
}


uint8_t readButton(void) {
	if(nextButtonPressed){
		nextButtonPressed = false;
		switch(menuSel){
			case MENU1:
				return MENU2;
				break;
			case MENU2:
				return MENU3;
				break;
			case MENU3:
				return MENU4;
				break;
			case MENU4:
				return MENU1;
				break;
		}
	}
	else {
		return menuSel;
	}
}



void nextPress(){
	switch(buttonNext) {
		case lowValue:
			if(digitalRead(buttonNextPin)==HIGH){
				nextButtonPressed=true;
				buttonNext=highValue;
			}
			break;
		case highValue:
			if(digitalRead(buttonNextPin)==LOW){
				buttonNext=lowValue;
			}
			break;
	}	
}
