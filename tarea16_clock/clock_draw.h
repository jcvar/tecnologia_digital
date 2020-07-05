// Drawing defines
#define INIT_X 10
#define INIT_Y 51
#define DIGIT_W 16
#define DIGIT_H 24
#define BLOCK_W 52
#define BLOCK_H 4
#define DIGIT_OFFSET 4
#define MEM_OFFSET 384 // W*H

TFT TFTscreen = TFT(CS, DC, RST);

// Get the x-axis coordinate for a given digit
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


// Access selected digit image and draw at digit_t
void draw_digit(unsigned short digit, digit_t digt) {
	int posX = get_digit_pos(digt);
	int offset = digit * MEM_OFFSET;
	for (int row = 0; row < DIGIT_H; row++) {
		for (int col = 0; col < DIGIT_W; col++) {
			word p = pgm_read_word(digits + offset + (row * DIGIT_W + col));
			TFTscreen.drawPixel(col + posX, row + INIT_Y, p);
		}
	}
}

// Draw all digits
void force_draw(time_t t) {
	draw_digit(t.second % 10, second_units);
	draw_digit(t.second / 10, second_tens);
	draw_digit(t.minute % 10, minute_units);
	draw_digit(t.minute / 10, minute_tens);
	draw_digit(t.hour % 10, hour_units);
	draw_digit(t.hour / 10, hour_tens);
}

// Redraw time values if changed
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

// Draw mode indicator
void draw_mode(state_t mode) {
	TFTscreen.drawRect(COURT_X + 1, INIT_Y + DIGIT_H + 1, COURT_W - 2, BLOCK_H, COLOR_BLACK);
	switch (mode) {
		case set_normal:
			// Do nothing;
			break;
		case set_alarm_active:
			TFTscreen.drawRect(get_digit_pos(second_tens), INIT_Y + DIGIT_H + 1, BLOCK_W, BLOCK_H, COLOR_RED);
			break;
		case set_alarm_hour:
			TFTscreen.drawRect(get_digit_pos(hour_tens), INIT_Y + DIGIT_H + 1, BLOCK_W, BLOCK_H, COLOR_RED);
			break;
		case set_alarm_minute:
			TFTscreen.drawRect(get_digit_pos(minute_tens), INIT_Y + DIGIT_H + 1, BLOCK_W, BLOCK_H, COLOR_RED);
			break;
		case set_hour:
			TFTscreen.drawRect(get_digit_pos(hour_tens), INIT_Y + DIGIT_H + 1, BLOCK_W, BLOCK_H, COLOR_GREEN);
			break;
		case set_minute:
			TFTscreen.drawRect(get_digit_pos(minute_tens), INIT_Y + DIGIT_H + 1, BLOCK_W, BLOCK_H, COLOR_GREEN);
			break;
		case set_sec:
			TFTscreen.drawRect(get_digit_pos(second_tens), INIT_Y + DIGIT_H + 1, BLOCK_W, BLOCK_H, COLOR_GREEN);
			break;
	}
}
