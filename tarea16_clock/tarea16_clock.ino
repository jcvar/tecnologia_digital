/*
  Tarea 16
  Clock

  Juan Camilo Vargas Q.
  Sergio Alejandro Vargas Q.

  Tecnologia Digital
  Universidad Nacional de Colombia
  2020-06-24
*/
#include <JC_Button.h>
#include <TimerOne.h>

#include "common.h"
#include "digits_small.h"
#include "clock.h"
#include "clock_draw.h"

#define MILLIS_LOOP 200
#define BUZZER_PIN 7

// Global structs, enum and button variables
mytime_t clk = {0, 0, 0, true};
mytime_t alarm = {0, 0, 0, false};
state_t clk_state = normal;
Button mode_button(4); // To cycle through the clock's states
Button next_button(5);  // To cycle through time values
pinMode(BUZZER_PIN, OUTPUT); // To buzz!
bool buzzer_on = false;

/* FUNCTIONS */
void timer1_isr();  // TimerOne Interrupt Service Routine
void update_time(); // Main clock logic. Increase values in the clk struct
state_t update_state(state_t); // Handles the clock's FSM
void update_next(state_t); // Changes values of mytime_t according to state_t
void choose_draw(state_t); // Select which mytime_t to force_draw
bool check_alarm();


void setup() {
  //TFTscreen.begin(); // For simulator
  TFTscreen.begin(INITR_BLACKTAB); // INITR_BLACKTAB for BGR
  TFTscreen.background(COLOR_BLACK);

  // TimerOne
  Timer1.initialize(500000); //timing for 500ms
  Timer1.attachInterrupt(timer1_isr); //declare the Interrupt Service routine (ISR): timer1_isr
  // Button
  mode_button.begin();
  next_button.begin();
  //
  force_draw(clk, false);
  force_draw(alarm, true);

  //Serial.begin(9600); // DEBUG
} // END SETUP

void loop() {
  static unsigned long loop_millis = 0;
  static bool mode_flag = false;
  static bool next_flag = false;

  // Read buttons
  mode_button.read();
  if (mode_button.wasPressed()) {
    mode_flag = true;
  }
  next_button.read();
  if (next_button.wasPressed()) {
    next_flag = true;
  }

  if (millis() - loop_millis > MILLIS_LOOP) {
    loop_millis = millis();
    // BUTTONS LOGIC
    if (mode_flag) {
      mode_flag = false;
      clk_state = update_state(clk_state);
      draw_state_indicator(clk_state);
    }
    if (next_flag) {
      next_flag = false;
      update_next(clk_state);
      choose_draw(clk_state);
    }
    //Serial.println(clk_state); // DEBUG
  }
} // END LOOP

void update_time() {
  clk.second++;
  if (clk.second == 60) {
    clk.second = 0;
    clk.minute++;          // Update minute
    if (clk.minute == 60) {
      clk.minute = 0;
      clk.hour++;          // Update hour
      if (clk.hour == 24) {
        clk.hour = 0;
      }
    }
  }
}

void timer1_isr() {
  // TODO: Blink should only happen on set_<clock/alarm> states
  // clk.active flags is used to produce a blink effect?
  clk.active = !clk.active;

  if (clk.active) {
    update_time();
    draw_time(clk);
    buzzer_on = check_alarm();
    if (buzzer_on) {
      digitalWrite(BUZZER_PIN, HIGH);
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    // Erase digit images
  }
}

state_t update_state(state_t old_state) {
  switch (old_state) {
    case normal:            return set_alarm_active;
    case set_alarm_active:  return set_alarm_minute;
    case set_alarm_minute:  return set_alarm_hour;
    case set_alarm_hour:    return set_clock_second;
    case set_clock_second:  return set_clock_minute;
    case set_clock_minute:  return set_clock_hour;
    case set_clock_hour:    return normal;
    default:                return normal;
  }
}

void update_next(state_t cur_state) {
  // TODO: Take a mytime_t parameter?
  switch (cur_state) {
    case normal:
      buzzer_on = false;
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
    case set_clock_hour:
      clk.hour = (clk.hour + 1) % 24;
      break;
    case set_clock_minute:
      clk.minute = (clk.minute + 1) % 60;
      break;
    case set_clock_second:
      clk.second = 0;
      break;
  }
}


void choose_draw(state_t cur_state) {
  // TODO: Take a mytime_t parameter?
  switch (cur_state) {
    case normal:
      // DO NOTHING
      break;
    case set_alarm_active:
    case set_alarm_hour:
    case set_alarm_minute:
      force_draw(alarm, true);
      break;
    case set_clock_hour:
    case set_clock_minute:
    case set_clock_second:
      force_draw(clk, false);
      break;
  }
}

bool check_alarm() {
  if (alarm.active) {
    if (alarm.hour == clk.hour && alarm.minute == clk.minute) {
        return true;
      }
  }
  return false;
}

