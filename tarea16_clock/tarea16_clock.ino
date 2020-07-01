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
#include "clock.h"
#include "clock_draw.h"
#include <JC_Button.h>
#include <TimerOne.h>

#define MILLIS_LOOP 200



court_t court = {COURT_Y, COURT_Y + COURT_H, COURT_X, COURT_X + COURT_W};

Button mode_button(4); // Cycle through state_t modes
Button next_button(5); // Cycle through time values in each state_t

// Sketch globals
state_t clk_mode = set_normal;
time_t clk = {0, 0, 0, true};
time_t alarm = {0, 0, 0, false};


/* DOCS */
// TimerOne interrupt serve routine
void timer1_isr();
// Main clock logic
void update_time();//(unsigned*, unsigned*);


void setup() {
	TFTscreen.begin(); // For simulator
	//TFTscreen.begin(INITR_BLACKTAB); // INITR_BLACKTAB for BGR
	TFTscreen.background(COLOR_BLACK);
	TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);
	
	// External inits
	Timer1.initialize(500000);//timing for 500ms
	Timer1.attachInterrupt(timer1_isr);//declare the interrupt serve routine:TimingISR
	mode_button.begin();
	next_button.begin();
	
	force_draw(clk);
} // END SETUP

void loop() {
	static unsigned long loop_millis = 0;
	
	mode_button.read();
	next_button.read();
	
	if (millis() - loop_millis > MILLIS_LOOP) {
		loop_millis = millis();
		
		if (mode_button.wasPressed()) {
			update_mode();
			draw_mode(clk_mode);
		}
		if (next_button.wasPressed()) {
			update_next(clk_mode);
		}
		
	}
	
} // END LOOP

void timer1_isr() {
	clk.active = !clk.active;
	if (clk.active) {
		update_time();
		if (clk_mode != set_alarm_active &&
			clk_mode != set_alarm_hour &&
		clk_mode != set_alarm_minute) {
			draw_time(clk);
		}
	}
}


void update_mode() {
	switch (clk_mode) {
		case set_normal:
			clk_mode = set_alarm_active;
			break;
		case set_alarm_active:
			clk_mode = set_alarm_hour;
			break;
		case set_alarm_hour:
			clk_mode = set_alarm_minute;
			break;
		case set_alarm_minute:
			clk_mode = set_hour;
			break;
		case set_hour:
			clk_mode = set_minute;
			break;
		case set_minute:
			clk_mode = set_sec;
			break;
		case set_sec:
			clk_mode = set_normal;
			break;
		default:
			clk_mode = set_normal;
			break;
			
	}
}

void update_next(state_t mode) {
	switch (mode) {
		case set_normal:
			// DO NOTHING
			break;
		case set_alarm_active:
			alarm.active = !alarm.active;
			break;
		case set_alarm_hour:
			alarm.hour = (alarm.hour + 1) % 24;
			break;
		case set_alarm_minute:
			alarm.minute = (alarm.minute + 1) % 60;
			break;
		case set_hour:
			clk.hour = (clk.hour + 1) % 24;
			break;
		case set_minute:
			clk.minute = (clk.minute + 1) % 60;
			break;
		case set_sec:
			clk.second = 0;
			break;
	}
}


void update_time() {
	clk.second++;
	if (clk.second == 60) {
		clk.second = 0;
		clk.minute++;					// Update minute
		if (clk.minute == 60) {
			clk.minute = 0;
			clk.hour++;					// Update hour
			if (clk.hour == 24) {
				clk.hour = 0;
			}
		}
	}
}
