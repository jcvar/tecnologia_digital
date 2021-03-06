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

// Vertical lines
#define LEFT_LINE 20
#define RIGHT_LINE 140


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

int cx[4]={75, 75, 85, 85};
int cy[4]={20, 30, 30, 20};


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
	static unsigned long prev_millis = 0;
	if(millis() > prev_millis + 200){
		prev_millis = millis();
		bar2();
	}
}


void bar2(){
	static int data_point = 0;
	
	data_point = map(analogRead(A0), 0, 1024, 0, 120);
	TFTscreen.fillRect(20, 59, data_point, 10, COLOR_GREEN);
	TFTscreen.fillRect(data_point, 59, 120 - data_point, 10, COLOR_BLACK);
}


void bar1(){
	static int x = LEFT_LINE + 1;
	static bool flag = true;
	if ((flag && x >= RIGHT_LINE - 5) || (!flag && x <= LEFT_LINE + 1)){
		flag = !flag;
	}
	if (flag) {
		TFTscreen.fillRect(x, 59, 5, 10, COLOR_GREEN);
		x += 5;
	} else {
		TFTscreen.fillRect(x, 59, 5, 10, COLOR_BLACK);
		x -= 5;
	}
}


