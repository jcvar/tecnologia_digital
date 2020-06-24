/*
Tarea 16
Clock

Juan Camilo Vargas Q.
Sergio Alejandro Vargas Q.

Tecnologia Digital
Universidad Nacional de Colombia
2020-06-24
*/
#include "../common.h"

#define DIGIT_W 32
#define DIGIT_H 40

typedef unsigned short hu // shorthand

TFT TFTscreen = TFT(CS, DC, RST);
court_t court;

hu clock_hour = 0;
hu clock_minute = 0;
unsigned long curMillis;

typedef enum {hour_units, hour_tens, minute_units, minute_tens} digit_t;

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
}

void loop() {
	update_time(&clock_hour, &clock_minute);
	draw_time(clock_hour, clock_minute);
}

// update_time: Borrow hour and minute, increase 1 minute
void update_time(hu *hour, hu *minute) {
	*minute++;          // Update minute
	if (*minute == 60) {
		*minute = 0;
		*hour++;          // Update hour
		if (*hour == 24) {
			*hour = 0;
		}
	}
}


// draw_time: Copies hour and minute
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

void draw_digit(hu digit, digit_t dt) {
	int x;
	switch(dt) {
		case minute_units:
			x = offset + DIGIT_W*4;
			break;
		case minute_tens:
			x = 
			break;
		case hour_units:
			x = 
			break;
		case hour_tens:
			x = 
			break;
	}
}

