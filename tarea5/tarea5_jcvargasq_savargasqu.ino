// Missing title header

// Display library and config
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

#define cs  10
#define dc   9
#define rst  8
TFT tft = TFT(cs, dc, rst);

// Some ready-made 16-bit ('565') color settings:
const word COLOR_BLACK   = 0x0000;
const word COLOR_WHITE   = 0xFFFF;
const word COLOR_RED     = 0xF800;
const word COLOR_GREEN   = 0x07E0;
const word COLOR_BLUE    = 0x001F;
const word COLOR_YELLOW  = 0xFFE0;
const word COLOR_MAGENTA = 0xF81F;
const word COLOR_CYAN    = 0x07FF;
const word COLOR_ORANGE  = 0xFC00;
word colorSel = COLOR_BLACK;

// Menu selector definitions
#define NONE  0
#define MENU1 1
#define MENU2 2
#define MENU3 3
#define MENU4 4
int menuSel = MENU1;

// State machine definitions
enum state_t {	s0, s1, s2};
state_t state = s0;
enum loop_t {	loopS0, loopS1, loopS2};
loop_t loopState = loopS0;
// Sketch flow variables
bool mnIdle = false;
bool flagMenuFinish = false;

// Blinking length of figures (half cycle)
const int length_MENU1 =  250;
const int length_MENU2 =  500;
const int length_MENU3 = 1000;
const int length_MENU4 = 2000;

// Button variables and config
enum button_t {	lowValue, highValue};
button_t buttonEnter = lowValue;
button_t buttonNext  = lowValue;
const int buttonEnterPin = 6;
const int buttonNextPin  = 7;
bool enterButtonPressed = false;
bool nextButtonPressed  = false;

void setup() {
	Serial.begin(9600);
	tft.begin();
	tft.fillScreen(COLOR_BLACK);
	pinMode(buttonEnterPin,INPUT_PULLUP);
	pinMode(buttonNextPin,INPUT_PULLUP);
}

void loop() {
	// Main menu state machine
	switch(loopState){
		case loopS0: // Main menu
			// Check for button press
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
			// Draw selected screen
			if(menuSel==MENU1) menu_selection(MENU1, length_MENU1);
			if(menuSel==MENU2) menu_selection(MENU2, length_MENU2);
			if(menuSel==MENU3) menu_selection(MENU3, length_MENU3);
			if(menuSel==MENU4) menu_selection(MENU4, length_MENU4);
			
			enterPress();
			if(enterButtonPressed) {
				tft.fillScreen(COLOR_BLACK);
				enterButtonPressed=false;
				flagMenuFinish=true;
				loopState=loopS0;
			}
			if(flagMenuFinish) {
				flagMenuFinish=false;
				mnIdle=false;
				state=s0;
			}
			break;
	}
}

// Draw and refresh all screen elements
void menu_selection(int selection, int duration) {
	static char charColor = '\0';
	// Write label
	tft.setCursor(0, 120);
	tft.setTextSize(1);
	tft.setTextColor(COLOR_WHITE);
	tft.println("Selected color is:");
	// Erase written color
	tft.setCursor(120, 120);
	tft.setTextColor(COLOR_BLACK);
	tft.print(charColor);
	// Update color
	charColor = readColor();
	colorSel = colorPicker(charColor);
	// Write updated color 
	tft.setCursor(120, 120);
	tft.setTextColor(colorSel);
	tft.print(charColor);
	// Write title of the selected screen
	tft.setCursor(0, 10);
	tft.setTextColor(colorSel);
	tft.setTextSize(2);
	tft.print("Seleccion");// sprintf("Seleccion %d", selection));
	tft.setCursor(120, 10);
	tft.print(selection);
	
	// Check for blink state
	static unsigned long blink_millis = millis();
	static bool blink = true;
	if(blink){
		if((millis() - blink_millis) >= duration){
			blink_millis = millis();
			blink = false;
			draw_selection(selection, COLOR_BLACK);
		}
	} else {
		if((millis() - blink_millis) >= duration){
			blink_millis = millis();
			blink = true;
			draw_selection(selection, colorSel);
		}
	}
	
}

// Draw figure of selected screen
void draw_selection(int selection, word color) {
	switch (selection){
		case MENU1:
			tft.fillCircle(80, 64, 20, color);
			break;
		case MENU2:
			tft.fillRect(40, 42, 80, 40, color);
			break;
		case MENU3:
			tft.fillTriangle(60, 40, 40, 90, 100, 120, color);
			break;
		case MENU4:
			tft.fillRect(50, 60, 60, 20, color);
			tft.fillRect(70, 40, 20, 60, color);
			break;
	}
}

void mainMenu() {
	tft.setCursor(0, 10);
	
	if(menuSel==MENU1)tft.setTextColor(COLOR_RED);
	else tft.setTextColor(COLOR_WHITE);
	tft.setTextSize(2);
	tft.println(" - MENU 1 -");
	
	if(menuSel==MENU2)tft.setTextColor(COLOR_RED);
	else tft.setTextColor(COLOR_WHITE);
	tft.setTextSize(2);
	tft.println(" - MENU 2 -");
	
	if(menuSel==MENU3)tft.setTextColor(COLOR_RED);
	else tft.setTextColor(COLOR_WHITE);
	tft.setTextSize(2);
	tft.println(" - MENU 3 -");
	
	if(menuSel==MENU4)tft.setTextColor(COLOR_RED);
	else tft.setTextColor(COLOR_WHITE);
	tft.setTextSize(2);
	tft.println(" - MENU 4 -");
	
	tft.setCursor(0, 100);
	tft.setTextColor(COLOR_WHITE);
	tft.setTextSize(1);
	tft.println("Press ENTER to select\nPress NEXT to navigate");
}

void buttonPress(button_t *buttonState, bool *buttonFlag, int buttonPin){
	switch(*buttonState){
		case lowValue:
			if(digitalRead(buttonPin)==HIGH){
				*buttonFlag = true;
				*buttonState = highValue;
			}
			break;
		case highValue:
			if(digitalRead(buttonPin)==LOW){
				*buttonState = lowValue;
			}
			break;
	}
	
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

int readButton() {
	if(nextButtonPressed) {
		nextButtonPressed = false;
		switch(menuSel){
			case MENU1:
				return MENU2;
			case MENU2:
				return MENU3;
			case MENU3:
				return MENU4;
			case MENU4:
				return MENU1;
		}
	}
	else {
		return menuSel;
	}
}

char readColor(){
	static char c = '\0';
	if(Serial.available() > 0){
		c = Serial.read();
	}
	return c;
}



word colorPicker(char c){
	switch(c){
		case 'R':
			return COLOR_RED;
		case 'G':
			return COLOR_GREEN;
		case 'B':
			return COLOR_BLUE;
		case 'C':
			return COLOR_CYAN;
		case 'M':
			return COLOR_MAGENTA;
		case 'Y':
			return COLOR_YELLOW;
		case 'W':
			return COLOR_WHITE;
		case 'O':
			return COLOR_ORANGE;
	}
	return COLOR_BLACK;
}
