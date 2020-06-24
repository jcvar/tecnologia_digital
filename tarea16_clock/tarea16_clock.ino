/*
Tarea 16
Clock

Juan Camilo Vargas Q.
Sergio Alejandro Vargas Q.

Tecnologia Digital
Universidad Nacional de Colombia
2020-06-24
*/
#include "common.h"
#include "digits_small.h"

#define MILLIS_MINUTE  100 // 60000
#define DIGIT_W 16
#define DIGIT_H 24

#define DIGIT_OFFSET 4
#define MEM_OFFSET 384
// FIXME
#define INIT_X 30
#define INIT_Y 51

typedef unsigned short hu; // shorthand

TFT TFTscreen = TFT(CS, DC, RST);
court_t court;

hu hour = 0;
hu minute = 0;

typedef enum {	hour_units, hour_tens, minute_units, minute_tens} digit_t;

void update_time();//(hu*, hu*);
void draw_time(hu, hu);
void draw_digit(hu, digit_t);
int get_digit_pos(digit_t);

void setup() {
	// Initialize screen
	TFTscreen.begin();
	TFTscreen.background(COLOR_BLACK);
	// Court initialization
	//court.top = COURT_Y;
	//court.bottom = COURT_Y + COURT_H;
	//court.left = COURT_X;
	//court.right = COURT_X + COURT_W;
	// Draw court
	//TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	draw_time(0, 0);
	delay(100);
}

void loop() {
	static unsigned long clock_millis = 0;
	if (millis() - clock_millis > MILLIS_MINUTE) {
		clock_millis = millis();
		update_time();//(hour, minute);
		draw_time(hour, minute);
	}
}

void update_time(){	//(hu *hour, hu *minute) {
	minute++;					// Update minute
	if (minute == 60) {
		minute = 0;
		hour++;					// Update hour
		if (hour == 24) {
			hour = 0;
		}
	}
}

void draw_time(hu hour, hu minute) {
	draw_digit(minute % 10, minute_units);
	if (minute % 10 == 0) {
		draw_digit(minute/10, minute_tens);
		if (minute / 10 == 0) {
			draw_digit(hour % 10, hour_units);
			if (hour % 10 == 0) {
				draw_digit(hour / 10, hour_tens);
			}
		}
	}
}

void draw_digit(hu digit, digit_t digt) {
	int posX = get_digit_pos(digt);
	int offset = digit * MEM_OFFSET;
	for (int row = 0; row < DIGIT_H; row++) {
		for (int col = 0; col < DIGIT_W; col++) {
			word p = pgm_read_word(digits + offset + (row*DIGIT_W + col));
			TFTscreen.drawPixel(col + posX, row + INIT_Y, p);
		}
	}
}

int get_digit_pos(digit_t dt) {
	int x = INIT_X;
	switch(dt) {
		case minute_units:
			x += (DIGIT_W+DIGIT_OFFSET)*4;
			break;
		case minute_tens:
			x += (DIGIT_W+DIGIT_OFFSET)*3;
			break;
		case hour_units:
			x += (DIGIT_W+DIGIT_OFFSET);
			break;
		case hour_tens:
			break;
	}
	return x;
}
