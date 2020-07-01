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

#define MILLIS_MINUTE  100 // 60000

// Drawing defines
#define INIT_X 10
#define INIT_Y 51
#define DIGIT_W 16
#define DIGIT_H 24
#define DIGIT_OFFSET 4
#define MEM_OFFSET 384 // W*H

// Enum for global clock states
typedef enum {
  normal,
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
unsigned hour = 0;
unsigned minute = 0;
unsigned second = 0;
bool half = true;
state_t mode = normal;


/* DOCS */
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

  force_draw(hour, minute, second);
} // END SETUP

void loop() {
  static unsigned long clock_millis = 0;

  //enterButton.read();
  //selectButton.read();

  /*
    if (millis() - clock_millis > MILLIS_MINUTE) {
    if (enterButton.wasPressed()) {
      hour = (hour + 1) % 24;
      force_draw(hour, minute);
    }
    if (selectButton.wasPressed()) {
      minute = (minute + 1) % 60;
      force_draw(hour, minute);
    }
    //  	clock_millis = millis();
    //  	update_time();//(hour, minute);
    //  	draw_time(hour, minute);
    }
  */

} // END LOOP

void timer1_isr() {
  half = !half;

  if (half) {
    update_time();
  }

}

void update_time() {	//(unsigned *hour, unsigned *minute) {
  second++;
  if (second == 60) {
    second = 0;
    minute++;					// Update minute
    if (minute == 60) {
      minute = 0;
      hour++;					// Update hour
      if (hour == 24) {
        hour = 0;
      }
    }
  }
  draw_time(hour, minute, second);
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
