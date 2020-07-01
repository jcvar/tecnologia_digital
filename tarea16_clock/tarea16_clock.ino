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

// Drawing defines
#define INIT_X 10
#define INIT_Y 51
#define DIGIT_W 16
#define DIGIT_H 24
#define DIGIT_OFFSET 4
#define MEM_OFFSET 384 // W*H

court_t court = {COURT_Y, COURT_Y + COURT_H, COURT_X, COURT_X + COURT_W};
TFT TFTscreen = TFT(CS, DC, RST);
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
// Redraw time values if changed
void draw_time(time_t);
// Draw all digits
void force_draw(time_t);
// Access selected digit image and draw at digit_t
void draw_digit(unsigned, digit_t);
// Get the x-axis coordinate for a given digit
int get_digit_pos(digit_t);

void setup() {
  TFTscreen.begin(INITR_BLACKTAB); // INITR_BLACKTAB for BGR
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
    }
    if (next_button.wasPressed()) {
      update_next(clk_mode);
    }

  }

} // END LOOP

void timer1_isr() {
  clk.half = !clk.half;
  if (clk.half) {
    update_time();
    draw_time(clk);
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
      clk_mode = set_alarm_min;
      break;
    case set_alarm_min:
      clk_mode = set_hour;
      break;
    case set_hour:
      clk_mode = set_min;
      break;
    case set_min:
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
      alarm.half = !alarm.half;
      break;
    case set_alarm_hour:
      alarm.hour = (alarm.hour + 1) % 24;
      break;
    case set_alarm_min:
      alarm.minute = (alarm.minute + 1) % 60;
      break;
    case set_hour:
      clk.hour = (clk.hour + 1) % 24;
      break;
    case set_min:
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
