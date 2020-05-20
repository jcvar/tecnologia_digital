/*
Tecnologia Digital
Universidad Nacional de Colombia
2020-05-04
*/
#include <TFT.h>
#include <SPI.h>
// Screen definitions and initialization
#define cs   10
#define dc   9
#define rst  8
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

// MILLIS
#define MILLIS_BAR 200
#define MILLIS_BUTTONS 200

// Vertical lines
#define LEFT_LINE 20
#define RIGHT_LINE 140
#define STEP 5

// Button pins
#define BUTTON_PIN_UP 7
#define BUTTON_PIN_DOWN 6

// Court parameters
#define COURT_X 5
#define COURT_Y 10
#define COURT_W 150
#define COURT_H 108

// Court structure
struct court_t {
	int top;
	int bottom;
	int left;
	int right;
};

court_t court;// court instance

int bar_val = 0;
bool button_pressed_up = false;
bool button_pressed_down = false;

void setup() {
	TFTscreen.begin(); // initialize the display
	TFTscreen.background(0, 0, 0); // black background
	
	// Court initialization
	court.top = COURT_Y;
	court.bottom = COURT_Y + COURT_H;
	court.left = COURT_X;
	court.right = COURT_X + COURT_W;
	// Draw court
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	
	// Draw green lines
	TFTscreen.drawFastVLine(LEFT_LINE, court.top, court.bottom - court.top, COLOR_GREEN);
	TFTscreen.drawFastVLine(RIGHT_LINE, court.top, court.bottom - court.top, COLOR_GREEN);
	delay(1000);
}

void loop() {
	static unsigned long prev_millis_bar = 0;
	static unsigned long prev_millis_buttons = 0;
	
	if(millis() > prev_millis_bar + MILLIS_BAR){
		prev_millis_bar = millis();
		bar();
	}
	
	if(millis() > prev_millis_buttons + MILLIS_BUTTONS){
		prev_millis_buttons = millis();
		upPressed();
		downPressed();
		
	}
}

void bar(){
	static int old_val = bar_val;
	if (bar_val != old_val) {
		old_val = bar_val;
		TFTscreen.fillRect(LEFT_LINE, 59, bar_val, 10, COLOR_GREEN);
		TFTscreen.fillRect(LEFT_LINE + bar_val + 1, 59, RIGHT_LINE - LEFT_LINE - bar_val - 1, 10, COLOR_BLACK);
	}
	/*
	if(button_pressed_up){
		button_pressed_up = false;
		if (bar_val + STEP <= RIGHT_LINE-LEFT_LINE){
			bar_val += STEP;
		}
	}else if(button_pressed_down){
		button_pressed_down = false;
		if (bar_val - STEP >= 0){
			bar_val -= STEP;
		}
	}
	TFTscreen.fillRect(LEFT_LINE, 59, bar_val, 10, COLOR_GREEN);
	TFTscreen.fillRect(LEFT_LINE + bar_val + 1, 59, RIGHT_LINE - LEFT_LINE - bar_val - 1, 10, COLOR_BLACK);
	*/
}




void upPressed(){
	if (digitalRead(BUTTON_PIN_UP) == HIGH && bar_val + STEP <= RIGHT_LINE-LEFT_LINE) {
		bar_val += STEP;
	}
	/*
	static bool button_state = false;
	if(!button_state && digitalRead(BUTTON_PIN_UP) == HIGH){
		button_pressed_up = true;
		button_state = true;
	} else if (button_state && digitalRead(BUTTON_PIN_UP) == LOW){
		button_state = false;
	}
	*/
}

void downPressed(){
	if (digitalRead(BUTTON_PIN_DOWN) == HIGH && (bar_val - STEP >= 0)) {
		bar_val -= STEP;
	}
	/*
	static bool button_state = false;
	if(!button_state && digitalRead(BUTTON_PIN_DOWN) == HIGH){
		button_pressed_down = true;
		button_state = true;
	} else if (button_state && digitalRead(BUTTON_PIN_DOWN) == LOW){
		button_state = false;
	}
	*/
}