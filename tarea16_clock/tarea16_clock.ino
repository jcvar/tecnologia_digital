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
#define INIT_X 30
#define INIT_Y 51
#define DIGIT_W 16
#define DIGIT_H 24
#define DIGIT_OFFSET 4
#define MEM_OFFSET 384 // W*H

typedef unsigned short hu; // shorthand

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


court_t court;
TFT TFTscreen = TFT(CS, DC, RST);
Button mode_button(4); // Cycle through state_t modes
Button next_button(5); // Cycle through time values in each state_t

// Sketch globals
hu hour = 0;
hu minute = 0;
hu second = 0;
state_t state = normal;

void update_time();//(hu*, hu*);
void draw_time(hu, hu);
void draw_digit(hu, digit_t);
int get_digit_pos(digit_t);

void setup() {
  // Initialize screen
  TFTscreen.begin(INITR_BLACKTAB);
  TFTscreen.background(COLOR_BLACK);

  // Court initialization
  court.top = COURT_Y;
  court.bottom = COURT_Y + COURT_H;
  court.left = COURT_X;
  court.right = COURT_X + COURT_W;
  // Draw court
  TFTscreen.drawRect(COURT_X, COURT_Y, COURT_W, COURT_H, COLOR_RED);


  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(update_time);//declare the interrupt serve routine:TimingISR

  enterButton.begin();
  selectButton.begin();

  force_draw(hour, minute);
} // END SETUP

void loop() {
  static unsigned long clock_millis = 0;

  enterButton.read();
  selectButton.read();

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


} // END LOOP

void set_time() {

}

void update_time() {	//(hu *hour, hu *minute) {
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
  draw_time(hour, minute);
}

void force_draw(hu hour, hu minute) {
  draw_digit(minute % 10, minute_units);
  draw_digit(minute / 10, minute_tens);
  draw_digit(hour % 10, hour_units);
  draw_digit(hour / 10, hour_tens);
}

void draw_time(hu hour, hu minute) {
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

// Draw a digit
void draw_digit(hu digit, digit_t digt) {
  int posX = get_digit_pos(digt);
  int offset = digit * MEM_OFFSET;
  for (int row = 0; row < DIGIT_H; row++) {
    for (int col = 0; col < DIGIT_W; col++) {
      word p = pgm_read_word(digits + offset + (row * DIGIT_W + col));
      TFTscreen.drawPixel(col + posX, row + INIT_Y, p);
    }
  }
}

// Get the x-axis coordinate for a given digit
int get_digit_pos(digit_t dt) {
  int x = INIT_X;
  switch (dt) {
    case minute_units:
      x += (DIGIT_W + DIGIT_OFFSET) * 4;
      break;
    case minute_tens:
      x += (DIGIT_W + DIGIT_OFFSET) * 3;
      break;
    case hour_units:
      x += (DIGIT_W + DIGIT_OFFSET);
      break;
    case hour_tens:
      break;
  }
  return x;
}
