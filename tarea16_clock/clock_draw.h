// This module contains the functions that interact with the TFT screen

// Dimensions of a digit's image
#define DIGIT_W 16
#define DIGIT_H 24
#define MEM_OFFSET   384 // W*H

// Reference points for the digits' positions
#define CLOCK_X  10
#define CLOCK_Y  24
#define ALARM_Y  80
#define DIGIT_OFFSET 4

// Dimensions of the indicator block
#define BLOCK_W 36
#define BLOCK_H 4

// TFT screen instance
TFT TFTscreen = TFT(CS, DC, RST);

int get_pos_x(digit_t dgt) {
  static int space_x = DIGIT_W + DIGIT_OFFSET;
  static int dots_x = DIGIT_W / 2 + DIGIT_OFFSET;
  static int x = 0;
  x = 0;
  switch (dgt) {
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
      x += CLOCK_X;
  }
  return x;
}

int get_pos_y(bool is_alarm) {
  return is_alarm ? ALARM_Y : CLOCK_Y;
}

// draw_digit: Reads a digit's image from memory and draw it in the screen
void draw_digit(unsigned short digit, digit_t dgt, bool is_alarm) {
  static int pos_x = 0;
  static int pos_y = 0;
  static int offset = 0;
  pos_x = get_pos_x(dgt);
  pos_y = get_pos_y(is_alarm);
  offset = digit * MEM_OFFSET;
  for (int row = 0; row < DIGIT_H; row++) {
    for (int col = 0; col < DIGIT_W; col++) {
      word p = pgm_read_word(digits + offset + (row * DIGIT_W + col));
      TFTscreen.drawPixel(col + pos_x, row + pos_y, p);
    }
  }
}


void draw_alarm(bool alarm_active) {
  static int pos_x = get_pos_x(second_tens);
  static int pos_y = ALARM_Y;
  if(alarm_active){
    TFTscreen.fillRect(pos_x, pos_y, BLOCK_W, DIGIT_H, COLOR_YELLOW);
  } else {
    TFTscreen.fillRect(pos_x, pos_y, BLOCK_W, DIGIT_H, COLOR_BLACK);
  }
}

// draw_time: Reads a mytime_t and calls draw_digit for every digit that changed
void draw_time(mytime_t t) {
  draw_digit(t.second % 10, second_units, false);
  if (t.second % 10 == 0) {
    draw_digit(t.second / 10, second_tens, false);
    if (t.second / 10 == 0) {
      draw_digit(t.minute % 10, minute_units, false);
      if (t.minute % 10 == 0) {
        draw_digit(t.minute / 10, minute_tens, false);
        if (t.minute / 10 == 0) {
          draw_digit(t.hour % 10, hour_units, false);
          if (t.hour % 10 == 0) {
            draw_digit(t.hour / 10, hour_tens, false);
          }
        }
      }
    }
  }
}

// force_draw: Draw all digits
void force_draw(mytime_t t, bool is_alarm) {
  if (is_alarm) {
    draw_alarm(t.active);
  } else {
    draw_digit(t.second % 10, second_units, false);
    draw_digit(t.second / 10, second_tens, false);
  }
  draw_digit(t.minute % 10, minute_units, is_alarm);
  draw_digit(t.minute / 10, minute_tens, is_alarm);
  draw_digit(t.hour % 10, hour_units, is_alarm);
  draw_digit(t.hour / 10, hour_tens, is_alarm);
}

// FIXME
// draw_state_indicator: Draws an indicator to show the current state
void draw_state_indicator(state_t state) {
  static int pos_x = CLOCK_X;
  static int pos_y = CLOCK_Y;
  static word cor = COLOR_BLACK;
  switch (state) {
    case set_alarm_active:
      pos_x = get_pos_x(second_tens);
      pos_y = get_pos_y(true);
      cor = COLOR_RED;
      break;
    case set_alarm_hour:
      pos_x = get_pos_x(hour_tens);
      pos_y = get_pos_y(true);
      cor = COLOR_RED;
      break;
    case set_alarm_minute:
      pos_x = get_pos_x(minute_tens);
      pos_y = get_pos_y(true);
      cor = COLOR_RED;
      break;
    case set_clock_hour:
      pos_x = get_pos_x(hour_tens);
      pos_y = get_pos_y(false);
      cor = COLOR_GREEN;
      break;
    case set_clock_minute:
      pos_x = get_pos_x(minute_tens);
      pos_y = get_pos_y(false);
      cor = COLOR_GREEN;
      break;
    case set_clock_second:
      pos_x = get_pos_x(second_tens);
      pos_y = get_pos_y(false);
      cor = COLOR_GREEN;
    default: // normal
      cor = COLOR_BLACK;
      break;
  }
  // Erase the previous indicators
  TFTscreen.fillRect(COURT_X + 1, CLOCK_Y + DIGIT_H + 1, COURT_W - 2, BLOCK_H, COLOR_BLACK);
  TFTscreen.fillRect(COURT_X + 1, ALARM_Y + DIGIT_H + 1, COURT_W - 2, BLOCK_H, COLOR_BLACK);
  // Draw new indicator
  TFTscreen.fillRect(pos_x, pos_y + DIGIT_H + 1, BLOCK_W, BLOCK_H, cor);
}
