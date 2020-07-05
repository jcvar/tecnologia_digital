// This module contains the functions that interact with the TFT screen

#define INIT_X 10
#define INIT_Y 51
#define DIGIT_W 16
#define DIGIT_H 24
#define BLOCK_W 52
#define BLOCK_H 4
#define DIGIT_OFFSET 4
#define MEM_OFFSET 384 // W*H

// TFT screen instance
TFT TFTscreen = TFT(CS, DC, RST);

// get_digit_pos: Get the x-axis coordinate for a given digit
int get_digit_pos(digit_t dgt) {
	static int space_x = DIGIT_W + DIGIT_OFFSET;
	static int dots_x = DIGIT_W / 2 + DIGIT_OFFSET;
	int x = 0;
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
		x += INIT_X;
	}
	return x;
}

// draw_digit: Reads a digit's image from memory and draw it in the screen
void draw_digit(unsigned short digit, digit_t dgt) {
	int posX = get_digit_pos(dgt);
	int offset = digit * MEM_OFFSET;
	for (int row = 0; row < DIGIT_H; row++) {
		for (int col = 0; col < DIGIT_W; col++) {
			word p = pgm_read_word(digits + offset + (row * DIGIT_W + col));
			TFTscreen.drawPixel(col + posX, row + INIT_Y, p);
		}
	}
}

// force_draw: Draw all digits
void force_draw(mytime_t t) {
	draw_digit(t.second % 10, second_units);
	draw_digit(t.second / 10, second_tens);
	draw_digit(t.minute % 10, minute_units);
	draw_digit(t.minute / 10, minute_tens);
	draw_digit(t.hour % 10, hour_units);
	draw_digit(t.hour / 10, hour_tens);
}

// draw_time: Reads a mytime_t and calls draw_digit for every digit that changed
void draw_time(mytime_t t) {
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

// FIXME
// draw_mode: Draws an indicator to show the current state
void draw_mode(state_t mode) {
	int pos = 0;
	word cor = COLOR_RED;
	// Erase the previous indicator
	TFTscreen.drawRect(COURT_X + 1, INIT_Y + DIGIT_H + 1, COURT_W - 2, BLOCK_H, COLOR_BLACK);
	switch (mode) {
	case set_normal: // Do nothing;
		break;
	case set_alarm_active:
		pos = get_digit_pos(second_tens);
		cor = COLOR_RED;
		break;
	case set_alarm_hour:
		pos = get_digit_pos(hour_tens);
		cor = COLOR_RED;
		break;
	case set_alarm_minute:
		pos = get_digit_pos(minute_tens);
		cor = COLOR_RED;
		break;
	case set_hour:
		pos = get_digit_pos(hour_tens);
		cor = COLOR_GREEN;
		break;
	case set_minute:
		pos = get_digit_pos(second_tens);
		cor = COLOR_GREEN;
		break;
	case set_sec:
		break;
	}
		TFTscreen.drawRect(pos, INIT_Y + DIGIT_H + 1, BLOCK_W, BLOCK_H, cor);
}
