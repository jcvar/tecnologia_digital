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

// Enum for global clock states
typedef enum {
  set_normal,
  set_alarm_active,
  set_alarm_hour,
  set_alarm_min,
  set_hour,
  set_min,
  set_sec,
} state_t;

// Enum to select digit position
typedef enum {
  hour_tens,
  hour_units,
  minute_tens,
  minute_units,
  second_tens,
  second_units
} digit_t;


court_t court = {COURT_Y, COURT_Y + COURT_H, COURT_X, COURT_X + COURT_W};
TFT TFTscreen = TFT(CS, DC, RST);
Button mode_button(4); // Cycle through state_t modes
Button next_button(5); // Cycle through time values in each state_t

// Sketch globals
unsigned clk_hour = 0;
unsigned clk_minute = 0;
unsigned clk_second = 0;
bool clk_half = true;
bool clk_alarm = false;
unsigned clk_alarm_hour = 0;
unsigned clk_alarm_minute = 0;
state_t clk_mode = normal;


/* DOCS */
// TimerOne interrupt serve routine
void timer1_isr();
// Main clock logic
void update_time();//(unsigned*, unsigned*);
// Redraw time values if changed
void draw_time(unsigned, unsigned, unsigned);
// Draw all digits
void force_draw(unsigned, unsigned, unsigned);
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

  force_draw(clk_hour, clk_minute, clk_second);
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
  clk_half = !clk_half;

  if (clk_half) {
    update_time();
    draw_time(clk_hour, clk_minute, clk_second);
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
      clk_mode = normal;
      break;

  }
}

void update_next(state_t mode) {
  switch (mode) {
    case set_normal:
      // DO NOTHING
      break;
    case set_alarm_active:
      clk_alarm = !clk_alarm;
      break;
    case set_alarm_hour:
      clk_alarm_hour = (clk_alarm_hour + 1) % 24;
      break;
    case set_alarm_min:
      clk_alarm_min = (clk_alarm_min + 1) % 60;
      break;
    case set_hour:
      clk_hour = (clk_hour + 1) % 24;
      break;
    case set_min:
      clk_minute = (clk_minute + 1) % 60;
      break;
    case set_sec:
      clk_second = 0;
      break;
  }
}


void update_time() {
  clk_second++;
  if (clk_second == 60) {
    clk_second = 0;
    clk_minute++;					// Update minute
    if (clk_minute == 60) {
      clk_minute = 0;
      clk_hour++;					// Update hour
      if (clk_hour == 24) {
        clk_hour = 0;
      }
    }
  }
}

void force_draw(unsigned hour, unsigned minute, unsigned second) {
  draw_digit(second % 10, second_units);
  draw_digit(second / 10, second_tens);
  draw_digit(minute % 10, minute_units);
  draw_digit(minute / 10, minute_tens);
  draw_digit(hour % 10, hour_units);
  draw_digit(hour / 10, hour_tens);
}

void draw_time(unsigned hour, unsigned minute, unsigned second) {
  draw_digit(second % 10, second_units);
  if (second % 10 == 0) {
    draw_digit(second / 10, second_tens);
    if (second / 10 == 0) {
      draw_digit(minute % 10, minute_units);
      if (minute % 10 == 0) {
        draw_digit(minute / 10, minute_tens);
        if (minute / 10 == 0) {
          draw_digit(hour % 10, hour_units);
          if (hour % 10 == 0) {
            draw_digit(hour / 10, hour_tens);
          }
        }
      }
    }
  }
}

void draw_digit(unsigned digit, digit_t digt) {
  int posX = get_digit_pos(digt);
  int offset = digit * MEM_OFFSET;
  for (int row = 0; row < DIGIT_H; row++) {
    for (int col = 0; col < DIGIT_W; col++) {
      word p = pgm_read_word(digits + offset + (row * DIGIT_W + col));
      TFTscreen.drawPixel(col + posX, row + INIT_Y, p);
    }
  }
}

int get_digit_pos(digit_t dt) {
  static int space_x = DIGIT_W + DIGIT_OFFSET;
  static int dots_x = DIGIT_W / 2 + DIGIT_OFFSET;
  int x = 0;
  switch (dt) {
    case second_units:
      x += space_x;
    case second_tens:
      x += space_x + dots_x;
    case minute_units:
      x += space_x;
    case minute_tens:
      x += space_x + dots_x;
    case hour_units:
      x += space_x;
    case hour_tens:
      x += INIT_X;
  }
  return x;
}
