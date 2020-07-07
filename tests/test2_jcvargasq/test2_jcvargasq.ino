/*
Parcial 2
Animacion de imágenes

Juan Camilo Vargas Q.

Tecnologia Digital
Universidad Nacional de Colombia
2020-07-03
*/
#include "common.h"
#include "x_img.h"

#define COLOR_GRAY 0x7BEF
#define COLOR_FILL_GREEN 0x4D47
#define COLOR_FILL_MAGENTA 0x90F5
#define IMG_W 32
#define IMG_H 32

#define RECT_X 16
#define RECT_Y 16
#define RECT_W 128
#define RECT_H 96
#define CHAR_X 14
#define CHAR_Y 12

#define STATE_MILLIS 1000

TFT TFTscreen = TFT(CS, DC, RST);

typedef enum {
	s0, s1, s2, s3, s4, s5, s6, s7
} state_t;
state_t sketch_state = s0;

void setup() {
	// Initialize screen
	TFTscreen.begin();
	TFTscreen.stroke(COLOR_WHITE);
}

void loop() {
	static unsigned long loop_millis = 0;
	if (millis() - loop_millis > STATE_MILLIS){
		loop_millis = millis();
		state_machine();
	}
}

void draw_image(int pos_x, int pos_y, bool alt_cor){
	for (int row = 0; row < IMG_H; row++) {
		for (int col = 0; col < IMG_W; col++) {
			word p = pgm_read_word(x_img + (row * IMG_W + col));
			if (p == 0x07E0){
				p = alt_cor ? COLOR_FILL_MAGENTA : COLOR_FILL_GREEN;
			}
			TFTscreen.drawPixel(col + pos_x, row + pos_y, p);
		}
	}
}

void state_machine(){
	switch(sketch_state){
		case s0: // Clear screen with gray background
			TFTscreen.background(COLOR_GRAY);
			
			sketch_state = s1;
			break;
		case s1: // Draw red rectangle
			TFTscreen.drawRect(RECT_X, RECT_Y, RECT_W, RECT_H, COLOR_RED);
			
			sketch_state = s2;
			break;
		case s2: // 1 in upper left img
			draw_image(0, 0, false);
			TFTscreen.text("1", 0 + CHAR_X, 0 + CHAR_Y);
			
			sketch_state = s3;
			break;
		case s3: // 2 in upper right img
			draw_image(RECT_W, 0, false);
			TFTscreen.text("2", RECT_W + CHAR_X, 0 + CHAR_Y);
			
			sketch_state = s4;
			break;
		case s4: // 3 in lower right img
			draw_image(RECT_W, RECT_H, false);
			TFTscreen.text("3", RECT_W + CHAR_X, RECT_H + CHAR_Y);
			
			sketch_state = s5;
			break;
		case s5: // 4 in lower left img
			draw_image(0, RECT_H, false);
			TFTscreen.text("4", 0 + CHAR_X, RECT_H + CHAR_Y);
			
			sketch_state = s6;
			break;
		case s6: // 5 in center
			TFTscreen.drawFastHLine(RECT_X, 64, RECT_W, COLOR_BLUE);
			TFTscreen.drawFastVLine(80, RECT_Y, RECT_H, COLOR_BLUE);
			draw_image(64, 48, true);
			TFTscreen.text("5", 64 + CHAR_X, 48 + CHAR_Y);
			
			sketch_state = s0;
			break;
	} // END SWITCH(sketch_state)
} // END STATE_MACHINE()


