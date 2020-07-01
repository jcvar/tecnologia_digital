void draw_mode(){}

void force_draw(time_t t) {
  draw_digit(t.second % 10, second_units);
  draw_digit(t.second / 10, second_tens);
  draw_digit(t.minute % 10, minute_units);
  draw_digit(t.minute / 10, minute_tens);
  draw_digit(t.hour % 10, hour_units);
  draw_digit(t.hour / 10, hour_tens);
}

void draw_time(time_t t) {
  draw_digit(t.second % 10, second_units);
  if (t.second % 10 == 0) {
    draw_digit(t.second / 10, second_tens);
    if (t.second / 10 == 0) {
      draw_digit(t.minute % 10, minute_units);
      if (t.minute % 10 == 0) {
        draw_digit(t.minute / 10, minute_tens);
        if (t.minute / 10 == 0) {
          draw_digit(t.hour % 10, hour_units);
          if (t.hour % 10 == 0) {
            draw_digit(t.hour / 10, hour_tens);
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
